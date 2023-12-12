#include "stdafx.h"
#include "Components/include/Renderable.h"
#include "Components/include/Score.h"
#include "Components/include/Timer.h"
#include "Components/include/Transform.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/RenderingHandler.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"

RenderingHandler &RenderingHandler::GetInstance()
{
    static RenderingHandler instance;
    return instance;
}

void RenderingHandler::HandleEvent(const Event& event, float deltaTime)
{
    EntityManager& entityManager = EntityManager::GetInstance();

    if (event.GetEventType() == "EnemyHitPlayer")
    {
        HandleEnemyHitPlayer(entityManager, deltaTime);
    }
}

void RenderingHandler::Render()
{
    GameState m_currentGameState = GameManager::GetInstance().GetCurrentGameState();
    EntityManager &entityManager = EntityManager::GetInstance();
    Screen &screen = Screen::GetInstance();

    switch (m_currentGameState)
    {
        case GameState::MainMenu:
            RenderMainMenuScene(entityManager, screen);
            break;
        case GameState::Gameplay:
            RenderGameplayScene(entityManager, screen);
            break;
        case GameState::GameOver:
            RenderGameOverScene(entityManager, screen);
            break;
        case GameState::Loading:
            RenderLoadingScene(entityManager, screen);
            break;
        case GameState::Paused:
            RenderPauseScene(entityManager, screen);
            break;
    }
}

void RenderingHandler::HideAmmoFilledEntity(int index)
{
    EntityManager& entityManager = EntityManager::GetInstance();
    vector<EntityId> ammoFilledEntityIds = entityManager.GetAmmoFilledEntityIds();

    if (index >= 0 && index < ammoFilledEntityIds.size())
    {
        CSimpleSprite* ammoFilledSprite = entityManager.GetComponent<Renderable>(ammoFilledEntityIds[index])->GetSprite();

        if (!ammoFilledSprite)
            return;

        ammoFilledSprite->SetVisible(false);
    }
}

void RenderingHandler::ShowAllAmmoFilledEntities()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    vector<EntityId> ammoFilledEntityIds = entityManager.GetAmmoFilledEntityIds();

    for (int i = 0; i < ammoFilledEntityIds.size(); i++)
    {
        CSimpleSprite* ammoFilledSprite = entityManager.GetComponent<Renderable>(ammoFilledEntityIds[i])->GetSprite();
        ammoFilledSprite->SetVisible(true);
    }
}

void RenderingHandler::UpdateFade(float deltaTime)
{
    const float fadeRate = 0.5f;
    m_fadeAmount += deltaTime * fadeRate;
}

void RenderingHandler::ResetFade()
{
    m_fadeAmount = 0;
}

void RenderingHandler::UpdateScreenShakeTimer(float deltaTime)
{
    if (m_shakeTimer <= 0.0f)
        return;

    m_shakeTimer -= deltaTime;
}

void RenderingHandler::RenderMainMenuScene(EntityManager &entityManager, Screen &screen)
{
    RenderStarfield(entityManager);
    RenderObjects(entityManager, GameState::MainMenu);
    RenderDescriptionText(screen);
}

void RenderingHandler::RenderGameplayScene(EntityManager &entityManager, Screen &screen)
{
    ApplyScreenShake();

    SetBackground(BLACK);
    DrawBorder(screen, WHITE);
    DrawBackgroundInBorder(screen, DARK_GREY);

    RenderAmmo();
    RenderCountdownTimer(entityManager);
    RenderScore(entityManager);
    RenderObjects(entityManager, GameState::Gameplay);

    RenderFadeOverlay(screen);

    if (m_shakeTimer > 0.0f)
        glPopMatrix();
}

void RenderingHandler::RenderGameOverScene(EntityManager &entityManager, Screen &screen)
{
    RenderStarfield(entityManager);
    RenderGameOverText(screen);
    RenderGameOverScoreText(entityManager, screen);
    RenderSprite(entityManager, entityManager.GetBackButtonEntityId());
}

