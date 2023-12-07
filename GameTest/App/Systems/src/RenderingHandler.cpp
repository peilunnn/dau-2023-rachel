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
    GameState gameState = GameManager::GetInstance().GetGameState();
    EntityManager& entityManager = EntityManager::GetInstance();
    Screen& screen = Screen::GetInstance();

    if (gameState == GameState::MainMenu)
        RenderMainMenuScene(entityManager, screen);
    else if (gameState == GameState::Gameplay)
        RenderGameScene(entityManager, screen);
}

void RenderingHandler::RenderMainMenuScene(EntityManager& entityManager, Screen& screen)
{
    SetBackground(screen.R_MAIN_MENU_BG, screen.G_MAIN_MENU_BG, screen.B_MAIN_MENU_BG, screen.ALPHA_MAIN_MENU_BG);

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Renderable>())
    {
        Tag* tag = entityManager.GetComponent<Tag>(entityId);
        
        if (tag->GetScene() != Scene::MainMenu)
            continue;

        CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
        Transform* transform = entityManager.GetComponent<Transform>(entityId);
        sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
        sprite->SetScale(transform->GetScale().x);
        
        if (entityId == entityManager.GetTitleEntityId())
            sprite->SetAngle(transform->GetRotation().z);

        sprite->Draw();
    }

    constexpr char* descriptionText = "Get the highest score in 60 seconds!";
    App::Print(screen.SCREEN_WIDTH - screen.DESCRIPTION_X_OFFSET, screen.SCREEN_HEIGHT - screen.DESCRIPTION_Y_OFFSET, descriptionText, screen.R_TEXT, screen.G_TEXT, screen.B_TEXT);
}

void RenderingHandler::RenderGameScene(EntityManager& entityManager, Screen& screen)
{
    SetBackground(screen.R_GAMEPLAY_BG, screen.G_GAMEPLAY_BG, screen.B_GAMEPLAY_BG, screen.ALPHA_GAMEPLAY_BG);
    DrawBorder(screen, screen.R_BORDER, screen.G_BORDER, screen.B_BORDER);
    DrawBackgroundInBorder(screen, screen.R_BG_IN_BORDER, screen.G_BG_IN_BORDER, screen.B_BG_IN_BORDER);

    for (EntityId entityId : entityManager.GetAmmoEmptyEntities())
        RenderSprite(entityManager, entityId);

    for (EntityId entityId : entityManager.GetAmmoFilledEntities())
        RenderSprite(entityManager, entityId);

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Renderable>())
    {
        Tag* tag = entityManager.GetComponent<Tag>(entityId);

        if (tag->GetScene() != Scene::Gameplay)
            continue;

        if (tag->GetEntityType() != EntityType::AmmoEmpty && tag->GetEntityType() != EntityType::AmmoFilled)
            RenderSprite(entityManager, entityId);
    }

    RenderScore(entityManager);
    RenderTimer(entityManager);
}

void RenderingHandler::RenderSprite(EntityManager &entityManager, EntityId entityId)
{
    Tag* tag = entityManager.GetComponent<Tag>(entityId);
    Transform* transform = entityManager.GetComponent<Transform>(entityId);
    Renderable* renderable = entityManager.GetComponent<Renderable>(entityId);
    CSimpleSprite* sprite = renderable->GetSprite();

    sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    sprite->SetScale(transform->GetScale().x);

    if (tag->GetEntityType() == EntityType::AmmoFilled && !(sprite->GetIsVisible()))
        return;

    sprite->Draw();
}

void RenderingHandler::RenderScore(EntityManager &entityManager)
{
    EntityId scoreEntityId = entityManager.GetScoreEntityId();
    Score* score = entityManager.GetComponent<Score>(scoreEntityId);
    Transform* scoreTransform = entityManager.GetComponent<Transform>(scoreEntityId);

    constexpr float r = 1.0f;
    constexpr float g = 1.0f;
    constexpr float b = 1.0f;

    if (!score)
        return;

    string scoreText = "Score: " + to_string(score->GetScore());
    App::Print(scoreTransform->GetPosition().x, scoreTransform->GetPosition().y, scoreText.c_str(), r, g, b);
}

void RenderingHandler::RenderTimer(EntityManager &entityManager)
{
    EntityId timerEntityId = entityManager.GetTimerEntityId();
    Timer* timer = entityManager.GetComponent<Timer>(timerEntityId);
    Transform* timerTransform = entityManager.GetComponent<Transform>(timerEntityId);

    string timerText = to_string(static_cast<int>(timer->GetCountdownTime()));
    App::Print(timerTransform->GetPosition().x, timerTransform->GetPosition().y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
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
