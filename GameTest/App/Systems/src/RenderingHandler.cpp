#include "stdafx.h"
#include "../include/RenderingHandler.h"
#include "../../Utilities/include/Helper.h"

void RenderingHandler::Render(EntityManager& entityManager)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Transform>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);
        auto transform = entityManager.GetComponent<Transform>(entity);

        // Render sprites
        auto renderable = entityManager.GetComponent<Renderable>(entity);
        if (renderable)
        {
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

        // Render score
        auto score = entityManager.GetComponent<Score>(entity);
        if (score && tag->entityType == EntityType::SCORE)
        {
            std::string scoreText = "Score: " + std::to_string(score->score);
            float x = transform->position.x;
            float y = transform->position.y;
            App::Print(x, y, scoreText.c_str(), 1.0f, 1.0f, 1.0f);
        }

        // Render timer
        auto timer = entityManager.GetComponent<Timer>(entity);
        if (timer && tag->entityType == EntityType::TIMER)
        {
            std::string timerText = std::to_string(static_cast<int>(timer->countdownTime));
            float x = transform->position.x;
            float y = transform->position.y;
            App::Print(x, y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
        }
    }
}

void RenderingHandler::SetBackground(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(GLfloat red, GLfloat green, GLfloat blue)
{
    float borderThickness = ScreenHandler::BORDER_THICKNESS;
    float borderLeftX = ScreenHandler::BORDER_LEFT_X;
    float borderRightX = ScreenHandler::BORDER_RIGHT_X;
    float borderTopY = ScreenHandler::BORDER_TOP_Y;
    float borderBottomY = ScreenHandler::BORDER_BOTTOM_Y;

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