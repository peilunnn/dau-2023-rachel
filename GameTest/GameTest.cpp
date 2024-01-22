//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SystemManager.h"
#include "Managers/include/SpriteManager.h"
#include "Managers/include/SoundManager.h"
#include "Systems/include/AnimationHandler.h"
#include "ParticleSystem/include/ParticleManager.h"
#include "ParticleSystem/include/ParticleRenderer.h"
#include "Systems/include/RenderingHandler.h"
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{	
	// Set up entities
	EntityManager &entityManager = EntityManager::GetInstance();
	entityManager.Init();

	// Set up managers and systems
	SystemManager::GetInstance().Init();

	// Set up animations
	AnimationHandler::GetInstance().Init();

	// Set up particle emitters
	ParticleManager::GetInstance().Init();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	GameManager& gameManager = GameManager::GetInstance();
	float deltaTimeInSeconds = deltaTime / 1000.0f;

	gameManager.Update(deltaTimeInSeconds);

	SoundManager::GetInstance().Cleanup();
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	RenderingHandler::GetInstance().Render();
	ParticleRenderer::GetInstance().RenderParticles(ParticleManager::GetInstance());
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	SpriteManager::GetInstance().DestroyAllSprites();
}