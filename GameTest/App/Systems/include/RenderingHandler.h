#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Animation.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/ScreenHandler.h"

class RenderingHandler : public System
{
public:
    void Render(EntityManager &entityManager);
    void SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void DrawBorder(GLfloat red, GLfloat green, GLfloat blue);
    void DrawBackgroundInBorder(GLfloat red, GLfloat green, GLfloat blue);

    SystemType GetSystemType() const override
    {
        return SystemType::RenderingHandler;
    }
};