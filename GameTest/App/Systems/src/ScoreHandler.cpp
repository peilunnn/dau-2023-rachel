#include "stdafx.h"
#include "../include/ScoreHandler.h"

void ScoreHandler::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime) {
	EntityId scoreEntity = entityManager.GetScoreEntityId();
	auto score = entityManager.GetComponent<Score>(scoreEntity);
	score->score += 1;
}