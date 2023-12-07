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

    void Render(GameState gameState, CSimpleSprite* titleSprite, CSimpleSprite* playButtonSprite);
    void RenderMainMenu(EntityManager& entityManager, Screen& screen, CSimpleSprite* titleSprite, CSimpleSprite* playButtonSprite);
    void RenderGameScene(EntityManager& entityManager, Screen& screen);

private:
    RenderingHandler() = default;

    void RenderEntities(EntityManager& entityManager, EntityId entityId);
    void RenderSprites(EntityManager& entityManager, EntityId entityId, Tag* tag, Transform* transform);
    void RenderScore(EntityManager& entityManager, EntityId entityId, Tag* tag, Transform* transform);
    void RenderTimer(EntityManager& entityManager, EntityId entityId, Tag* tag, Transform* transform);

    void SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void DrawBorder(Screen& screen, GLfloat red, GLfloat green, GLfloat blue);
    void DrawBackgroundInBorder(Screen& screen, GLfloat red, GLfloat green, GLfloat blue);
};
