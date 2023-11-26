#include "stdafx.h"
#include "../include/ScoreHandler.h"

void ScoreHandler::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime) {
	Entity scoreEntity = entityManager.GetScoreEntity();
	auto score = entityManager.GetComponent<Score>(scoreEntity);
	score->score += 1;
}