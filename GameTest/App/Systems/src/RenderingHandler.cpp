#include "stdafx.h"
#include "../include/RenderingHandler.h"
#include "../../Utilities/include/Helper.h"

void RenderingHandler::Render(EntityManager &entityManager)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Transform, Renderable>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto renderable = entityManager.GetComponent<Renderable>(entity);

        if (!(transform && renderable))
            continue;

        renderable->sprite->SetPosition(transform->position.x, transform->position.y);
        renderable->sprite->SetScale(transform->scale.x);
        renderable->sprite->SetAngle(transform->rotation.z);

        if (tag->entityType == EntityType::AMMO_FILLED)
        {
            bool isVisible = renderable->sprite->GetIsVisible();
            if (!isVisible)
                continue;
        }

        renderable->sprite->Draw();
    }
}

void RenderingHandler::SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3f(red, green, blue);
    glLineWidth(borderThickness);
    glBegin(GL_LINE_LOOP);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}

void RenderingHandler::DrawBackgroundInBorder(GLfloat red, GLfloat green, GLfloat blue)
{
    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();

}
