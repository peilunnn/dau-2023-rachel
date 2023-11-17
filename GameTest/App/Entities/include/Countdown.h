#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateCountdown(EntityManager& entityManager, const std::string& text, glm::vec3& color, int fontSize, const glm::vec3& position) {
    Entity countdown = entityManager.CreateEntity();
    entityManager.AddComponent(countdown, Text(text, color, fontSize));
    entityManager.AddComponent(countdown, Transform(position, glm::vec3(0), glm::vec3(1)));
    return countdown;
}

#endif // COUNTDOWN_H
