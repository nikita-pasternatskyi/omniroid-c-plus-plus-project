#pragma once
#include "entt/entt.hpp"

class ISystem
{
public:
	virtual void Update(entt::registry& registry) = 0;
};