#include "stdafx.h"
#include "../include/ScoreHandler.h"

void ScoreHandler::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime) {
	Entity scoreEntity = entityManager.GetScoreEntity();
	auto score = entityManager.GetComponent<Score>(scoreEntity);
	Helper::Log("before increment, score is: ", score->score);
	score->score += 1;
	Helper::Log("after increment, score is: ", score->score);
}