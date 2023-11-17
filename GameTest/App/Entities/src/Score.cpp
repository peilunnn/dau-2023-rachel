#include "stdafx.h"
#include "../include/Score.h"

Entity CreateScore(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position)
{
    Entity score = entityManager.CreateEntity();
    entityManager.AddComponent(score, std::make_shared<Text>(text, color, fontSize));
    entityManager.AddComponent(score, std::make_shared<Transform>(position, glm::vec3(0), glm::vec3(1)));
    return score;
}
