#include "PlayerSystem.h"
#include "PlayerComponent.h"
#include "../../gameWorld/systems/animatedSprite/AnimatedSpriteRendererComponent.h"
#include "../../gameWorld/systems/TransformComponent.h"
#include "../../time/Time.h"
#include "../../gameWorld/systems/collisions/RectCollider.h"
#include "../CrateComponent.h"
#include "../../gameWorld/systems/SpriteRendererComponent.h"
#include "../FractionParticleComponent.h"
#include "../monster/MonsterCapsuleComponent.h"
#include "../../gameWorld/systems/LightComponent.h"
#include "../src/rendering/materials/SimpleMaterial.h"
#include "../Demo.h"

PlayerSystem::PlayerSystem(Input& input, Audio& audio, entt::entity camera, Demo* level)
	: _input(input), _footStep(audio, "event:/Player/player_footstep", 1.0), _camera(camera), _level(level),
	_normalTrack(audio, "event:/Music/In-Game Normal", 0.0),
	_chaseTrack(audio, "event:/Music/In-Game Chase", 0),
	_monsterHitSound(audio, "event:/Player/player_monster_hit", 1),
	_wallHitSound(audio, "event:/Player/player_wall_hit", 1),
	_glassBreakSound(audio, "event:/Player/player_glass_break", 1),
	_jumpSound(audio, "event:/Player/player_jump", 1),
	_crateBreakSound(audio, "event:/crate_break", 1)

{
	_normalTrack.Play();
	std::srand(Time::TimeSinceStartup);
	_fractionMaterial = new DefaultLitMaterial(
		"assets/level/crates/crate_piece_albedo.png",
		"assets/level/crates/crate_piece_normal.png",
		"assets/level/crates/crate_piece_params.png",
		glm::vec4(1.0), 1.0);
	_glassFractionMaterial = new DefaultLitMaterial(
		"assets/level/glass_shards.png",
		"assets/level/glass_shards.png",
		"assets/level/glass_shards.png",
		glm::vec4(1.0), 1.0);
	FMOD_REVERB_PROPERTIES properties = FMOD_PRESET_HALLWAY;
	audio.SetReverbProperties(&properties);
}

PlayerSystem::~PlayerSystem()
{
	_normalTrack.Stop();
	_chaseTrack.Stop();
}

