#pragma once
struct FractionParticleComponent
{
	FractionParticleComponent(float xV, float yV)
		: XVelocity(xV), YVelocity(yV)
	{
		
	}

	float XVelocity;
	float YVelocity;
	float Gravity = -980.0f;
	float LifeTime = 1.5f;
	float Life = 0;
};