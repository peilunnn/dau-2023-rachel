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
#include "App/Utilities/include/app.h"
#include "App/Managers/include/EntityManager.h"
#include "App/Systems/include/HandleInput.h"
#include "App/Systems/include/HandleMovement.h"
#include "App/Systems/include/RenderEntities.h"
#include "App/Systems/include/HandleAnimation.h"
//------------------------------------------------------------------------

EntityManager entityManager;
Entity playerEntityId;
HandleInput handleInput;
HandleMovement handleMovement;
RenderEntities renderEntities;
CSimpleSprite *playerSprite;
HandleAnimation handleAnimation;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	playerSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
	playerSprite->SetPosition(400.0f, 400.0f);
	playerSprite->SetScale(1.0f);

	playerEntityId = entityManager.CreatePlayerEntity(playerSprite);
	handleAnimation.Init(playerSprite);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	playerSprite->Update(deltaTime);
	handleInput.Update(entityManager, deltaTime, playerEntityId);
	handleMovement.Update(entityManager, deltaTime);
	handleAnimation.Update(entityManager, deltaTime);
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
	delete playerSprite;
}