void PlayerSystem::Update(entt::registry& registry)
{
	auto playerView = registry.view<PlayerComponent, TransformComponent, AnimatedSpriteRendererComponent>();
	
	float movementInput = 0;
	if (_input.GetButtonState(MOVE_LEFT_BUTTON)->Pressed)
	{
		movementInput = -1;
	}
	else if (_input.GetButtonState(MOVE_RIGHT_BUTTON)->Pressed)
	{
		movementInput = 1;
	}
	else if (_input.GetButtonState(MOVE_LEFT_BUTTON)->Pressed == _input.GetButtonState(MOVE_RIGHT_BUTTON)->Pressed)
	{
		movementInput = 0;
	}
	
	Button* slide = _input.GetButtonState(SLIDE_BUTTON);
	Button* bash = _input.GetButtonState(BASH_BUTTON);
	Button* jump = _input.GetButtonState(JUMP_BUTTON);

	for (auto entity : playerView)
	{
		auto& player = playerView.get<PlayerComponent>(entity);
		auto& transform = playerView.get<TransformComponent>(entity);
		auto& sprite = playerView.get<AnimatedSpriteRendererComponent>(entity);
		auto& camera = registry.get<TransformComponent>(_camera);

		if (_countDownBegun)
		{
			auto& timerTransform = registry.get<TransformComponent>(_countDownTimerVisual);
			timerTransform.Position.x = -camera.Position.x + 400;
			timerTransform.Position.y = -camera.Position.y + 200;
			_countDownTimer += Time::DeltaTime;
			if (_countDownTimer >= _countDownTime)
			{
				if (_level->Status == LevelCompletionStatus::NONE)
				{
					_normalTrack.Stop();
					_chaseTrack.Stop();
					_level->Status = LevelCompletionStatus::LOST;
				}
				return;
			}
			if (transform.Position.x <= -130 && transform.Position.y >= -80)
			{
				_normalTrack.Stop();
				_chaseTrack.Stop();
				_level->Status = LevelCompletionStatus::WON;
			}
		}

		_footStep.ChangePosition(transform.Position);


		float noiseX = Random(glm::vec2(Time::TimeSinceStartup, Time::TimeSinceStartup));
		float noiseY = Random(glm::vec2(Time::TimeSinceStartup * 2, Time::TimeSinceStartup * 2));

		camera.Position.x = -std::clamp(transform.Position.x, 0.0f, 1440.0f + 480.0f) + noiseX * _shakePower;
		camera.Position.y = -std::clamp(transform.Position.y + 100, -930.0f - 270.0f - 270.0f, 0.0f) + noiseY * _shakePower;

		PlayerState newState = player.CurrentState;
		std::vector<entt::entity> collidedCrates;
		entt::entity monsterCapsule;
		
		AnimatedSpriteRendererComponent* monsterSprite;
		//execute current state, change x position and etc.
		switch (player.CurrentState)
		{
			case PlayerState::IDLE:

				if (!player.Grounded)
				{
					newState = PlayerState::FALL;
				}
				if (movementInput != 0)
				{
					newState = PlayerState::MOVE;
				}
				if (jump->JustPressed)
				{
					newState = PlayerState::JUMP;
					player.yVelocity = player.JumpHeight;
					//transform.Position.y += 0.3;
				}
				player.xVelocity = movementInput * (player.MoveSpeed * Time::DeltaTime);
				break;

			case PlayerState::MOVE:
				if (!player.Grounded)
				{
					newState = PlayerState::FALL;
				}
				player.xVelocity = movementInput * (player.MoveSpeed * Time::DeltaTime);
				AlignPlayerToInput(movementInput, player, transform);
				if (movementInput == 0)
				{
					newState = PlayerState::IDLE;
				}
				if (jump->JustPressed)
				{
					newState = PlayerState::JUMP;
					player.yVelocity = player.JumpHeight;
				}
				if (bash->JustPressed)
				{
					newState = PlayerState::BASH;
					player.yVelocity = player.BashJumpHeight;
				}
				if (slide->JustPressed)
				{
					newState = PlayerState::SLIDE;
				}
				break;

			case PlayerState::JUMP:
				if (player.yVelocity < 0)
				{
					newState = PlayerState::FALL;
				}
				player.xVelocity = movementInput * (player.MoveSpeed * Time::DeltaTime);
				AlignPlayerToInput(movementInput, player, transform);
				break;

			case PlayerState::FALL:
				if (player.Grounded)
				{
					newState = PlayerState::MOVE;
				}
				glm::vec3 position = transform.Position;
				position.x += player.Facing * player.WallSlideProximity;
				if (CheckCollision(registry, position, player.NormalCollider, nullptr) && movementInput == player.Facing)
				{
					position.x = transform.Position.x;
					position.y -= player.WallSlideMinHeight;
					if(!CheckCollision(registry, position, player.NormalCollider, nullptr))
					{
						newState = PlayerState::WALL_SLIDE;
						position.x += player.Facing * player.WallSlideProximity;
					}
				}
				player.xVelocity = movementInput * (player.MoveSpeed * Time::DeltaTime);
				AlignPlayerToInput(movementInput, player, transform);
				break;

			case PlayerState::SLIDE:
				player.SlideTimer += Time::DeltaTime;
				if (player.SlideTimer >= player.SlideTime)
				{
					glm::vec3 pos = transform.Position;
					pos.y += player.SlideCeilingProximity;
					if (!CheckCollision(registry, pos, player.NormalCollider, nullptr))
					{
						newState = PlayerState::MOVE;
						player.SlideTimer = 0;
					}
				}
				if (!player.Grounded)
				{
					newState = PlayerState::FALL;
				}
				player.xVelocity = player.Facing * (player.SlideSpeed * Time::DeltaTime);
				break;

			case PlayerState::BASH:
				player.BashTimer += Time::DeltaTime;
				if (player.BashTimer >= player.BashTime)
				{
					newState = PlayerState::MOVE;
					player.BashTimer = 0.0f;
				}
				player.xVelocity = player.Facing * (player.BashSpeed * Time::DeltaTime);

				collidedCrates = CollectCrateEntities(registry, transform.Position, player.BashDestroyCollider);

				for (auto ent : collidedCrates)
				{
					auto& transform = registry.get<TransformComponent>(ent);
					glm::vec3 position = transform.Position;
					_crateBreakSound.Play();
					for (int i = 1; i != 8; i++)
					{
						auto particle = registry.create();
						registry.emplace<TransformComponent>(particle, position, glm::vec2(10), RandomFloat(90));
						registry.emplace<SpriteRendererComponent>(particle, _fractionMaterial);
						float sign = i % 2 == 0 ? 1 : -1;
						float force = RandomFloat(600) * player.Facing;
						registry.emplace<FractionParticleComponent>(particle, force, force * sign);
					}
					registry.destroy(ent);
				}

				if (CollidedWithMonsterCapsule(registry, transform.Position, player.BashDestroyCollider, &monsterCapsule))
				{
					_glassBreakSound.Play();
					auto& transform = registry.get<TransformComponent>(monsterCapsule);
					auto& animatedCapsule = registry.get<AnimatedSpriteRendererComponent>(monsterCapsule);
					animatedCapsule.ChangeAnimation("Broken");
					glm::vec3 position = transform.Position;
					for (int i = 1; i != 12; i++)
					{
						auto particle = registry.create();

						registry.emplace<TransformComponent>(particle, position, glm::vec2(10), RandomFloat(90));
						registry.emplace<SpriteRendererComponent>(particle, _glassFractionMaterial);
						float sign = i % 2 == 0 ? 1 : -1;
						float force = RandomFloat(600) * -player.Facing;
						registry.emplace<FractionParticleComponent>(particle, force, force * sign);
					}
					player.xVelocity = -player.xVelocity;
					registry.remove<RectCollider>(monsterCapsule);
					newState = PlayerState::FINAL_CUTSCENE;
					_monster = _level->CreateMonster(1, _cutsceneMonsterPosition.x, _cutsceneMonsterPosition.y, "Cutscene");
				}

				break;
			case PlayerState::WALL_SLIDE:
				position = transform.Position;
				position.y -= player.WallSlideMinHeight;
				if (CheckCollision(registry, position, player.NormalCollider, nullptr))
				{
					newState = PlayerState::WALL_JUMP;
					player.yVelocity = player.JumpHeight;
				}
				position.x += player.Facing * player.WallSlideProximity;
				position.y = transform.Position.y;
				if (!CheckCollision(registry, position, player.NormalCollider, nullptr))
				{
					newState = PlayerState::FALL;
				}
				player.yVelocity = player.WallSlideGravity;

				if (jump->JustPressed)
				{
					newState = PlayerState::WALL_JUMP;
					player.yVelocity = player.JumpHeight;
				}
				break;
			case PlayerState::WALL_JUMP:
				if (player.yVelocity < 0)
				{
					newState = PlayerState::FALL;
					player.WallJumpPushTimer = 0;
				}

				player.WallJumpPushTimer += Time::DeltaTime;

				if (player.WallJumpPushTimer >= player.WallJumpPushTime)
				{
					player.xVelocity = movementInput * (player.MoveSpeed * Time::DeltaTime);
					AlignPlayerToInput(movementInput, player, transform);
				}
				else
				{
					player.xVelocity = -player.Facing * (player.WallSlidePushAwayForce * Time::DeltaTime);
				}
				break;
			case PlayerState::FINAL_CUTSCENE:
			{
				auto& monsterTransform = registry.get<TransformComponent>(_monster);
				player.CutsceneTimer += Time::DeltaTime;
				if (player.CutsceneTimer >= player.CutsceneMonsterMoveDelay)
				{
					float t = (player.CutsceneTimer - player.CutsceneMonsterMoveDelay) / (player.CutsceneAttackTime - player.CutsceneMonsterMoveDelay);
					monsterTransform.Position = Lerp(
						glm::vec3(_cutsceneMonsterPosition.x, _cutsceneMonsterPosition.y, 3),
						transform.Position, t);
				}
				if (player.CutsceneTimer >= player.CutsceneAttackTime)
				{
					_monsterHitSound.Play();
					newState = PlayerState::CUTSCENE_HURT_FLY;
					player.xVelocity = 50;
					player.CutsceneTimer = 0;
					sprite.ChangeAnimation("Cutscene_Hurt");
					break;
				}
				if (player.Grounded)
				{
					sprite.ChangeAnimation("Idle");
					player.xVelocity = 0;
				}
				break;
			}
			case PlayerState::CUTSCENE_HURT_FLY:
				glm::vec3 curPosition = transform.Position;
				curPosition.x += 15;
				if (CheckCollision(registry, curPosition, player.NormalCollider, nullptr))
				{
					if(player.CutsceneGetupTimer == 0)
					{
						_wallHitSound.Play();
					}
					sprite.ChangeAnimation("Cutscene_Recovery");
					if (player.CutsceneGetupTimer >= player.CutsceneGetUpTime)
					{
						player.BashTimer = 0;
						newState = PlayerState::IDLE;
						_normalTrack.Stop();
						_chaseTrack.Play();
						player.xVelocity = 0;
						player.yVelocity = 0;
						auto lightView = registry.view<LightComponent>();
						for (auto entity : lightView)
						{
							auto& lightComponent = lightView.get<LightComponent>(entity);
							lightComponent.Color = glm::vec3(1.0, 0, 0);
						}
						_shakePower = 10;
						_countDownBegun = true;
						_countDownTimerVisual = registry.create();
						auto mat = new DefaultLitMaterial("assets/ui/level/countdown.png", "", "assets/level/params/City.png", glm::vec4(1), 1.0);
						auto& animator = registry.emplace<AnimatedSpriteRendererComponent>(_countDownTimerVisual, "assets/ui/level/countdown.json");
						animator.ChangeAnimation("countdown");
						auto& spr = registry.emplace<SpriteRendererComponent>(_countDownTimerVisual, mat);
						auto scale = glm::vec2(32);
						registry.emplace<TransformComponent>(_countDownTimerVisual, glm::vec3(transform.Position.x, transform.Position.y + 80, 4), scale, 0.0);
					}
					player.CutsceneGetupTimer += Time::DeltaTime;
				}
				break;
				
		}

		//physics

		if (player.CurrentState != PlayerState::IDLE)
		{
			player.yVelocity += player.Gravity * Time::DeltaTime;

			player.yVelocity = std::max(player.yVelocity, player.MinYVelocity);

			float oldX = transform.Position.x;
			float oldY = transform.Position.y;

			float xMove = player.xVelocity;
			float yMove = player.yVelocity * Time::DeltaTime;

			float substeps = 10.0f;

			float xDelta = xMove / substeps;
			float yDelta = yMove / substeps;

			float newX = oldX;
			float newY = oldY;

			PlayerCollider collider = player.CurrentState == PlayerState::SLIDE ? player.SlideCollider : player.NormalCollider;

			for (int i = 0; i < substeps; i++)
			{
				newX += xDelta;
				if (CheckCollision(registry, glm::vec3(newX, oldY, 0), collider, nullptr))
				{
					if (player.CurrentState != PlayerState::SLIDE)
					{
						newX -= xDelta;
					}
					else
					{
						if (CheckCollision(registry, glm::vec3(newX, oldY + 10, 0), collider, nullptr))
						{
							newX -= xDelta;
						}
					}
				}

				newY += yDelta;
				if (CheckCollision(registry, glm::vec3(newX, newY, 0), collider, nullptr))
				{
					newY -= yDelta;
					player.Grounded = true;
				}
				else
				{
					player.Grounded = false;
				}
			}

			transform.Position.x = newX;
			transform.Position.y = newY;
		}

		if (player.CurrentState != PlayerState::BASH && 
			transform.Position.x <= _cutsceneTriggerPosition.x && transform.Position.y <= _cutsceneTriggerPosition.y
			&& _cutscenePlayed == false)
		{
			AlignPlayerToInput(-1, player, transform);
			newState = PlayerState::BASH;
			player.yVelocity = player.JumpHeight * 0.75;
			_cutscenePlayed = true;
		}

		if (newState == player.CurrentState)
		{
			continue;
		}
		
		if (player.yVelocity < 0)
		{
			player.yVelocity = -1;
		}
		
		if (player.CurrentState == PlayerState::FALL && newState != PlayerState::WALL_SLIDE)
		{
			_jumpSound.Play();
		}
		//set new state
		switch (newState)
		{
		case PlayerState::IDLE:
			sprite.ChangeAnimation("Idle");
			_footStep.Stop();
			break;
		case PlayerState::MOVE:
			sprite.ChangeAnimation("Run");
			_footStep.Play();
			break;
		case PlayerState::JUMP:
			sprite.ChangeAnimation("Jump");
			_footStep.Stop();
			break;
		case PlayerState::FALL:
			sprite.ChangeAnimation("Fall");
			_footStep.Stop();
			break;
		case PlayerState::SLIDE:
			sprite.ChangeAnimation("Slide");
			_footStep.Stop();
			break;
		case PlayerState::BASH:
			sprite.ChangeAnimation("Bash");
			_footStep.Stop();
			break;
		case PlayerState::WALL_SLIDE:
			sprite.ChangeAnimation("WallSlide");
			_footStep.Stop();
			break;
		case PlayerState::WALL_JUMP:
			_jumpSound.Play();
			sprite.ChangeAnimation("WallJump");
			_footStep.Stop();
			break;
		}
		player.CurrentState = newState;
	}
}