void RenderingHandler::RenderLoadingScene(EntityManager &entityManager, Screen &screen)
{
    SetBackground(BLACK);
    RenderSprite(entityManager, entityManager.GetLoadingScreenCharacterEntityId());
    RenderLoadingText(screen);
}

void RenderingHandler::RenderPauseScene(EntityManager& entityManager, Screen& screen)
{
    RenderGameplayScene(entityManager, screen);
    RenderTransluscentOverlay(screen);
    RenderObjects(entityManager, GameState::Paused);
}

void RenderingHandler::RenderObjects(EntityManager& entityManager, GameState gameState)
{
    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Renderable>())
    {
        Tag* tag = entityManager.GetComponent<Tag>(entityId);

        if (tag->GetCurrentGameState() != gameState)
            continue;

        if (tag->GetEntityType() == EntityType::AmmoEmpty || tag->GetEntityType() == EntityType::AmmoFilled)
            continue;

        RenderSprite(entityManager, entityId);
    }
}

void RenderingHandler::RenderAmmo()
{
    EntityManager& entityManager = EntityManager::GetInstance();

    for (EntityId entityId : entityManager.GetAmmoEmptyEntityIds())
        RenderSprite(entityManager, entityId);
    
    for (EntityId entityId : entityManager.GetAmmoFilledEntityIds())
        RenderSprite(entityManager, entityId);
}

