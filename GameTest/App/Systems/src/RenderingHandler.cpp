#include "stdafx.h"
#include "Components/include/Renderable.h"
#include "Components/include/Score.h"
#include "Components/include/Timer.h"
#include "Components/include/Transform.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/RenderingHandler.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"

void RenderingHandler::Render()
{
    GameState m_gameState = GameManager::GetInstance().GetGameState();
    EntityManager &entityManager = EntityManager::GetInstance();
    Screen &screen = Screen::GetInstance();

    switch (m_gameState)
    {
        case GameState::MainMenu: 
            RenderMainMenuScene(entityManager, screen);
            break;
        case GameState::Gameplay:
            RenderGameScene(entityManager, screen);
            break;
        case GameState::GameOver:
            RenderGameOverScene(entityManager, screen);
            break;
        case GameState::Loading:
            RenderLoadingScreen(screen);
            break;
    }
}

void RenderingHandler::RenderMainMenuScene(EntityManager &entityManager, Screen &screen)
{
    SetBackground(BLACK);

    const float descriptionXOffset = 600.0f;
    const float descriptionYOffset = 450.0f;

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Renderable>())
    {
        Tag *tag = entityManager.GetComponent<Tag>(entityId);

        if (tag->GetGameState() != GameState::MainMenu)
            continue;

        CSimpleSprite *sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
        Transform *transform = entityManager.GetComponent<Transform>(entityId);
        sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
        sprite->SetScale(transform->GetScale().x);

        if (entityId == entityManager.GetTitleEntityId())
            sprite->SetAngle(transform->GetRotation().z);

        sprite->Draw();
    }

    constexpr char *descriptionText = "Get the highest score in 60 seconds!";
    App::Print(screen.SCREEN_WIDTH - descriptionXOffset, screen.SCREEN_HEIGHT - descriptionYOffset, descriptionText, WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderGameScene(EntityManager &entityManager, Screen &screen)
{
    SetBackground(BLACK);
    DrawBorder(screen, WHITE);
    DrawBackgroundInBorder(screen, DARK_GREY);

    for (EntityId entityId : entityManager.GetAmmoEmptyEntityIds())
        RenderSprite(entityManager, entityId);

    for (EntityId entityId : entityManager.GetAmmoFilledEntityIds())
        RenderSprite(entityManager, entityId);

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Renderable>())
    {
        Tag *tag = entityManager.GetComponent<Tag>(entityId);

        if (tag->GetGameState() != GameState::Gameplay)
            continue;

        if (tag->GetEntityType() != EntityType::AmmoEmpty && tag->GetEntityType() != EntityType::AmmoFilled)
            RenderSprite(entityManager, entityId);
    }

    RenderScore(entityManager);
    RenderTimer(entityManager);
}

void RenderingHandler::RenderGameOverScene(EntityManager &entityManager, Screen &screen)
{
    SetBackground(BLACK);

    EntityId scoreEntityId = entityManager.GetScoreEntityId();
    Score *score = entityManager.GetComponent<Score>(scoreEntityId);
    const float gameOverTextXOffset = 545.0f;
    const float gameOverTextYOffset = 410.0f;
    const float scoreTextXOffset = 570.0f;
    const float scoreTextYOffset = 450.0f;
    const float gameOverTextX = screen.SCREEN_WIDTH - gameOverTextXOffset;
    const float gameOverTextY = screen.SCREEN_HEIGHT - gameOverTextYOffset;
    const float scoreTextX = screen.SCREEN_WIDTH - scoreTextXOffset;
    const float scoreTextY = screen.SCREEN_HEIGHT - scoreTextYOffset;
    constexpr char *gameOverText = "Game Over";
    string scoreMessage = "You got a score of : " + to_string(score->GetScore());

    App::Print(scoreTextX, scoreTextY, scoreMessage.c_str(), WHITE.r, WHITE.g, WHITE.b);
    App::Print(gameOverTextX, gameOverTextY, gameOverText, WHITE.r, WHITE.g, WHITE.b);

    EntityId backButtonEntityId = entityManager.GetBackButtonEntityId();
    CSimpleSprite *backButtonSprite = entityManager.GetComponent<Renderable>(backButtonEntityId)->GetSprite();
    Transform *transform = entityManager.GetComponent<Transform>(backButtonEntityId);
    backButtonSprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    backButtonSprite->SetScale(transform->GetScale().x);
    backButtonSprite->Draw();
}

void RenderingHandler::RenderLoadingScreen(Screen& screen) {
    SetBackground(BLACK);

    const float loadingTextX = screen.SCREEN_WIDTH / 2.0f;
    const float loadingTextY = screen.SCREEN_HEIGHT / 2.0f;
    constexpr char* loadingText = "Loading...";

    App::Print(loadingTextX, loadingTextY, loadingText, WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderSprite(EntityManager &entityManager, EntityId entityId)
{
    Tag *tag = entityManager.GetComponent<Tag>(entityId);
    Transform *transform = entityManager.GetComponent<Transform>(entityId);
    Renderable *renderable = entityManager.GetComponent<Renderable>(entityId);
    CSimpleSprite *sprite = renderable->GetSprite();

    sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    sprite->SetScale(transform->GetScale().x);

    if (tag->GetEntityType() == EntityType::AmmoFilled && !(sprite->GetIsVisible()))
        return;

    sprite->Draw();
}

void RenderingHandler::RenderScore(EntityManager &entityManager)
{
    EntityId scoreEntityId = entityManager.GetScoreEntityId();
    Score *score = entityManager.GetComponent<Score>(scoreEntityId);
    Transform *scoreTransform = entityManager.GetComponent<Transform>(scoreEntityId);

    if (!score)
        return;

    string scoreText = "Score: " + to_string(score->GetScore());
    App::Print(scoreTransform->GetPosition().x, scoreTransform->GetPosition().y, scoreText.c_str(), WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderTimer(EntityManager &entityManager)
{
    EntityId timerEntityId = entityManager.GetTimerEntityId();
    Timer *timer = entityManager.GetComponent<Timer>(timerEntityId);
    Transform *timerTransform = entityManager.GetComponent<Transform>(timerEntityId);

    string timerText = to_string(static_cast<int>(timer->GetDuration()));
    App::Print(timerTransform->GetPosition().x, timerTransform->GetPosition().y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
}

void RenderingHandler::SetBackground(const Color &color)
{
    glClearColor(color.r, color.g, color.b, color.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(Screen &screen, const Color &color)
{
    const float borderThickness = screen.BORDER_THICKNESS;
    const float borderLeftX = screen.BORDER_LEFT_X;
    const float borderRightX = screen.BORDER_RIGHT_X;
    const float borderTopY = screen.BORDER_TOP_Y;
    const float borderBottomY = screen.BORDER_BOTTOM_Y;

    glColor3f(color.r, color.g, color.b);
    glLineWidth(borderThickness);
    glBegin(GL_LINE_LOOP);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}

void RenderingHandler::DrawBackgroundInBorder(Screen &screen, const Color &color)
{
    float borderThickness = screen.BORDER_THICKNESS;
    float borderLeftX = screen.BORDER_LEFT_X;
    float borderRightX = screen.BORDER_RIGHT_X;
    float borderTopY = screen.BORDER_TOP_Y;
    float borderBottomY = screen.BORDER_BOTTOM_Y;

    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}