float PlayerSystem::Random(glm::vec2 co)
{
	return glm::fract(sin(glm::dot(co, glm::vec2(12.9898, 78.233))) * 43758.5453);
}

glm::vec3 PlayerSystem::Lerp(glm::vec3 x, glm::vec3 y, float t)
{
	return x * (1.f - t) + y * t;
}

//https://stackoverflow.com/questions/686353/random-float-number-generation
float PlayerSystem::RandomFloat(float max)
{
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
}

void PlayerSystem::AlignPlayerToInput(float moveInput, PlayerComponent& player, TransformComponent& transform)
{
	if (moveInput == 0)
		return;
	if (moveInput == 1)
	{
		player.Facing = 1;
		transform.Scale.x = std::abs(transform.Scale.x);
	}
	else if (moveInput == -1)
	{
		player.Facing = -1;
		transform.Scale.x = -std::abs(transform.Scale.x);
	}
}

bool PlayerSystem::CheckCollision(entt::registry& registry, const glm::vec3& position, PlayerCollider& collider, entt::entity* collidedEntity)
{
	auto collisionView = registry.view<RectCollider, TransformComponent>(entt::exclude<PlayerComponent>);
	float left = position.x + collider.ColliderXOffset - collider.ColliderWidth;
	float right = position.x + collider.ColliderXOffset + collider.ColliderWidth;
	float top = position.y + collider.CollderYOffset + collider.ColliderHeight;
	float bottom = position.y + collider.CollderYOffset - collider.ColliderHeight;

	for (auto entity : collisionView)
	{
		auto& rectTransform = collisionView.get<TransformComponent>(entity);
		glm::vec2 topLeft = rectTransform.TopLeft();
		glm::vec2 topRight = rectTransform.TopRight();
		glm::vec2 bottomLeft = rectTransform.BottomLeft();
		glm::vec2 bottomRight = rectTransform.BottomRight();

		bool collisionRight = right > topLeft.x;
		bool collisionLeft = left < topRight.x;
		bool collisionBottom = bottom < topLeft.y;
		bool collisionTop = top > bottomLeft.y;
		if (collisionRight && collisionLeft && collisionBottom && collisionTop)
		{
			collidedEntity = &entity;
			return true;
		}
	}
	return false;
}

