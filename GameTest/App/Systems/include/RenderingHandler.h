#ifndef RENDERING_HANDLER_H
#define RENDERING_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Animation.h"
#include "../../Systems/include/System.h"

class RenderingHandler : public System {
private:
    const float borderLeftX = -0.9f;
    const float borderTopY = 0.75f;
    const float borderRightX = 0.9f;
    const float borderBottomY = -0.68f;
    const float borderThickness = 10.0f;

public:
    void Render(EntityManager& entityManager);
    void SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void DrawBorder(GLfloat red, GLfloat green, GLfloat blue);
    void DrawBackgroundInBorder(GLfloat red, GLfloat green, GLfloat blue);

    Type GetSystemType() const override {
        return System::Type::RenderingHandler;
    }
};

#endif // RENDERING_HANDLER_H
