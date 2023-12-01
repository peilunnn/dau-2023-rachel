#include "stdafx.h"
#include "../include/ScoreHandler.h"
#include "Components/include/Score.h"

void ScoreHandler::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime) {
	EntityId scoreEntityId = entityManager.GetScoreEntityId();
	auto score = entityManager.GetComponent<Score>(scoreEntityId);
	auto currentScore = score->GetScore();
	score->SetScore(currentScore + 1);
}