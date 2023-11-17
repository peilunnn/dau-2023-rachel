#include "stdafx.h"
#include "../include/Countdown.h"

Entity CreateCountdown(EntityManager& entityManager, const std::string& text, glm::vec3& color, int fontSize, const glm::vec3& position)
{
    Entity countdown = entityManager.CreateEntity();
    entityManager.AddComponent(countdown, std::make_shared<Text>(text, color, fontSize));
    entityManager.AddComponent(countdown, std::make_shared<Transform>(position, glm::vec3(0), glm::vec3(1)));
    return countdown;
}
