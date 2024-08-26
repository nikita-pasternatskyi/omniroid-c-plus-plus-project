#include "FractionParticleSystem.h"
#include "FractionParticleComponent.h"
#include "../gameWorld/systems/TransformComponent.h"
#include "../time/Time.h"

void FractionParticleSystem::Update(entt::registry& registry)
{
	auto view = registry.view<FractionParticleComponent, TransformComponent>();
	std::vector<entt::entity> entitiesToDestroy;
	for (auto entity : view)
	{
		auto& transform = view.get<TransformComponent>(entity);
		auto& fractionParticle = view.get<FractionParticleComponent>(entity);
		if (fractionParticle.Life >= fractionParticle.LifeTime)
		{
			entitiesToDestroy.push_back(entity);
		}

		fractionParticle.YVelocity += fractionParticle.Gravity * Time::DeltaTime;
		fractionParticle.Life += Time::DeltaTime;

		transform.Position.y += fractionParticle.YVelocity * Time::DeltaTime;
		transform.Position.x += fractionParticle.XVelocity * Time::DeltaTime;
	}

	for (auto entt : entitiesToDestroy)
	{
		registry.destroy(entt);
	}

	entitiesToDestroy.clear();
}
