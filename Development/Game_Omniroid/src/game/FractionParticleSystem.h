#pragma once
#include "../gameWorld/systems/ISystem.h"
class FractionParticleSystem : public ISystem
{
public:
	virtual void Update(entt::registry& registry) override;

};