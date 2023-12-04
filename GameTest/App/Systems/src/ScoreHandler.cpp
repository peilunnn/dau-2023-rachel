#include "stdafx.h"
#include "../include/ScoreHandler.h"
#include "Components/include/Score.h"

void ScoreHandler::HandleBulletHitEnemy(EntityManager &entityManager, float deltaTime)
{
	EntityId scoreEntityId = entityManager.GetScoreEntityId();
	shared_ptr<Score> score = entityManager.GetComponent<Score>(scoreEntityId);
	int currentScore = score->GetScore();
	score->SetScore(currentScore + 1);
}