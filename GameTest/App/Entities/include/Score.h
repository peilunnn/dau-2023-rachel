#ifndef SCORE_H
#define SCORE_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateScore(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position);

#endif // SCORE_H
