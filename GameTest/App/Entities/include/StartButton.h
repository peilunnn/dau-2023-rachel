#ifndef STARTBUTTON_H
#define STARTBUTTON_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"
#include "../../Components/include/Button.h"

Entity CreateStartButton(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position, std::function<void()> onClick);

#endif // STARTBUTTON_H
