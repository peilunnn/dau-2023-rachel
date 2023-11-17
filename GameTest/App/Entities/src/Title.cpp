#include "stdafx.h"
#include "../include/Title.h"

Entity CreateTitle(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position)
{
    Entity title = entityManager.CreateEntity();
    entityManager.AddComponent(title, std::make_shared<Text>(text, color, fontSize));
    entityManager.AddComponent(title, std::make_shared<Transform>(position, glm::vec3(0), glm::vec3(1)));
    return title;
}
