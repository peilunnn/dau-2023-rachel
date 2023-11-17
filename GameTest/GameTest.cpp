//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include <memory>
#include <iostream>
//------------------------------------------------------------------------
#include "App/Application/include/app.h"
#include "App/Components/include/Transform.h"
#include "App/Components/include/Collider.h"
#include "App/Components/include/Renderable.h"
#include "App/Components/include/Velocity.h"
#include "App/Components/include/Direction.h"
#include "App/Components/include/Health.h"
#include "App/Entities/include/EntityManager.h"
#include "App/Systems/include/HandleInput.h"
#include "App/Systems/include/UpdateMovement.h"
//------------------------------------------------------------------------

const float THUMB_STICK_THRESHOLD = 0.5f;
const float POSITION_DELTA = 1.0f;
int lastNonIdleAnimState = 6;

EntityManager entityManager;
Entity playerEntityId;
HandleInput handleInput;
UpdateMovement updateMovement;
CSimpleSprite *playerSprite;

enum
{
	ANIM_FORWARDS,
	ANIM_BACKWARDS,
	ANIM_LEFT,
	ANIM_RIGHT,
	ANIM_IDLE_LEFT,
	ANIM_IDLE_RIGHT,
	ANIM_IDLE_FORWARDS,
	ANIM_IDLE_BACKWARDS,
};

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	playerSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	playerSprite->SetPosition(400.0f, 400.0f);
	float speed = 1.0f / 15.0f;
	playerSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
	playerSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
	playerSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
	playerSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
	playerSprite->CreateAnimation(ANIM_IDLE_BACKWARDS, speed, { 0 });
	playerSprite->CreateAnimation(ANIM_IDLE_LEFT, speed, { 8 });
	playerSprite->CreateAnimation(ANIM_IDLE_RIGHT, speed, { 16 });
	playerSprite->CreateAnimation(ANIM_IDLE_FORWARDS, speed, { 24 });
	playerSprite->SetScale(1.0f);
	
	// ECS Initialization
	playerEntityId = entityManager.CreateEntity();
	
	// Create components
	auto playerTransform = std::make_shared<Transform>(glm::vec3(400.0f, 400.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	auto playerRenderable = std::make_shared<Renderable>(playerSprite);

	auto playerCollider = std::make_shared<Collider>();
	playerCollider->collisionShape = CollisionShape::SPHERE;
	playerCollider->collisionMask = 1;

	auto playerVelocity = std::make_shared<Velocity>(0.0f, 0.0f);

	auto playerDirection = std::make_shared<Direction>();
	playerDirection->direction = glm::vec2(0.0f, -1.0f);

	auto playerHealth = std::make_shared<Health>();

	// Add components
	entityManager.AddComponent(playerEntityId, playerTransform);
	entityManager.AddComponent(playerEntityId, playerRenderable);
	entityManager.AddComponent(playerEntityId, playerCollider);
	entityManager.AddComponent(playerEntityId, playerVelocity);
	entityManager.AddComponent(playerEntityId, playerDirection);
	entityManager.AddComponent(playerEntityId, playerHealth);
	//------------------------------------------------------------------------
}

void UpdatePositionAndAnimation()
{
	float x, y;
	playerSprite->GetPosition(x, y);
	float thumbStickX = App::GetController().GetLeftThumbStickX();
	float thumbStickY = App::GetController().GetLeftThumbStickY();
	int animationState = -1;

	if (thumbStickX > THUMB_STICK_THRESHOLD) {
		animationState = ANIM_RIGHT;
		x += POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_RIGHT;
	}
	else if (thumbStickX < -THUMB_STICK_THRESHOLD) {
		animationState = ANIM_LEFT;
		x -= POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_LEFT;
	}

	if (thumbStickY > THUMB_STICK_THRESHOLD) {
		animationState = ANIM_BACKWARDS;
		y -= POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_BACKWARDS;
	}
	else if (thumbStickY < -THUMB_STICK_THRESHOLD) {
		animationState = ANIM_FORWARDS;
		y += POSITION_DELTA;
		lastNonIdleAnimState = ANIM_IDLE_FORWARDS;
	}

	if (fabs(thumbStickX) <= THUMB_STICK_THRESHOLD && fabs(thumbStickY) <= THUMB_STICK_THRESHOLD) {
		animationState = lastNonIdleAnimState;
	}

	playerSprite->SetAnimation(animationState);
	playerSprite->SetPosition(x, y);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	char debugMsg[256];

	playerSprite->Update(deltaTime);

	//UpdatePositionAndAnimation();

	handleInput.Update(entityManager, deltaTime, playerEntityId);
	updateMovement.Update(entityManager, deltaTime);
	auto transform = entityManager.GetComponent<Transform>(playerEntityId);
	if (transform) {
		sprintf_s(debugMsg, "Current Transform: X: %f, Y: %f\n", transform->position.x, transform->position.y);
		OutputDebugStringA(debugMsg);
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	auto transform = entityManager.GetComponent<Transform>(playerEntityId);
	auto renderable = entityManager.GetComponent<Renderable>(playerEntityId);

	if (transform && renderable && renderable->sprite) {
		renderable->sprite->SetPosition(transform->position.x, transform->position.y);
		renderable->sprite->SetAngle(transform->rotation.z);
		renderable->sprite->SetScale(transform->scale.x);
		renderable->sprite->Draw();
	}
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	//------------------------------------------------------------------------
	// Example Sprite Code....
	delete playerSprite;
	//------------------------------------------------------------------------
}