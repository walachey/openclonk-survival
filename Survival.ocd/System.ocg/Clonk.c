#appendto Clonk

static all_player_character_sheets;

public func Construction()
{
	this.BleedDamage = 0;
	this.FractureDamage = 0;
	this.Daze = 0;
	this.MaxHunger = 1000;
	this.MaxThirst = 1000;
	this.Hunger = 0;
	this.Thirst = 0;
	this.HitpointRegenerationSpeed = 50;
	AddTimer("DegenerateDamage", 10);
	
	// Make ActMap writeable.
	if (ActMap == this.Prototype.ActMap)
		ActMap = { Prototype = this.Prototype.ActMap };
	
	// Add "unconscious" action.
	ActMap.Unconscious = {
		Name = "Unconscious",
		Prototype = ActMap.Dead,
		StartCall = "StartUnconscious",
		StopCall = "StopUnconscious",
		AbortCall = "StopUnconscious",
		NoOtherAction = 0,
	};
	
	return _inherited(...);
}

public func Recruitment()
{	
	if (all_player_character_sheets == nil)
		all_player_character_sheets = [];
	if (all_player_character_sheets[GetOwner()] == nil)
	{
		all_player_character_sheets[GetOwner()] = Stats->Init();
		this.character = all_player_character_sheets[GetOwner()];
	}
	
	PlayerHUD->Update(this);
	
	return _inherited(...);
}

private func StartUnconscious()
{
	if (this.UnconsciousAnimation == nil)
		this.UnconsciousAnimation = PlayAnimation("Dead", CLONK_ANIM_SLOT_Death, Anim_Linear(0, 0, GetAnimationLength("Dead"), 20, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Hold));
	// Update carried items
	this->UpdateAttach();
	// Set proper turn type
	this->SetTurnType(1);
}

private func StopUnconscious()
{
	if (this.UnconsciousAnimation != nil)
	{
		SetAnimationWeight(this.UnconsciousAnimation, Anim_Linear(1000, 1000, 0, 10, ANIM_Remove));
		var fx = AddEffect("IntScheduleRemoveAnimation", this, 1, 5, this);
		fx.anim = this.UnconsciousAnimation;
	}
	this.UnconsciousAnimation = nil;
}

private func FxIntScheduleRemoveAnimationTimer(object target, effect fx)
{
	StopAnimation(fx.anim);
	return -1;
}

private func DegenerateDamage()
{
	this.BleedDamage = Max(this.BleedDamage - 10000, 0);
	this.FractureDamage = Max(this.FractureDamage - 10000, 0);
	this.Daze = Max(this.Daze - 1000, 0);
	
	var hunger_rate = 1;
	
	if (this.Hunger == this.MaxHunger || this.Thirst == this.MaxThirst)
	{
		DealDamage(this, 50, DMG_True);
	}
	else // regenerate!
	{
		var hp_missing = this.MaxEnergy - GetEnergy() * 1000;
	
		if (hp_missing > 0)
		{
			var rate = this.HitpointRegenerationSpeed;
			if (this.Hunger < this.MaxHunger/2) rate *= 2;
			else if (this.Hunger > 3 * this.MaxHunger / 4) rate = 0;
			if (rate != 0)
			{
				DoEnergy(rate, true,  FX_Call_EngBaseRefresh);
				hunger_rate += rate / 10;
			}
		}
	}
	
	DoHunger(hunger_rate);
	DoThirst(1);
}

public func DoHunger(int change)
{
	this.Hunger = BoundBy(this.Hunger + change, 0, this.MaxHunger);
}

public func DoThirst(int change)
{
	this.Thirst = BoundBy(this.Thirst + change, 0, this.MaxThirst);
}

public func Eat(object food)
{
	if(GetProcedure() == "WALK")
	{
		DoHunger(-food->~NutritionalValue());
		DoThirst(-food->~WaterValue());
		food->RemoveObject();
		Sound("Munch?");
		SetAction("Eat");
	}
}

public func DoDaze(int amount)
{
	this.Daze = Max(this.Daze + amount, 0);
	
	if (amount > 0)
	{
		if (CheckStatusTriggered(this.Daze / 100, 500, 1000))
			TryApplyStatusEffect("StatusUnconscious");
	}
}

