#pragma once
#include "Systems/include/System.h"

class SoundHandler : public System
{
public:
    static SoundHandler& GetInstance() {
        static SoundHandler instance;
        return instance;
    }
    SoundHandler(SoundHandler const&) = delete;
    void operator=(SoundHandler const&) = delete;

private:
    SoundHandler() {
        m_subscribedEvents.insert("PlayerShoot");
        m_subscribedEvents.insert("PlayerHitReloadingCircle");
        m_subscribedEvents.insert("EnemyHitPlayer");
        m_subscribedEvents.insert("BulletHitEnemy");

    }

    set<string> m_subscribedEvents = {};
};

