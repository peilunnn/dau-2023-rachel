#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/Collider.h"
#include "Components/include/Cooldown.h"
#include "Components/include/EntityId.h"
#include "Components/include/Health.h"
#include "Components/include/Renderable.h"
#include "Components/include/Score.h"
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Components/include/Timer.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/Helper.h"
using glm::vec2;
using glm::vec3;

EntityManager& EntityManager::GetInstance()
{
	static EntityManager instance;
	return instance;
}

void EntityManager::Init()
{
	Screen &screen = screen.GetInstance();
	ShootingHandler &shootingHandler = ShootingHandler::GetInstance();
	SpriteManager &spriteManager = SpriteManager::GetInstance();

	const float screenWidth = screen.SCREEN_WIDTH;
	const float screenHeight = screen.SCREEN_HEIGHT;
	const int ammoSpriteSpacing = screen.AMMO_SPRITE_SPACING;
	const float ammoXOffset = 20.0f;
	const float ammoYOffset = 720.0f;
	const float healthBarXOffset = 880.0f;
	const float healthBarYOffset = 720.0f;
	const float ammoStartingX = screenWidth - ammoXOffset;
	const float ammoYPos = screenHeight - ammoYOffset;
	const float healthBarXPos = screenWidth - healthBarXOffset;
	const float healthBarYPos = screenHeight - healthBarYOffset;

	m_playerEntityId = CreatePlayerEntity(spriteManager);
	m_enemyEntityId = CreateEnemyEntity(spriteManager);
	m_reloadingCircleEntityId = CreateReloadingCircleEntity(spriteManager);
	m_healthBarEntityId = CreateHealthBarEntity(spriteManager, healthBarXPos, healthBarYPos);

	for (int i = 0; i < shootingHandler.MAX_BULLETS; ++i)
	{
		const float xPos = ammoStartingX - i * ammoSpriteSpacing;

		m_ammoEmptyEntityId = CreateAmmoEntity(spriteManager, EntityType::AmmoEmpty, xPos, ammoYPos);
		m_ammoEmptyEntityIds.push_back(m_ammoEmptyEntityId);
		m_ammoFilledEntityId = CreateAmmoEntity(spriteManager, EntityType::AmmoFilled, xPos, ammoYPos);
		m_ammoFilledEntityIds.push_back(m_ammoFilledEntityId);
	}

	m_scoreEntityId = CreateScoreEntity();
	m_countdownTimerEntityId = CreateCountdownTimerEntity();
	m_starfieldEntityId = CreateStarfieldEntity(spriteManager);
	m_titleEntityId = CreateTitleEntity(spriteManager);
	m_playButtonEntityId = CreatePlayButtonEntity(spriteManager);
	m_backButtonEntityId = CreateBackButtonEntity(spriteManager);
	m_quitButtonEntityId = CreateQuitButtonEntity(spriteManager);
	m_loadingScreenCharacterEntityId = CreateLoadingScreenCharacterEntity(spriteManager);
}

vector<EntityId> EntityManager::GetAllEntities()
{
	vector<EntityId> allEntities;
	for (const auto &pair : m_entityComponents)
		allEntities.push_back(pair.first);
	return allEntities;
}

EntityId EntityManager::CreateEntityId()
{
	return m_nextEntityId++;
}

