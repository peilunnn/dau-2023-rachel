#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Renderable.h"
#include "Components/include/Transform.h"
#include "Components/include/Score.h"
#include "Components/include/Timer.h"
#include "Systems/include/ScreenHandler.h"
#include "Utilities/include/Helper.h"
#include "Utilities/include/App.h"
#include "../include/RenderingHandler.h"

void RenderingHandler::Render(EntityManager& entityManager)
{
    SetBackground(0.2f, 0.2f, 0.2f, 1.0f);
    DrawBorder(1.0f, 1.0f, 1.0f);
    DrawBackgroundInBorder(0.0f, 0.0f, 0.0f);

    for (auto entityId : entityManager.GetEntitiesWithComponents<Tag, Transform>())
    {
        RenderEntities(entityManager, entityId);
    }
}

void RenderingHandler::RenderEntities(EntityManager& entityManager, EntityId entityId)
{
    auto tag = entityManager.GetComponent<Tag>(entityId);
    auto transform = entityManager.GetComponent<Transform>(entityId);

    RenderSprites(entityManager, entityId, tag, transform);
    RenderScore(entityManager, entityId, tag, transform);
    RenderTimer(entityManager, entityId, tag, transform);
}

void RenderingHandler::RenderSprites(EntityManager& entityManager, EntityId entityId, shared_ptr<Tag> tag, shared_ptr<Transform> transform)
{
    auto renderable = entityManager.GetComponent<Renderable>(entityId);

    if (!renderable)
        return;

    auto sprite = renderable->GetSprite();

    sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    sprite->SetScale(transform->GetScale().x);

    if (tag->GetEntityType() == EntityType::AmmoFilled && !(sprite->GetIsVisible()))
        return;

    sprite->Draw();
}

void RenderingHandler::RenderScore(EntityManager& entityManager, EntityId entityId, shared_ptr<Tag> tag, shared_ptr<Transform> transform)
{
    auto score = entityManager.GetComponent<Score>(entityId);
    if (score && tag->GetEntityType() == EntityType::Score)
    {
        string scoreText = "Score: " + to_string(score->GetScore());
        App::Print(transform->GetPosition().x, transform->GetPosition().y, scoreText.c_str(), 1.0f, 1.0f, 1.0f);
    }
}

void RenderingHandler::RenderTimer(EntityManager& entityManager, EntityId entityId, shared_ptr<Tag> tag, shared_ptr<Transform> transform)
{
    auto timer = entityManager.GetComponent<Timer>(entityId);
    if (timer && tag->GetEntityType() == EntityType::Timer)
    {
        string timerText = to_string(static_cast<int>(timer->GetCountdownTime()));
        App::Print(transform->GetPosition().x, transform->GetPosition().y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
    }
}

void RenderingHandler::SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(GLfloat red, GLfloat green, GLfloat blue)
{
	constexpr float borderThickness = ScreenHandler::BORDER_THICKNESS;
	constexpr float borderLeftX = ScreenHandler::BORDER_LEFT_X;
	constexpr float borderRightX = ScreenHandler::BORDER_RIGHT_X;
	constexpr float borderTopY = ScreenHandler::BORDER_TOP_Y;
	constexpr float borderBottomY = ScreenHandler::BORDER_BOTTOM_Y;

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
	float borderThickness = ScreenHandler::BORDER_THICKNESS;
	float borderLeftX = ScreenHandler::BORDER_LEFT_X;
	float borderRightX = ScreenHandler::BORDER_RIGHT_X;
	float borderTopY = ScreenHandler::BORDER_TOP_Y;
	float borderBottomY = ScreenHandler::BORDER_BOTTOM_Y;

	glColor3f(red, green, blue);
	glBegin(GL_QUADS);
	glVertex2f(borderLeftX, borderTopY);
	glVertex2f(borderRightX, borderTopY);
	glVertex2f(borderRightX, borderBottomY);
	glVertex2f(borderLeftX, borderBottomY);
	glEnd();
}
