#pragma once
#include "PlayerState.h"
#include "PlayerCollider.h"

struct PlayerComponent
{
	PlayerState CurrentState;

	bool Grounded;
	PlayerCollider NormalCollider			{ 0, -10, 22, 60 };
	PlayerCollider SlideCollider			{ 0, -65, 22, 11 };
	PlayerCollider BashDestroyCollider		{ 22, 0,  65, 65 };

	float Facing = 1;

	const float WallSlideProximity = 5;
	const float WallSlideMinHeight = 5;
	const float WallSlidePushAwayForce = 300.0f;
	const float WallSlideGravity = -60.0f;
	const float WallJumpPushTime = 0.15f;
	float WallJumpPushTimer = 0.0f;

	const float Gravity = -1800.8f;

	const float MoveSpeed = 300.0f;
	const float JumpHeight = 655.0f;

	float BashTimer = 0.0f;
	const float BashTime = 0.4f;
	const float BashJumpHeight = JumpHeight * 0.5f;
	const float BashSpeed = 800;
	const float BashForce = 10.0f;
	
	const float SlideCeilingProximity = 1;
	const float SlideTime = 0.25f;
	const float SlideSpeed = MoveSpeed * 2.0f;
	float SlideTimer = 0.0f;

	float CutsceneTimer = 0.0;
	float CutsceneMonsterMoveDelay = 1.0f;
	float CutsceneAttackTime = 1.25f;

	float CutsceneGetUpTime = 2.5f;
	float CutsceneGetupTimer = 0.0f;

	const float MinYVelocity = -700;
	float yVelocity = 0;
	float xVelocity = 0;

};

