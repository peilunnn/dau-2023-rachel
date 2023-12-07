#pragma once
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class RenderingHandler : public System {
public:
    static RenderingHandler& GetInstance() {
        static RenderingHandler instance;
        return instance;
    }
    RenderingHandler(RenderingHandler const&) = delete;
    void operator=(RenderingHandler const&) = delete;

    void Render(GameState gameState);
    void RenderMainMenu(EntityManager& entityManager, Screen& screen);
    void RenderGameScene(EntityManager& entityManager, Screen& screen);

private:
    RenderingHandler() = default;

    void RenderSprite(EntityManager& entityManager, EntityId entityId);
    void RenderScore(EntityManager& entityManager);
    void RenderTimer(EntityManager& entityManager);

    void SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void DrawBorder(Screen& screen, GLfloat red, GLfloat green, GLfloat blue);
    void DrawBackgroundInBorder(Screen& screen, GLfloat red, GLfloat green, GLfloat blue);
};
