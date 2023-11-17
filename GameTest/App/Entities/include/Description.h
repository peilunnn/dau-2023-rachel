#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateDescription(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, glm::vec3& position) {
    Entity description = entityManager.CreateEntity();
    entityManager.AddComponent(description, Text(text, color, fontSize));
    entityManager.AddComponent(description, Transform(position, glm::vec3(0), glm::vec3(1)));
    return description;
}

#endif // DESCRIPTION_H