EntityId EntityManager::CreatePlayerEntity(SpriteManager &spriteManager)
{
	EntityId playerEntityId = CreateEntityId();
	CSimpleSprite *playerSprite = spriteManager.CreateSprite(playerEntityId, Helper::PATH_TO_PLAYER, 4, 4);

	Screen &screen = screen.GetInstance();
	const float xPos = Helper::GenerateFloat(screen.BORDER_LEFT_SCREEN_COORD, screen.BORDER_RIGHT_SCREEN_COORD);
	const float yPos = Helper::GenerateFloat(screen.BORDER_TOP_SCREEN_COORD, screen.BORDER_BOTTOM_SCREEN_COORD);
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.6f);
	vec2 vel = vec2(0.0f);
	constexpr float radiusMultiplier = 0.25f;
	constexpr float playerShootingCooldown = 0.5f;

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Player, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(playerSprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Player);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Enemy) | static_cast<int>(CollisionType::ReloadingCircle));
	collider->SetRadius(playerSprite->GetWidth() * radiusMultiplier);
	unique_ptr<Velocity> velocity = make_unique<Velocity>();
	unique_ptr<Health> health = make_unique<Health>();
	unique_ptr<Animation> animation = make_unique<Animation>();
	unique_ptr<Cooldown> cooldown = make_unique<Cooldown>(playerShootingCooldown);
	unique_ptr<Timer> playerDeathTimer = make_unique<Timer>(TimerType::PlayerDeath, 2.0f);

	AddComponent(playerEntityId, move(tag));
	AddComponent(playerEntityId, move(transform));
	AddComponent(playerEntityId, move(renderable));
	AddComponent(playerEntityId, move(collider));
	AddComponent(playerEntityId, move(velocity));
	AddComponent(playerEntityId, move(health));
	AddComponent(playerEntityId, move(animation));
	AddComponent(playerEntityId, move(cooldown));
	AddComponent(playerEntityId, move(playerDeathTimer));

	return playerEntityId;
}

EntityId EntityManager::CreateEnemyEntity(SpriteManager &spriteManager)
{
	EntityId enemyEntityId = CreateEntityId();
	CSimpleSprite *enemySprite = spriteManager.CreateSprite(enemyEntityId, Helper::PATH_TO_ENEMY, 4, 2);

	Screen &screen = screen.GetInstance();
	const float borderWidth = (screen.BORDER_RIGHT_SCREEN_COORD - screen.BORDER_LEFT_SCREEN_COORD);
	const float borderHeight = (screen.BORDER_BOTTOM_SCREEN_COORD - screen.BORDER_TOP_SCREEN_COORD);
	constexpr float minVx = -100.0f;
	constexpr float maxVx = 300.0f;
	constexpr float minVy = -100.0;
	constexpr float maxVy = 300.0f;
	vec3 playerPos = GetComponent<Transform>(m_playerEntityId)->GetPosition();
	vec3 enemyPos = Helper::GetOppositeQuadrantPosition(playerPos, borderWidth, borderHeight);
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.4f);
	constexpr float radiusMultiplier = 0.25f;
	vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Enemy, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(enemyPos, rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(enemySprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Enemy);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Player) | static_cast<int>(CollisionType::Bullet));
	collider->SetRadius(enemySprite->GetWidth() * radiusMultiplier);
	unique_ptr<Velocity> velocity = make_unique<Velocity>(randomVelocity);
	unique_ptr<BounceDirection> bounceDirection = make_unique<BounceDirection>();
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(enemyEntityId, move(tag));
	AddComponent(enemyEntityId, move(transform));
	AddComponent(enemyEntityId, move(renderable));
	AddComponent(enemyEntityId, move(collider));
	AddComponent(enemyEntityId, move(velocity));
	AddComponent(enemyEntityId, move(bounceDirection));
	AddComponent(enemyEntityId, move(animation));

	return enemyEntityId;
}

EntityId EntityManager::CreateBulletEntity(SpriteManager &spriteManager, const vec3 &pos, const vec2 &targetVelocity)
{
	EntityId bulletEntityId = CreateEntityId();
	CSimpleSprite *bulletSprite = spriteManager.CreateSprite(bulletEntityId, Helper::PATH_TO_BULLET_SPRITE, 1, 1);

	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);
	constexpr float radiusMultiplier = 0.25f;

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Bullet, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(pos, rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(bulletSprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Bullet);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Enemy));
	collider->SetRadius(bulletSprite->GetWidth() * radiusMultiplier);
	unique_ptr<Velocity> velocity = make_unique<Velocity>(targetVelocity);

	AddComponent(bulletEntityId, move(tag));
	AddComponent(bulletEntityId, move(transform));
	AddComponent(bulletEntityId, move(renderable));
	AddComponent(bulletEntityId, move(collider));
	AddComponent(bulletEntityId, move(velocity));

	return bulletEntityId;
}

