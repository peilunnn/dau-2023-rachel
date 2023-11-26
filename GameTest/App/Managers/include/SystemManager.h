#pragma once
#include "../../Systems/include/Event.h"
#include "../../Systems/include/ScoreHandler.h"
#include "../../Systems/include/ScreenHandler.h"
#include "../../Systems/include/System.h"
#include "EntityManager.h"
#include <queue>
using namespace std;

class SystemManager {
private:
	vector<unique_ptr<System>> systems;

public:
	queue<Event> eventQueue;

	void Init();
	void AddSystem(unique_ptr<System> system);
	void SendEvent(Event event);
	void ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime, const glm::vec3& playerPos);
	void HandleBulletHitEnemyEvent(EntityManager& entityManager, ScoreHandler& scoreHandler, const Event& event, float deltaTime, const glm::vec3& playerPos);
	void HandleEnemyHitPlayerEvent(EntityManager& entityManager, const Event& event, float deltaTime);
	void HandlePlayerHitPlayerReloadingCircle(EntityManager& entityManager, const Event& event, float deltaTime);
};