#pragma once

enum class CollisionShape
{
	SPHERE,
	CAPSULE,
};

enum class CollisionType
{
	NONE = 0,
	PLAYER = 1 << 0,          // 0001
	ENEMY = 1 << 1,           // 0010
	BULLET = 1 << 2,          // 0100
	RELOADING_CIRCLE = 1 << 3 // 1000
};

enum class EntityType
{
	NONE,
	PLAYER,
	ENEMY,
	BULLET,
	RELOADING_CIRCLE,
	AMMO_EMPTY,
	AMMO_FILLED,
	HEALTH_BAR,
	SCORE,
	TIMER
};

enum class EntityState
{
	// For player
	ALIVE,
	HIT_BY_ENEMY,
	// For enemies
	HIT_BY_BULLET,
};

enum PlayerAnimationStates
{
	PLAYER_ANIM_FORWARDS,
	PLAYER_ANIM_BACKWARDS,
	PLAYER_ANIM_LEFT,
	PLAYER_ANIM_RIGHT,
	PLAYER_ANIM_IDLE_LEFT,
	PLAYER_ANIM_IDLE_RIGHT,
	PLAYER_ANIM_IDLE_FORWARDS,
	PLAYER_ANIM_IDLE_BACKWARDS,
};
enum EnemyAnimationStates
{
	ENEMY_ANIM_IDLE,
	ENEMY_ANIM_MELT,
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

enum class EventType
{
	PlayerShoot,
	BulletHitEnemy,
	EnemyHitPlayer,
	PlayerHitReloadingCircle,
	PlayerHealthReachZero,
	CountdownReachZero
};

enum class SystemType
{
	AnimationHandler,
	CollisionHandler,
	HealthHandler,
	InputHandler,
	MovementHandler,
	ShootingHandler,
	RenderingHandler,
	ScreenHandler,
	ScoreHandler,
	TimerHandler
};