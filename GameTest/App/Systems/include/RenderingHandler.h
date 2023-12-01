#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include <memory>

class RenderingHandler : public System {
public:
    void Render(EntityManager& entityManager);
    SystemType GetSystemType() const override {
        return SystemType::RenderingHandler;
    }

private:
    void RenderEntities(EntityManager& entityManager, EntityId entityId);
    void RenderSprites(EntityManager& entityManager, EntityId entityId, shared_ptr<Tag> tag, shared_ptr<Transform> transform);
    void RenderScore(EntityManager& entityManager, EntityId entityId, shared_ptr<Tag> tag, shared_ptr<Transform> transform);
    void RenderTimer(EntityManager& entityManager, EntityId entityId, shared_ptr<Tag> tag, shared_ptr<Transform> transform);

    void SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void DrawBorder(GLfloat red, GLfloat green, GLfloat blue);
    void DrawBackgroundInBorder(GLfloat red, GLfloat green, GLfloat blue);
};
