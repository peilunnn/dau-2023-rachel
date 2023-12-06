#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Renderable.h"
#include "Components/include/Score.h"
#include "Components/include/Transform.h"
#include "Components/include/Timer.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"
#include "../include/RenderingHandler.h"

void RenderingHandler::Render(GameState gameState, CSimpleSprite* titleSprite)
{
    EntityManager& entityManager = EntityManager::GetInstance();
    Screen& screen = Screen::GetInstance();

    switch (gameState) {
    case MAIN_MENU:
        RenderMainMenu(entityManager, screen, titleSprite);
        break;
    case GAMEPLAY:
        RenderGameScene(entityManager, screen);
        break;
    }
}

void RenderingHandler::RenderMainMenu(EntityManager& entityManager, Screen& screen, CSimpleSprite* titleSprite)
{
    EntityId titleEntityId = entityManager.GetTitleEntityId();
    titleSprite = entityManager.GetComponent<Renderable>(titleEntityId)->GetSprite();
    Transform* transform = entityManager.GetComponent<Transform>(titleEntityId);
    const float amplitude = 20.0f; // Max rotation angle.
    const float frequency = 2.0f; // Adjust this value for faster or slower rotation.
    //float titleRotationAngle = amplitude * sin(App::GetTime() * frequency);

    SetBackground(screen.R_MAIN_MENU_BG, screen.G_MAIN_MENU_BG, screen.B_MAIN_MENU_BG, screen.ALPHA_MAIN_MENU_BG);

    titleSprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    //titleSprite->SetAngle(titleRotationAngle);
    titleSprite->SetScale(transform->GetScale().x);
    titleSprite->Draw();

    constexpr char* descriptionText = "Get the highest score in 60 seconds!";
    App::Print(screen.SCREEN_WIDTH - screen.DESCRIPTION_X_OFFSET, screen.SCREEN_HEIGHT - screen.DESCRIPTION_Y_OFFSET, descriptionText, screen.R_TEXT, screen.G_TEXT, screen.B_TEXT);
}

void RenderingHandler::RenderGameScene(EntityManager& entityManager, Screen& screen)
{
    SetBackground(screen.R_GAMEPLAY_BG, screen.G_GAMEPLAY_BG, screen.B_GAMEPLAY_BG, screen.ALPHA_GAMEPLAY_BG);
    DrawBorder(screen, screen.R_BORDER, screen.G_BORDER, screen.B_BORDER);
    DrawBackgroundInBorder(screen, screen.R_BG_IN_BORDER, screen.G_BG_IN_BORDER, screen.B_BG_IN_BORDER);

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Tag, Transform>())
        RenderEntities(entityManager, entityId);
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

    CSimpleSprite* sprite = renderable->GetSprite();

    sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    sprite->SetScale(transform->GetScale().x);

    if (tag->GetEntityType() == EntityType::AmmoFilled && !(sprite->GetIsVisible()))
        return;

    sprite->Draw();
}

void RenderingHandler::RenderScore(EntityManager &entityManager, EntityId entityId, Tag* tag, Transform* transform)
{
    Score* score = entityManager.GetComponent<Score>(entityId);
    constexpr float r = 1.0f;
    constexpr float g = 1.0f;
    constexpr float b = 1.0f;

    if (!score)
        return;

    string scoreText = "Score: " + to_string(score->GetScore());
    App::Print(transform->GetPosition().x, transform->GetPosition().y, scoreText.c_str(), r, g, b);
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

void RenderingHandler::DrawBorder(Screen& screen, GLfloat red, GLfloat green, GLfloat blue)
{
    const float borderThickness = screen.BORDER_THICKNESS;
    const float borderLeftX = screen.BORDER_LEFT_X;
    const float borderRightX = screen.BORDER_RIGHT_X;
    const float borderTopY = screen.BORDER_TOP_Y;
    const float borderBottomY = screen.BORDER_BOTTOM_Y;

    glColor3f(red, green, blue);
    glLineWidth(borderThickness);
    glBegin(GL_LINE_LOOP);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}

void RenderingHandler::DrawBackgroundInBorder(Screen& screen, GLfloat red, GLfloat green, GLfloat blue)
{
    float borderThickness = screen.BORDER_THICKNESS;
    float borderLeftX = screen.BORDER_LEFT_X;
    float borderRightX = screen.BORDER_RIGHT_X;
    float borderTopY = screen.BORDER_TOP_Y;
    float borderBottomY = screen.BORDER_BOTTOM_Y;

    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}