EntityId EntityManager::CreateReloadingCircleEntity(SpriteManager &spriteManager)
{
	EntityId reloadingCircleEntityId = CreateEntityId();
	CSimpleSprite *reloadingCircleSprite = spriteManager.CreateSprite(reloadingCircleEntityId, Helper::PATH_TO_RELOADING_CIRCLE, 5, 2);

	Screen &screen = screen.GetInstance();
	const float xPos = Helper::GenerateFloat(screen.BORDER_LEFT_SCREEN_COORD, screen.BORDER_RIGHT_SCREEN_COORD);
	const float yPos = Helper::GenerateFloat(screen.BORDER_TOP_SCREEN_COORD, screen.BORDER_BOTTOM_SCREEN_COORD);
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.4f);
	constexpr float radiusMultiplier = 0.25f;

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::ReloadingCircle, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(reloadingCircleSprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::ReloadingCircle);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Player));
	collider->SetRadius(reloadingCircleSprite->GetWidth() * radiusMultiplier);
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(reloadingCircleEntityId, move(tag));
	AddComponent(reloadingCircleEntityId, move(transform));
	AddComponent(reloadingCircleEntityId, move(renderable));
	AddComponent(reloadingCircleEntityId, move(collider));
	AddComponent(reloadingCircleEntityId, move(animation));

	return reloadingCircleEntityId;
}

EntityId EntityManager::CreateAmmoEntity(SpriteManager &spriteManager, EntityType entityType, float xPos, float yPos)
{
	EntityId ammoEntityId = CreateEntityId();
	const char *pathToSprite = nullptr;
	if (entityType == EntityType::AmmoEmpty)
		pathToSprite = Helper::PATH_TO_AMMO_EMPTY;
	else if (entityType == EntityType::AmmoFilled)
		pathToSprite = Helper::PATH_TO_AMMO_FILLED;
	CSimpleSprite *ammoSprite = spriteManager.CreateSprite(ammoEntityId, pathToSprite, 1, 1);

	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.5f);

	unique_ptr<Tag> tag = make_unique<Tag>(entityType, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(ammoSprite);

	AddComponent(ammoEntityId, move(tag));
	AddComponent(ammoEntityId, move(transform));
	AddComponent(ammoEntityId, move(renderable));

	return ammoEntityId;
}

EntityId EntityManager::CreateHealthBarEntity(SpriteManager &spriteManager, float xPos, float yPos)
{
	EntityId healthBarEntityId = CreateEntityId();
	CSimpleSprite *healthBarSprite = spriteManager.CreateSprite(healthBarEntityId, Helper::PATH_TO_HEALTH_BAR, 2, 3);

	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::HealthBar, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(healthBarSprite);
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(healthBarEntityId, move(tag));
	AddComponent(healthBarEntityId, move(transform));
	AddComponent(healthBarEntityId, move(renderable));
	AddComponent(healthBarEntityId, move(animation));

	return healthBarEntityId;
}

EntityId EntityManager::CreateScoreEntity()
{
	EntityId scoreEntityId = CreateEntityId();
	Screen &screen = screen.GetInstance();
	constexpr float xOffset = 100.0f;
	constexpr float yOffset = 50.0f;
	const float xPos = screen.SCREEN_WIDTH - xOffset;
	const float yPos = screen.SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Score, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Score> score = make_unique<Score>();

	AddComponent(scoreEntityId, move(tag));
	AddComponent(scoreEntityId, move(transform));
	AddComponent(scoreEntityId, move(score));

	return scoreEntityId;
}

