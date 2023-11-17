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
#include "App/Systems/include/RenderEntities.h"
#include "App/Systems/include/HandleAnimation.h"
//------------------------------------------------------------------------

EntityManager entityManager;
Entity playerEntityId;
HandleInput handleInput;
UpdateMovement updateMovement;
RenderEntities renderEntities;
CSimpleSprite *playerSprite;
HandleAnimation handleAnimation;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	playerSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	playerSprite->SetPosition(400.0f, 400.0f);
	playerSprite->SetScale(1.0f);
	handleAnimation.Init(playerSprite);

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
	auto playerAnimation= std::make_shared<Animation>();

	// Add components
	entityManager.AddComponent(playerEntityId, playerTransform);
	entityManager.AddComponent(playerEntityId, playerRenderable);
	entityManager.AddComponent(playerEntityId, playerCollider);
	entityManager.AddComponent(playerEntityId, playerVelocity);
	entityManager.AddComponent(playerEntityId, playerDirection);
	entityManager.AddComponent(playerEntityId, playerHealth);
	entityManager.AddComponent(playerEntityId, playerAnimation);
	//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	playerSprite->Update(deltaTime);

	handleInput.Update(entityManager, deltaTime, playerEntityId);
	updateMovement.Update(entityManager, deltaTime);
	handleAnimation.Update(entityManager, deltaTime);

	char debugMsg[256];
	auto animation = entityManager.GetComponent<Animation>(playerEntityId);
	if (animation) {
		sprintf_s(debugMsg, "Current Animation: %d\n", animation->currentAnimation);
		OutputDebugStringA(debugMsg);
	};
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	renderEntities.Render(entityManager);
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