#ifndef SYSTEM_EVENT_H
#define SYSTEM_EVENT_H

using Entity = unsigned int;

struct SystemEvent {
    enum EventType { BulletHitEnemy, PlayerHitEnemy, PlayerHitReloadingCircle, PlayerHealthReachZero, CountdownReachZero } type;
    Entity entity1;
    Entity entity2;
    bool newEnemiesCreated = false;
};

#endif // SYSTEM_EVENT_H
