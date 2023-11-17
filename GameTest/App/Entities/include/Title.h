#ifndef TITLE_H
#define TITLE_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateTitle(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position) {
    Entity title = entityManager.CreateEntity();
    entityManager.AddComponent(title, Text(text, color, fontSize));
    entityManager.AddComponent(title, Transform(position, glm::vec3(0), glm::vec3(1)));
    return title;
}

#endif // TITLE_H