public func OnDamage(object by, int exact_damage, int type, int by_player)
{
	if (exact_damage <= 0) return;
	
	var bleed_strength = 0;
	var fracture_strength = 0;
	if (type == DMG_Blunt)
	{
		fracture_strength = 100;
		bleed_strength = 25;
	}
	else if (type == DMG_Slash)
	{
		fracture_strength = 0;
		bleed_strength = 100;
	}
	else if (type == DMG_Pierce)
	{
		fracture_strength = 50;
		bleed_strength = 50;
	}
	
	if (bleed_strength > 0) CheckBleed(exact_damage * 100 / bleed_strength);
	if (fracture_strength > 0) CheckFracture(exact_damage * 100 / fracture_strength);
}

private func CheckStatusTriggered(int value, int stddev, int target)
{
	var random = Abs(NormalRandom(0, stddev));
	if (value + random >= target) return true;
	return false;
}

private func CheckBleed(int dmg)
{
	this.BleedDamage += dmg;
	if (CheckStatusTriggered(this.BleedDamage / 100, 500, 1000))
		TryApplyStatusEffect("StatusBleeding");
}

private func CheckFracture(int dmg)
{
	this.FractureDamage += dmg;
	if (CheckStatusTriggered(this.FractureDamage / 100, 500, 1000))
		TryApplyStatusEffect("StatusFracture");
}

private func TryApplyStatusEffect(string name)
{
	if (GetEffect(name, this)) return;
	AddEffect(name, this, 1, 5, this);
}

private func FxStatusBleedingStart(object target, effect fx, temp)
{
	if (temp) return;
	fx.particles =
	{
		Prototype = Particles_Material(RGB(255, 0, 0)),
		ForceX = PV_Random(-5, 5),
		DampingX = 950,
		OnCollision = PC_Stop(),
		Alpha = PV_Linear(255, 0),
	};
}

private func FxStatusBleedingTimer(object target, effect fx, int time)
{
	if (!Random(5))
		target->CreateParticleAtBone("SphereSpark", "skeleton_head", [0, 0, 0], [0, 0, -RandomX(200, 300)], PV_Random(10, 30), fx.particles, 20); 
	DealDamage(target, 100, DMG_True);
}

private func FxStatusFractureStart(object target, effect fx, temp)
{
	if (temp) return;
	target->PushActionSpeed("Walk", 100);
	target->PushActionSpeed("Jump", 200);
}

private func FxStatusFractureStop(object target, effect fx, int reason, temp)
{
	if (temp) return;
	target->PopActionSpeed("Walk");
	target->PopActionSpeed("Jump");
}

private func FxStatusUnconsciousStart(object target, effect fx, temp)
{
	if (temp) return;
	target->SetAction("Unconscious");
	target->SetTurnForced(target->GetDir());
}

private func FxStatusUnconsciousTimer(object target, effect fx)
{
	if (target.Daze <= 0) return FX_Execute_Kill;
	return FX_OK;
}

private func FxStatusUnconsciousStop(object target, effect fx, int reason, temp)
{
	if (temp || !target) return;
	if (target->GetAlive())
	{
		target->SetAction("Kneel");
		target->SetTurnForced(-1);
	
		var duration = 15;
		var length = target->GetAnimationLength("KneelDown");
		target->PlayAnimation("KneelDown", CLONK_ANIM_SLOT_Movement, Anim_Linear(length, length, 0, duration, ANIM_Remove), Anim_Linear(0, 0, 1000, 5, ANIM_Hold));
		ScheduleCall(target, "EndKneel", duration, 1);
	}
}

private func MakeProgressBar(string text, id symbol, int color, int color_back)
{
	var container = 
	{
		Bottom = "2em",
		text = 
		{
			Style = GUI_NoCrop | GUI_TextLeft | GUI_TextVCenter,
			Right = "10em",
			Text = text
		},
		bar_container = 
		{
			Left = "10em",
			Margin = "0.5em",
			BackgroundColor = RGB(200, 200, 200),
			
			bar_background = 
			{
				Margin = "0.1em",
				BackgroundColor = color_back,
				
				bar = 
				{
					Margin = "0.05em",
					BackgroundColor = color,
					
					symbol = 
					{
						Symbol = Icon_Menu_DarkCircle,
						Left = "100% - 1em",
						Right = "100% + 1em",
						
						actual_symbol = {Symbol = symbol}
					}
				}
			}
		}
	};
	return container;
}

private func OpenStatusUI()
{
	var menu = 
	{
		Style = GUI_Multiple | GUI_FitChildren | GUI_VerticalLayout,
		Left = "2em", Right = "40em",
		
	};
	this.status_ui_id = GuiOpen(menu);
}

private func CloseStatusUI()
{
	if (!this.status_ui_id) return;
	GuiClose(this.status_ui_id);
}

public func OnShiftCursor()
{
	CloseStatusUI();
}

public func OnSelection()
{
	OpenStatusUI();
}