EntityId EntityManager::CreateCountdownTimerEntity()
{
	EntityId countdownTimerEntityId = CreateEntityId();
	Screen &screen = screen.GetInstance();
	constexpr float xOffset = 1000.0f;
	constexpr float yOffset = 50.0f;
	const float xPos = screen.SCREEN_WIDTH - xOffset;
	const float yPos = screen.SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Timer, GameState::Gameplay);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Timer> timer = make_unique<Timer>(TimerType::Countdown, 60.0f);

	AddComponent(countdownTimerEntityId, move(tag));
	AddComponent(countdownTimerEntityId, move(transform));
	AddComponent(countdownTimerEntityId, move(timer));

	return countdownTimerEntityId;
}

EntityId EntityManager::CreateTitleEntity(SpriteManager &spriteManager)
{
	EntityId titleEntityId = CreateEntityId();
	CSimpleSprite *titleSprite = spriteManager.CreateSprite(titleEntityId, Helper::PATH_TO_TITLE, 1, 1);

	Screen &screen = screen.GetInstance();
	const float titleXOffset = 520.0f;
	const float titleYOffset = 200.0f;
	const float xPos = screen.SCREEN_WIDTH - titleXOffset;
	const float yPos = screen.SCREEN_HEIGHT - titleYOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.5f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Title, GameState::MainMenu);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(titleSprite);

	AddComponent(titleEntityId, move(tag));
	AddComponent(titleEntityId, move(transform));
	AddComponent(titleEntityId, move(renderable));

	return titleEntityId;
}

EntityId EntityManager::CreatePlayButtonEntity(SpriteManager &spriteManager)
{
	EntityId playButtonEntityId = CreateEntityId();
	CSimpleSprite *playButtonSprite = spriteManager.CreateSprite(playButtonEntityId, Helper::PATH_TO_PLAY_BUTTON, 1, 1);

	Screen &screen = screen.GetInstance();
	const float playButtonXOffset = 520.0f;
	const float playButtonYOffset = 400.0f;
	const float xPos = screen.SCREEN_WIDTH - playButtonXOffset;
	const float yPos = screen.SCREEN_HEIGHT - playButtonYOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.2f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::PlayButton, GameState::MainMenu);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(playButtonSprite);

	AddComponent(playButtonEntityId, move(tag));
	AddComponent(playButtonEntityId, move(transform));
	AddComponent(playButtonEntityId, move(renderable));

	return playButtonEntityId;
}

EntityId EntityManager::CreateBackButtonEntity(SpriteManager &spriteManager)
{
	EntityId backButtonEntityId = CreateEntityId();
	CSimpleSprite *backButtonSprite = spriteManager.CreateSprite(backButtonEntityId, Helper::PATH_TO_BACK_BUTTON, 1, 1);

	Screen &screen = screen.GetInstance();
	const float backButtonXOffset = 520.0f;
	const float backButtonYOffset = 400.0f;
	const float xPos = screen.SCREEN_WIDTH - backButtonXOffset;
	const float yPos = screen.SCREEN_HEIGHT - backButtonYOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.2f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::BackButton, GameState::GameOver);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(backButtonSprite);

	AddComponent(backButtonEntityId, move(tag));
	AddComponent(backButtonEntityId, move(transform));
	AddComponent(backButtonEntityId, move(renderable));

	return backButtonEntityId;
}

EntityId EntityManager::CreateQuitButtonEntity(SpriteManager& spriteManager)
{
	EntityId quitButtonEntityId = CreateEntityId();
	CSimpleSprite* quitButtonSprite = spriteManager.CreateSprite(quitButtonEntityId, Helper::PATH_TO_QUIT_BUTTON, 1, 1);

	Screen& screen = screen.GetInstance();
	const float quitButtonXOffset = 520.0f;
	const float quitButtonYOffset = 400.0f;
	const float xPos = screen.SCREEN_WIDTH - quitButtonXOffset;
	const float yPos = screen.SCREEN_HEIGHT - quitButtonYOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.2f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::QuitButton, GameState::Paused);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(quitButtonSprite);

	AddComponent(quitButtonEntityId, move(tag));
	AddComponent(quitButtonEntityId, move(transform));
	AddComponent(quitButtonEntityId, move(renderable));

	return quitButtonEntityId;
}

