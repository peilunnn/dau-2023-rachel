#pragma once
#include "Components/include/Screen.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/Color.h"

class RenderingHandler : public System {
public:
    static RenderingHandler& GetInstance();
    RenderingHandler(RenderingHandler const&) = delete;
    void operator=(RenderingHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    void Render();
    void HideAmmoFilledEntity(int index);
    void ShowAllAmmoFilledEntities();
    void UpdateFade(float deltaTime);
    void ResetFade();
    void UpdateScreenShakeTimer(float deltaTime);

private:
    RenderingHandler() {
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
    }

    const float FADE_RATE = 0.5f;
    float m_fadeAmount = 0.0f;
    float m_shakeDuration = 0.0f;
    float m_shakeIntensity = 0.0f;
    float m_shakeTimer = 0.0f;
    const float MIN_SHAKE_DURATION = 1.0f;
    const float MAX_SHAKE_DURATION = 2.0f;
    const float MIN_SHAKE_INTENSITY = 5.0f;
    const float MAX_SHAKE_INTENSITY = 20.0f;

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
    void RenderStarfield(EntityManager& entityManager);
    void SetBackground(const Color& color, float alpha = 1.0f);
    void DrawBorder(Screen& screen, const Color& color);
    void DrawBackgroundInBorder(Screen& screen, const Color& color);
    void RenderOverlay(Screen& screen, float alpha);
    void RenderFadeOverlay(Screen& screen);
    void RenderTransluscentOverlay(Screen& screen);
    void SetUpScreenShake();
    void ApplyScreenShake();
    void HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
};
