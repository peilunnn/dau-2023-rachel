#ifndef STARTBUTTON_H
#define STARTBUTTON_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"
#include "../../Components/include/Button.h"

Entity CreateStartButton(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position, std::function<void()> onClick) {
    Entity startButton = entityManager.CreateEntity();
    entityManager.AddComponent(startButton, Text(text, color, fontSize));
    entityManager.AddComponent(startButton, Transform(position, glm::vec3(0), glm::vec3(1)));
    entityManager.AddComponent(startButton, Button(onClick));
    return startButton;
}

#endif // STARTBUTTON_H