std::vector<entt::entity> PlayerSystem::CollectCrateEntities(entt::registry& registry, const glm::vec3& position, PlayerCollider& collider)
{
	std::vector<entt::entity> result;
	auto collisionView = registry.view<RectCollider, CrateComponent, TransformComponent>(entt::exclude<PlayerComponent>);
	float left = position.x + collider.ColliderXOffset - collider.ColliderWidth;
	float right = position.x + collider.ColliderXOffset + collider.ColliderWidth;
	float top = position.y + collider.CollderYOffset + collider.ColliderHeight;
	float bottom = position.y + collider.CollderYOffset - collider.ColliderHeight;

	for (auto entity : collisionView)
	{
		auto& rectTransform = collisionView.get<TransformComponent>(entity);
		glm::vec2 topLeft = rectTransform.TopLeft();
		glm::vec2 topRight = rectTransform.TopRight();
		glm::vec2 bottomLeft = rectTransform.BottomLeft();
		glm::vec2 bottomRight = rectTransform.BottomRight();

		bool collisionRight = right > topLeft.x;
		bool collisionLeft = left < topRight.x;
		bool collisionBottom = bottom < topLeft.y;
		bool collisionTop = top > bottomLeft.y;
		if (collisionRight && collisionLeft && collisionBottom && collisionTop)
		{
			result.push_back(entity);
		}
	}
	return result;
}