void RenderingHandler::RenderDescriptionText(Screen& screen)
{
    const float descriptionXOffset = 600.0f;
    const float descriptionYOffset = 350.0f;
    constexpr char* descriptionText = "Get the highest score in 60 seconds!";
    App::Print(screen.SCREEN_WIDTH - descriptionXOffset, screen.SCREEN_HEIGHT - descriptionYOffset, descriptionText, WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderLoadingText(Screen& screen)
{
    const float loadingTextXOffset = 540.0f;
    const float loadingTextYOffset = 400.0f;
    const float loadingTextX = screen.SCREEN_WIDTH - loadingTextXOffset;
    const float loadingTextY = screen.SCREEN_HEIGHT - loadingTextYOffset;
    constexpr char* loadingText = "Loading...";
    App::Print(loadingTextX, loadingTextY, loadingText, WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderGameOverText(Screen& screen)
{
    const float gameOverTextXOffset = 545.0f;
    const float gameOverTextYOffset = 300.0f;
    const float gameOverTextX = screen.SCREEN_WIDTH - gameOverTextXOffset;
    const float gameOverTextY = screen.SCREEN_HEIGHT - gameOverTextYOffset;
    constexpr char* gameOverText = "Game Over";
    App::Print(gameOverTextX, gameOverTextY, gameOverText, WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderGameOverScoreText(EntityManager& entityManager, Screen& screen)
{
    EntityId scoreEntityId = entityManager.GetScoreEntityId();
    Score* score = entityManager.GetComponent<Score>(scoreEntityId);
    const float scoreTextXOffset = 570.0f;
    const float scoreTextYOffset = 350.0f;
    const float scoreTextX = screen.SCREEN_WIDTH - scoreTextXOffset;
    const float scoreTextY = screen.SCREEN_HEIGHT - scoreTextYOffset;
    string scoreMessage = "You got a score of : " + to_string(score->GetScore());
    App::Print(scoreTextX, scoreTextY, scoreMessage.c_str(), WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderSprite(EntityManager &entityManager, EntityId entityId)
{
    Tag *tag = entityManager.GetComponent<Tag>(entityId);
    Transform *transform = entityManager.GetComponent<Transform>(entityId);
    Renderable *renderable = entityManager.GetComponent<Renderable>(entityId);
    CSimpleSprite *sprite = renderable->GetSprite();

    sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
    sprite->SetScale(transform->GetScale().x);

    if (entityId == entityManager.GetTitleEntityId())
        sprite->SetAngle(transform->GetRotation().z);

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

void RenderingHandler::RenderCountdownTimer(EntityManager &entityManager)
{
    EntityId countdownTimerEntityId = entityManager.GetCountdownTimerEntityId();
    Timer *timer = entityManager.GetComponent<Timer>(countdownTimerEntityId);
    Transform *timerTransform = entityManager.GetComponent<Transform>(countdownTimerEntityId);

    string timerText = to_string(static_cast<int>(timer->GetRemainingTime()));
    App::Print(timerTransform->GetPosition().x, timerTransform->GetPosition().y, timerText.c_str(), 1.0f, 1.0f, 1.0f);
}

void RenderingHandler::RenderStarfield(EntityManager &entityManager)
{
    EntityId starfieldEntityId = entityManager.GetStarfieldEntityId();
    CSimpleSprite *starfieldSprite = entityManager.GetComponent<Renderable>(starfieldEntityId)->GetSprite();
    Transform *starfieldTransform = entityManager.GetComponent<Transform>(starfieldEntityId);
    starfieldSprite->SetPosition(starfieldTransform->GetPosition().x, starfieldTransform->GetPosition().y);
    starfieldSprite->SetScale(starfieldTransform->GetScale().x);
    starfieldSprite->Draw();
}

void RenderingHandler::SetBackground(const Color& color, float alpha)
{
    glClearColor(color.r, color.g, color.b, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderingHandler::DrawBorder(Screen &screen, const Color &color)
{
    const float borderThickness = screen.BORDER_THICKNESS;
    const float borderLeftX = screen.BORDER_LEFT_NDC;
    const float borderRightX = screen.BORDER_RIGHT_NDC;
    const float borderTopY = screen.BORDER_TOP_NDC;
    const float borderBottomY = screen.BORDER_BOTTOM_NDC;

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
    float borderLeftX = screen.BORDER_LEFT_NDC;
    float borderRightX = screen.BORDER_RIGHT_NDC;
    float borderTopY = screen.BORDER_TOP_NDC;
    float borderBottomY = screen.BORDER_BOTTOM_NDC;

    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(borderLeftX, borderTopY);
    glVertex2f(borderRightX, borderTopY);
    glVertex2f(borderRightX, borderBottomY);
    glVertex2f(borderLeftX, borderBottomY);
    glEnd();
}

void RenderingHandler::RenderOverlay(Screen& screen, float alpha)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (alpha <= 0.0f)
        return;

    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_QUADS);
    glVertex2f(screen.SCREEN_LEFT_NDC, screen.SCREEN_BOTTOM_NDC);
    glVertex2f(screen.SCREEN_RIGHT_NDC, screen.SCREEN_BOTTOM_NDC);
    glVertex2f(screen.SCREEN_RIGHT_NDC, screen.SCREEN_TOP_NDC);
    glVertex2f(screen.SCREEN_LEFT_NDC, screen.SCREEN_TOP_NDC);
    glEnd();

    glDisable(GL_BLEND);
}

void RenderingHandler::RenderFadeOverlay(Screen& screen)
{
    RenderOverlay(screen, m_fadeAmount);
}

void RenderingHandler::RenderTransluscentOverlay(Screen& screen)
{
    const float overlayAlpha = 0.5f;
    RenderOverlay(screen, overlayAlpha);
}

void RenderingHandler::SetUpScreenShake() {
    m_shakeDuration = MIN_SHAKE_DURATION + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_SHAKE_DURATION - MIN_SHAKE_DURATION)));

    m_shakeIntensity = MIN_SHAKE_INTENSITY + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (MAX_SHAKE_INTENSITY - MIN_SHAKE_INTENSITY)));

    m_shakeTimer = m_shakeDuration;
}

void RenderingHandler::ApplyScreenShake() {
    if (m_shakeTimer <= 0.0f)
        return;
    
    float shakeOffsetX = (rand() / static_cast<float>(RAND_MAX) * 2 - 1) * m_shakeIntensity;
    float shakeOffsetY = (rand() / static_cast<float>(RAND_MAX) * 2 - 1) * m_shakeIntensity;
    glPushMatrix();
    glTranslatef(shakeOffsetX, shakeOffsetY, 0.0f);
}

void RenderingHandler::HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime)
{
    SetUpScreenShake();
}
