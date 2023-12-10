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
        RenderLoadingScreen(entityManager, screen);
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

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Renderable>())
    {
        Tag *tag = entityManager.GetComponent<Tag>(entityId);

        if (tag->GetCurrentGameState() != GameState::MainMenu)
            continue;

        CSimpleSprite *sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
        Transform *transform = entityManager.GetComponent<Transform>(entityId);
        sprite->SetPosition(transform->GetPosition().x, transform->GetPosition().y);
        sprite->SetScale(transform->GetScale().x);

        if (entityId == entityManager.GetTitleEntityId())
            sprite->SetAngle(transform->GetRotation().z);

        sprite->Draw();
    }

    const float descriptionXOffset = 600.0f;
    const float descriptionYOffset = 350.0f;
    constexpr char *descriptionText = "Get the highest score in 60 seconds!";
    App::Print(screen.SCREEN_WIDTH - descriptionXOffset, screen.SCREEN_HEIGHT - descriptionYOffset, descriptionText, WHITE.r, WHITE.g, WHITE.b);
}

void RenderingHandler::RenderGameplayScene(EntityManager &entityManager, Screen &screen)
{
    float shakeOffsetX = 0.0f;
    float shakeOffsetY = 0.0f;
    if (m_shakeTimer > 0.0f)
    {
        float shakeOffsetX = (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * m_shakeIntensity;
        float shakeOffsetY = (static_cast<float>(rand()) / RAND_MAX * 2 - 1) * m_shakeIntensity;

        glPushMatrix();
        glTranslatef(shakeOffsetX, shakeOffsetY, 0.0f);
    }

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

        if (tag->GetCurrentGameState() != GameState::Gameplay)
            continue;

        if (tag->GetEntityType() != EntityType::AmmoEmpty && tag->GetEntityType() != EntityType::AmmoFilled)
            RenderSprite(entityManager, entityId);
    }

    RenderScore(entityManager);
    RenderCountdownTimer(entityManager);
    RenderFadeOverlay();

    if (m_shakeTimer > 0.0f)
        glPopMatrix();
}

void RenderingHandler::RenderGameOverScene(EntityManager &entityManager, Screen &screen)
{
    RenderStarfield(entityManager);

    EntityId scoreEntityId = entityManager.GetScoreEntityId();
    Score *score = entityManager.GetComponent<Score>(scoreEntityId);
    const float gameOverTextXOffset = 545.0f;
    const float gameOverTextYOffset = 300.0f;
    const float gameOverTextX = screen.SCREEN_WIDTH - gameOverTextXOffset;
    const float gameOverTextY = screen.SCREEN_HEIGHT - gameOverTextYOffset;
    constexpr char *gameOverText = "Game Over";
    App::Print(gameOverTextX, gameOverTextY, gameOverText, WHITE.r, WHITE.g, WHITE.b);

    const float scoreTextXOffset = 570.0f;
    const float scoreTextYOffset = 350.0f;
    const float scoreTextX = screen.SCREEN_WIDTH - scoreTextXOffset;
    const float scoreTextY = screen.SCREEN_HEIGHT - scoreTextYOffset;
    string scoreMessage = "You got a score of : " + to_string(score->GetScore());
    App::Print(scoreTextX, scoreTextY, scoreMessage.c_str(), WHITE.r, WHITE.g, WHITE.b);

    EntityId backButtonEntityId = entityManager.GetBackButtonEntityId();
    CSimpleSprite *backButtonSprite = entityManager.GetComponent<Renderable>(backButtonEntityId)->GetSprite();
    Transform *backButtonTransform = entityManager.GetComponent<Transform>(backButtonEntityId);
    backButtonSprite->SetPosition(backButtonTransform->GetPosition().x, backButtonTransform->GetPosition().y);
    backButtonSprite->SetScale(backButtonTransform->GetScale().x);
    backButtonSprite->Draw();
}

void RenderingHandler::RenderLoadingScreen(EntityManager &entityManager, Screen &screen)
{
    SetBackground(BLACK);

    EntityId loadingScreenCharacterEntityId = entityManager.GetLoadingScreenCharacterEntityId();
    CSimpleSprite *loadingScreenCharacterSprite = entityManager.GetComponent<Renderable>(loadingScreenCharacterEntityId)->GetSprite();
    Transform *loadingScreenCharacterTransform = entityManager.GetComponent<Transform>(loadingScreenCharacterEntityId);
    loadingScreenCharacterSprite->SetPosition(loadingScreenCharacterTransform->GetPosition().x, loadingScreenCharacterTransform->GetPosition().y);
    loadingScreenCharacterSprite->SetScale(loadingScreenCharacterTransform->GetScale().x);
    loadingScreenCharacterSprite->Draw();

    const float loadingTextXOffset = 540.0f;
    const float loadingTextYOffset = 400.0f;
    const float loadingTextX = screen.SCREEN_WIDTH - loadingTextXOffset;
    const float loadingTextY = screen.SCREEN_HEIGHT - loadingTextYOffset;
    constexpr char *loadingText = "Loading...";

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

void RenderingHandler::SetBackground(const Color &color)
{
    glClearColor(color.r, color.g, color.b, color.alpha);
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

void RenderingHandler::RenderFadeOverlay()
{
    Screen& screen = Screen::GetInstance();

    if (m_fadeAmount == 0.0f)
        return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, m_fadeAmount);
    glBegin(GL_QUADS);
    glVertex2f(screen.SCREEN_LEFT_NDC, screen.SCREEN_BOTTOM_NDC);
    glVertex2f(screen.SCREEN_RIGHT_NDC, screen.SCREEN_BOTTOM_NDC);
    glVertex2f(screen.SCREEN_RIGHT_NDC, screen.SCREEN_TOP_NDC);
    glVertex2f(screen.SCREEN_LEFT_NDC, screen.SCREEN_TOP_NDC);
    glEnd();
}

void RenderingHandler::SetUpScreenShake()
{
    const float minDuration = 1.0f;
    const float maxDuration = 2.0f;
    m_shakeDuration = minDuration + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxDuration - minDuration)));

    const float minIntensity = 5.0f;
    const float maxIntensity = 20.0f;
    m_shakeIntensity = minIntensity + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxIntensity - minIntensity)));

    m_shakeTimer = m_shakeDuration;
}

void RenderingHandler::HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime)
{
    SetUpScreenShake();
}
