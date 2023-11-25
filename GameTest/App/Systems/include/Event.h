#ifndef EVENT_H
#define EVENT_H

#include <vector>

using Entity = unsigned int;

enum class EventType {
    PlayerShoot,
    BulletHitEnemy,
    EnemyHitPlayer,
    PlayerHitReloadingCircle,
    PlayerHealthReachZero,
    CountdownReachZero
};

class Event {
public:
    EventType eventType;
    std::vector<Entity> entities;

    Event(EventType type, std::initializer_list<Entity> ents, bool newEnemies = false)
        : eventType(type), entities(ents) {}
};

#endif // EVENT_H
