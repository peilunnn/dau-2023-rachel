#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateCountdown(EntityManager& entityManager, const std::string& text, glm::vec3& color, int fontSize, const glm::vec3& position);

#endif // COUNTDOWN_H
