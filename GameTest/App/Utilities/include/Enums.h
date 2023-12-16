#pragma once

enum class CollisionShape
{
    Sphere,
};

enum class EntityType
{
    None,
    Player,
    Enemy,
    Bullet,
    AmmoPickup,
    AmmoEmpty,
    AmmoFilled,
    HealthBar,
    Score,
    Timer,
    Title,
    Button,
    LoadingScreenCharacter,
    Starfield,
    Crosshair
};

enum class EntityState
{
    Alive,
    HitByEnemy,
    HitByBullet,
    Dead
};

enum class EventType
{
    None,
    PlayerHitAmmoPickup,
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

enum EnemyAnimationState
{
    ENEMY_ANIM_IDLE,
    ENEMY_ANIM_MELT,
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

enum class TimerType
{
    None,
    Countdown,
    PlayerDeath,
    EnemyMelt
};