bool PlayerSystem::CollidedWithMonsterCapsule(entt::registry& registry, const glm::vec3& position, PlayerCollider& collider, entt::entity* monster)
{
	auto collisionView = registry.view<RectCollider, TransformComponent, MonsterCapsuleComponent>(entt::exclude<PlayerComponent>);
	float left = position.x + collider.ColliderXOffset - collider.ColliderWidth;
	float right = position.x + collider.ColliderXOffset + collider.ColliderWidth;
	float top = position.y + collider.CollderYOffset + collider.ColliderHeight;
	float bottom = position.y + collider.CollderYOffset - collider.ColliderHeight;

	for (auto entity : collisionView)
	{
		auto& rectTransform = collisionView.get<TransformComponent>(entity);
		glm::vec2 topLeft = rectTransform.TopLeft();
		glm::vec2 topRight = rectTransform.TopRight();
		glm::vec2 bottomLeft = rectTransform.BottomLeft();
		glm::vec2 bottomRight = rectTransform.BottomRight();

		bool collisionRight = right > topLeft.x;
		bool collisionLeft = left < topRight.x;
		bool collisionBottom = bottom < topLeft.y;
		bool collisionTop = top > bottomLeft.y;
		if (collisionRight && collisionLeft && collisionBottom && collisionTop)
		{
			*monster = entity;
			return true;
		}
	}
	return false;
}
