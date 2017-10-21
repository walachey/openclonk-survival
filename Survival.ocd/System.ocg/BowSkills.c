static const ChargedShotBowEffect = new Effect
{
	Construction = func()
	{
		this.charged = 0;
		this.charge_time = 35 * 4;
		this.particles = new Particles_Glimmer()
		{
			ForceY = PV_Random(-5, 5, 5),
			ForceX = PV_Random(-5, 5, 5),
		};
	},
	Timer = func(int time)
	{
		if (!this.charged) return FX_OK;
		if (this.charge_time > 0)
		{
			this.charge_time -= this.Interval;
			var r = 10 + this.charge_time / 5; 
			this.Target->CreateParticle("SphereSpark", PV_Sin(PV_Random(0, 360, 0, 1), r), PV_Cos(PV_Random(0, 360, 0, 1), r), 0, 0, 10, this.particles, 30);
		}
		else
		{
			this.Target->CreateParticleAtBone("SphereSpark", "pos_hand2", [0, 0, 0], [0, 0, 0], 40, this.particles, 10);
		}
		return FX_OK;
	},
	OnCanceledAiming = func()
	{
		RemoveEffect(nil, nil, this);
	},
	OnLoadingFinished = func()
	{
		this.charged = true;
	},
	OnArrowFired = func(object arrow, object bow, int angle, int shooting_strength)
	{
		if (this.charge_time > 0) return;
		arrow->SetVelocity(angle, 150 * shooting_strength / 100);
		var fx = arrow->CreateEffect(ChargedShotArrowTrailEffect, 1, 1);
		fx.particles = this.particles;
		this.Target->ShakeViewport(30, 0, 0, 10);
		RemoveEffect(nil, nil, this);
	}
};

static const ChargedShotArrowTrailEffect = new Effect
{
	Timer = func(int time)
	{
		if (time > 30) return FX_Execute_Kill;
		this.Target->CreateParticle("SphereSpark", 0, 0, 0, 0, 90, this.particles, 5);
		this.Target->CreateParticle("SphereSpark", -this.Target->GetXDir()/20, -this.Target->GetYDir()/20, 0, 0, 90, this.particles, 5);
	}
};
