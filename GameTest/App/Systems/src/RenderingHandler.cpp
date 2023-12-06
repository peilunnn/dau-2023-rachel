#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Renderable.h"
#include "Components/include/Transform.h"
#include "Components/include/Score.h"
#include "Components/include/Timer.h"
#include "Systems/include/ScreenHandler.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"
#include "../include/RenderingHandler.h"

void RenderingHandler::Render()
{
    ScreenHandler& screenHandler = ScreenHandler::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();

    SetBackground(0.2f, 0.2f, 0.2f, 1.0f);
    DrawBorder(screenHandler, 1.0f, 1.0f, 1.0f);
    DrawBackgroundInBorder(screenHandler, 0.0f, 0.0f, 0.0f);

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Tag, Transform>())
    {
        RenderEntities(entityManager, entityId);
    }
}

void RenderingHandler::RenderEntities(EntityManager &entityManager, EntityId entityId)
{
    Tag* tag = entityManager.GetComponent<Tag>(entityId);
    Transform* transform = entityManager.GetComponent<Transform>(entityId);

    RenderSprites(entityManager, entityId, tag, transform);
    RenderScore(entityManager, entityId, tag, transform);
    RenderTimer(entityManager, entityId, tag, transform);
}

void RenderingHandler::RenderSprites(EntityManager &entityManager, EntityId entityId, Tag* tag, Transform* transform)
{
    Renderable* renderable = entityManager.GetComponent<Renderable>(entityId);

    if (!renderable)
        return;

    shared_ptr<CSimpleSprite> sprite = renderable->GetSprite();

    sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    sprite->SetScale(transform->GetScale().x);

    if (tag->GetEntityType() == EntityType::AmmoFilled && !(sprite->GetIsVisible()))
        return;

    sprite->Draw();
}

void RenderingHandler::RenderScore(EntityManager &entityManager, EntityId entityId, Tag* tag, Transform* transform)
{
    Score* score = entityManager.GetComponent<Score>(entityId);
    if (!score)
        return;

        string scoreText = "Score: " + to_string(score->GetScore());
        App::Print(transform->GetPosition().x, transform->GetPosition().y, scoreText.c_str(), 1.0f, 1.0f, 1.0f);
}

void RenderingHandler::RenderTimer(EntityManager &entityManager, EntityId entityId, Tag* tag, Transform* transform)
{
    Timer* timer = entityManager.GetComponent<Timer>(entityId);
    if (!timer)
        return;

        string timerText = to_string(static_cast<int>(timer->GetCountdownTime()));
        App::Print(transform->GetPosition().x, transform->GetPosition().y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
}

void RenderingHandler::SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(ScreenHandler& screenHandler, GLfloat red, GLfloat green, GLfloat blue)
{
    const float borderThickness = screenHandler.BORDER_THICKNESS;
    const float borderLeftX = screenHandler.BORDER_LEFT_X;
    const float borderRightX = screenHandler.BORDER_RIGHT_X;
    const float borderTopY = screenHandler.BORDER_TOP_Y;
    const float borderBottomY = screenHandler.BORDER_BOTTOM_Y;

    glColor3f(red, green, blue);
    glLineWidth(borderThickness);
    glBegin(GL_LINE_LOOP);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}

void RenderingHandler::DrawBackgroundInBorder(ScreenHandler& screenHandler, GLfloat red, GLfloat green, GLfloat blue)
{
    float borderThickness = screenHandler.BORDER_THICKNESS;
    float borderLeftX = screenHandler.BORDER_LEFT_X;
    float borderRightX = screenHandler.BORDER_RIGHT_X;
    float borderTopY = screenHandler.BORDER_TOP_Y;
    float borderBottomY = screenHandler.BORDER_BOTTOM_Y;

    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}
