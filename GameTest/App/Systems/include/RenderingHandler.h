#pragma once
#include "Components/include/Screen.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Components/include/Color.h"

class RenderingHandler : public System 
{
public:
    static RenderingHandler& GetInstance();
    RenderingHandler(RenderingHandler const&) = delete;
    void operator=(RenderingHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    void Render();
    void ShowAllAmmoFilledEntities();
    void HideAmmoFilledEntity(int index);
    void UpdateFade(float deltaTime);
    void ResetFade();
    void UpdateScreenShakeTimer(float deltaTime);

private:
    RenderingHandler()
    {
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
        m_subscribedEvents.insert(EventType::PlayerHitAmmoPickup);
    }

    const float FADE_RATE = 0.5f;
    const float MAX_SHAKE_DURATION = 1.5f;
    const float MAX_SHAKE_INTENSITY = 30.0f;
    const float MIN_SHAKE_DURATION = 1.0f;
    const float MIN_SHAKE_INTENSITY = 20.0f;
    float m_fadeAmount = 0.0f;
    float m_shakeDuration = 0.0f;
    float m_shakeIntensity = 0.0f;
    float m_shakeTimer = 0.0f;

    const float DESCRIPTION_X_OFFSET = 600.0f;
    const float DESCRIPTION_Y_OFFSET = 350.0f;
    const float GAME_OVER_TEXT_X_OFFSET = 545.0f;
    const float GAME_OVER_TEXT_Y_OFFSET = 300.0f;
    const float LOADING_TEXT_X_OFFSET = 540.0f;
    const float LOADING_TEXT_Y_OFFSET = 400.0f;
    const float SCORE_TEXT_X_OFFSET = 570.0f;
    const float SCORE_TEXT_Y_OFFSET = 350.0f;

    const char* DESCRIPTION_TEXT = "Get the highest score in 60 seconds!";
    const char* GAME_OVER_TEXT = "Game Over";
    const char* LOADING_TEXT = "Loading...";
    const char* SCORE_TEXT_PREFIX = "You got a score of : ";

    void RenderMainMenuScene(EntityManager& entityManager, Screen& screen);
    void RenderGameplayScene(EntityManager& entityManager, Screen& screen);
    void RenderGameOverScene(EntityManager& entityManager, Screen& screen);
    void RenderLoadingScene(EntityManager& entityManager, Screen& screen);
    void RenderPauseScene(EntityManager& entityManager, Screen& screen);
    void RenderObjects(EntityManager& entityManager, GameState gameState);
    void RenderAmmo();
    void RenderDescriptionText(Screen& screen);
    void RenderLoadingText(Screen& screen);
    void RenderGameOverText(Screen& screen);
    void RenderGameOverScoreText(EntityManager& entityManager, Screen& screen);
    void RenderSprite(EntityManager& entityManager, EntityId entityId);
    void RenderScore(EntityManager& entityManager);
    void RenderCountdownTimer(EntityManager& entityManager);
    void SetBackground(const Color& color, float alpha = 1.0f);
    void DrawBorder(Screen& screen, const Color& color);
    void DrawBackgroundInBorder(Screen& screen, const Color& color);
    void RenderOverlay(Screen& screen, float alpha);
    void RenderFadeOverlay(Screen& screen);
    void RenderTransluscentOverlay(Screen& screen);
    void SetUpScreenShake();
    void ApplyScreenShake();
    void HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
    void HandlePlayerHitAmmoPickup();
};
