#include "stdafx.h"
#include "Components/include/Score.h"
#include "Systems/include/ScoreHandler.h"

ScoreHandler& ScoreHandler::GetInstance()
{
	static ScoreHandler instance;
	return instance;
}

void ScoreHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "BulletHitEnemy") {
		HandleBulletHitEnemy(entityManager, deltaTime);
	}
}

void ScoreHandler::HandleBulletHitEnemy(EntityManager &entityManager, float deltaTime)
{
	EntityId scoreEntityId = entityManager.GetScoreEntityId();
	Score* score = entityManager.GetComponent<Score>(scoreEntityId);
	int currentScore = score->GetScore();
	score->SetScore(currentScore + 1);
}