EntityId EntityManager::CreateLoadingScreenCharacterEntity(SpriteManager &spriteManager)
{
	EntityId loadingScreenCharacterEntityId = CreateEntityId();
	CSimpleSprite *loadingScreenCharacterSprite = spriteManager.CreateSprite(loadingScreenCharacterEntityId, Helper::PATH_TO_PLAYER, 4, 4);

	Screen &screen = screen.GetInstance();
	const float loadingScreenCharacterXOffset = 520.0f;
	const float loadingScreenCharacterYOffset = 340.0f;
	const float xPos = screen.SCREEN_WIDTH - loadingScreenCharacterXOffset;
	const float yPos = screen.SCREEN_HEIGHT - loadingScreenCharacterYOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.5f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::LoadingScreenCharacter, GameState::Loading);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(loadingScreenCharacterSprite);
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(loadingScreenCharacterEntityId, move(tag));
	AddComponent(loadingScreenCharacterEntityId, move(transform));
	AddComponent(loadingScreenCharacterEntityId, move(renderable));
	AddComponent(loadingScreenCharacterEntityId, move(animation));

	return loadingScreenCharacterEntityId;
}

EntityId EntityManager::CreateStarfieldEntity(SpriteManager &spriteManager)
{
	EntityId starfieldEntityId = CreateEntityId();
	CSimpleSprite *starfieldSprite = spriteManager.CreateSprite(starfieldEntityId, Helper::PATH_TO_STARFIELD, 1, 1);

	Screen &screen = screen.GetInstance();
	const float xPos = screen.SCREEN_WIDTH / 2.0f;
	const float yPos = screen.SCREEN_HEIGHT / 2.0f;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(2.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Starfield, GameState::GameOver);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(starfieldSprite);

	AddComponent(starfieldEntityId, move(tag));
	AddComponent(starfieldEntityId, move(transform));
	AddComponent(starfieldEntityId, move(renderable));

	return starfieldEntityId;
}

void EntityManager::MoveEntityToRandomPos(EntityId entityId)
{
	Screen &screen = screen.GetInstance();
	const float xPos = Helper::GenerateFloat(screen.BORDER_LEFT_SCREEN_COORD, screen.BORDER_RIGHT_SCREEN_COORD);
	const float yPos = Helper::GenerateFloat(screen.BORDER_TOP_SCREEN_COORD, screen.BORDER_BOTTOM_SCREEN_COORD);
	constexpr float zPos = 0.0f;

	vec3 newPos = vec3(xPos, yPos, zPos);
	Transform *transform = GetComponent<Transform>(entityId);
	transform->SetPosition(newPos);
}

void EntityManager::MarkEntityForDeletion(EntityId entityId)
{
	// Check if already marked for deletion
	if (find(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), entityId) == m_entitiesToDelete.end())
	{
		m_entitiesToDelete.push_back(entityId);
	}
}

void EntityManager::ProcessDeletions()
{
	GameManager& gameManager = GameManager::GetInstance();

	if (gameManager.GetCurrentGameState() == GameState::Paused)
		return;

	for (EntityId entityId : m_entitiesToDelete)
	{
		m_entityComponents.erase(entityId);
	}
	m_entitiesToDelete.clear();
}

void EntityManager::ResetEnemies()
{
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	vector<EntityId> allEntities = GetAllEntities();
	
	// Delete all enemies
	for (EntityId entityId : allEntities) 
	{
		Tag* tag = GetComponent<Tag>(entityId);
		if (tag->GetEntityType() == EntityType::Enemy)
			MarkEntityForDeletion(entityId);
	}
	ProcessDeletions();

	// Create one new enemy
	CreateEnemyEntity(spriteManager);
}
