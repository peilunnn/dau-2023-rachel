#ifndef SCORE_H
#define SCORE_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateScore(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position) {
    Entity score = entityManager.CreateEntity();
    entityManager.AddComponent(score, new Text(text, color, fontSize));
    entityManager.AddComponent(score, new Transform(position, glm::vec3(0), glm::vec3(1)));
    return score;
}

#endif // SCORE_H
