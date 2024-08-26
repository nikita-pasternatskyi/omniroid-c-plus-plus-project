#pragma once
#include "../src/gameWorld/systems/ISystem.h"
#include "../../input/Input.h"
#include "glm/glm.hpp"
#include "../../audio/SoundEffect.h"
#include "../../rendering/materials/DefaultLitMaterial.h"


#define MOVE_LEFT_BUTTON "move_left"
#define MOVE_RIGHT_BUTTON "move_right"
#define JUMP_BUTTON "jump"
#define BASH_BUTTON "bash"
#define SLIDE_BUTTON "slide"

class Level;
class PlayerComponent;
class PlayerCollider;
class Demo;
class TransformComponent;

class PlayerSystem : public ISystem
{
public:
	PlayerSystem(Input& input, Audio& audio, entt::entity camera, Demo* level);
	~PlayerSystem();
	virtual void Update(entt::registry& registry) override;

private:
	float _countDownTimer = 0;
	float _countDownTime = 30.0f;
	bool _countDownBegun = false;

	SoundEffect _normalTrack;
	SoundEffect _chaseTrack;
	SoundEffect _monsterHitSound;
	SoundEffect _wallHitSound;
	SoundEffect _glassBreakSound;
	SoundEffect _jumpSound;
	SoundEffect _crateBreakSound;
	SoundEffect _footStep;

	float _shakePower = 0;
	float Random(glm::vec2 co);
	glm::vec3 Lerp(glm::vec3 x, glm::vec3 y, float t);
	Demo* _level;
	glm::vec2 _cutsceneTriggerPosition = { 149.102, -1500 };
	glm::vec3 _cutsceneMonsterPosition = { -252, -1509, 3 };
	bool _cutscenePlayed = false;
	float RandomFloat(float max);
	void AlignPlayerToInput(float moveInput, PlayerComponent& player, TransformComponent& transform);
	bool CheckCollision(entt::registry& registry, const glm::vec3& position, PlayerCollider& collider, entt::entity* collidedEntity);
	std::vector<entt::entity> CollectCrateEntities(entt::registry& registry, const glm::vec3& position, PlayerCollider& collider);
	bool CollidedWithMonsterCapsule(entt::registry& registry, const glm::vec3& position, PlayerCollider& collider, entt::entity* monster);
	entt::entity _camera;
	entt::entity _monster;
	entt::entity _countDownTimerVisual;
	DefaultLitMaterial* _fractionMaterial;
	DefaultLitMaterial* _glassFractionMaterial;
	Input& _input;
};