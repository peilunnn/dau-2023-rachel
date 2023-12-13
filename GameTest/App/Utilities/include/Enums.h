#pragma once

enum class CollisionShape
{
    Sphere,
};

enum class CollisionType
{
    None = 0,
    Player = 1 << 0,         // 0001
    Enemy = 1 << 1,          // 0010
    Bullet = 1 << 2,         // 0100
    ReloadingCircle = 1 << 3 // 1000
};

enum class EntityType
{
    None,
    Player,
    Enemy,
    Bullet,
    ReloadingCircle,
    AmmoEmpty,
    AmmoFilled,
    HealthBar,
    Score,
    Timer,
    Title,
    Button,
    LoadingScreenCharacter,
    Starfield
};

enum class EntityState
{
    Alive,
    HitByEnemy,
    Dead
};

enum class EventType
{
    None,
    PlayerShoot,
    PlayerHitReloadingCircle,
    EnemyHitPlayer,
    BulletHitEnemy,
    BulletOutOfBounds
};

enum PlayerAnimationState
{
    PLAYER_ANIM_FORWARDS,
    PLAYER_ANIM_BACKWARDS,
    PLAYER_ANIM_LEFT,
    PLAYER_ANIM_RIGHT,
    PLAYER_ANIM_IDLE_LEFT,
    PLAYER_ANIM_IDLE_RIGHT,
    PLAYER_ANIM_IDLE_FORWARDS,
    PLAYER_ANIM_IDLE_BACKWARDS
};

enum ReloadingCircleAnimationState
{
    RELOADING_CIRCLE_ANIM_SPIN
};

enum HealthBarAnimationState
{
    HEALTH_100,
    HEALTH_80,
    HEALTH_60,
    HEALTH_40,
    HEALTH_20,
    HEALTH_0,
};

enum class GameState
{
    None,
    MainMenu,
    Gameplay,
    GameOver,
    Loading,
    Paused
};

enum class TimerType {
    None,
    Countdown,
    PlayerDeath
};