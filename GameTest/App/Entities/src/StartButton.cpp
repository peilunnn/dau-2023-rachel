#include "stdafx.h"
#include "../include/StartButton.h"

Entity CreateStartButton(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position, std::function<void()> onClick)
{
    Entity startButton = entityManager.CreateEntity();
    entityManager.AddComponent(startButton, std::make_shared<Text>(text, color, fontSize));
    entityManager.AddComponent(startButton, std::make_shared<Transform>(position, glm::vec3(0), glm::vec3(1)));
    entityManager.AddComponent(startButton, std::make_shared<Button>(onClick));
    return startButton;
}
