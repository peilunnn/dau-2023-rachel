#pragma once

enum class CollisionShape {
    Sphere,
};

enum class CollisionType {
    None = 0,
    Player = 1 << 0,          // 0001
    Enemy = 1 << 1,           // 0010
    Bullet = 1 << 2,          // 0100
    ReloadingCircle = 1 << 3  // 1000
};

enum class EntityType {
    None,
    Player,
    Enemy,
    Bullet,
    ReloadingCircle,
    AmmoEmpty,
    AmmoFilled,
    HealthBar,
    Score,
    Timer
};

enum class EntityState {
    Alive,
    HitByEnemy,
    HitByBullet
};

enum PlayerAnimationStates {
	PLAYER_ANIM_FORWARDS,
	PLAYER_ANIM_BACKWARDS,
	PLAYER_ANIM_LEFT,
	PLAYER_ANIM_RIGHT,
	PLAYER_ANIM_IDLE_LEFT,
	PLAYER_ANIM_IDLE_RIGHT,
	PLAYER_ANIM_IDLE_FORWARDS,
    PLAYER_ANIM_IDLE_BACKWARDS
};

enum EnemyAnimationStates {
	ENEMY_ANIM_IDLE,
    ENEMY_ANIM_MELT
};

enum ReloadingCircleAnimationStates
{
	RELOADING_CIRCLE_ANIM_SPIN
};

enum HealthBarAnimationStates
{
	HEALTH_100,
	HEALTH_80,
	HEALTH_60,
	HEALTH_40,
	HEALTH_20,
	HEALTH_0,
};