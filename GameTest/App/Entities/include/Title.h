#ifndef TITLE_H
#define TITLE_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Text.h"

Entity CreateTitle(EntityManager& entityManager, const std::string& text, const glm::vec3& color, int fontSize, const glm::vec3& position);

#endif // TITLE_H
