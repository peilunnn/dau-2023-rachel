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
    set<string> GetSubscribedEvents() const override
    {
        return m_subscribedEvents;
    }

    void Render();
    void HideAmmoFilledEntity(int index);
    void ShowAllAmmoFilledEntities();
    void UpdateFade(float deltaTime);
    void ResetFade();
    void UpdateScreenShakeTimer(float deltaTime);

private:
    RenderingHandler() {
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    set<string> m_subscribedEvents = {};
    float m_fadeAmount = 0.0f;
    float m_shakeDuration = 0.0f;
    float m_shakeIntensity = 0.0f;
    float m_shakeTimer = 0.0f;

    void RenderMainMenuScene(EntityManager& entityManager, Screen& screen);
    void RenderGameplayScene(EntityManager& entityManager, Screen& screen);
    void RenderGameOverScene(EntityManager& entityManager, Screen& screen);
    void RenderLoadingScreen(EntityManager& entityManager, Screen& screen);
    void RenderSprite(EntityManager& entityManager, EntityId entityId);
    void RenderScore(EntityManager& entityManager);
    void RenderCountdownTimer(EntityManager& entityManager);
    void RenderStarfield(EntityManager& entityManager);
    void SetBackground(const Color& color);
    void DrawBorder(Screen& screen, const Color& color);
    void DrawBackgroundInBorder(Screen& screen, const Color& color);
    void RenderFadeOverlay();
    void SetUpScreenShake();
    void HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
};
