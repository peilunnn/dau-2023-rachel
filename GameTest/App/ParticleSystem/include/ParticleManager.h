#pragma once
#include "ParticleSystem/include/ParticleEmitter.h"
#include "Systems/include/System.h"
#include "Utilities/include/Enums.h"
#include <unordered_map>
using glm::vec2;
using glm::vec3;
using std::vector;
using std::unordered_map;

class ParticleManager : public System
{
public:
    static ParticleManager& GetInstance();
    ParticleManager(const ParticleManager&) = delete;
    void operator=(const ParticleManager&) = delete;

    const int STEAM_PARTICLES_PER_ENEMY = 3;
    
    void Init();
    void Update(float deltaTime);
    void EmitParticles(ParticleType type, const vec3& position, const vec3& velocity);

    const unordered_map<ParticleType, ParticleEmitter>& GetEmitters() const { return m_typeEmitters; };
    vec3 GetPlayerDustEmissionPos();

private:
    ParticleManager() = default;

    const float VELOCITY_MULTIPLIER = 0.25f;
    const float EMISSION_POS_Y_MULTIPLIER = 0.25f;
    const float EMISSION_POS_HORIZONTAL_OFFSET_MULTIPLIER = 0.10f;
    const float EMISSION_POS_VERTICAL_OFFSET_UP_MULTIPLIER = 0.20f;
    const float EMISSION_POS_VERTICAL_OFFSET_DOWN_MULTIPLIER = 0.125f;
    const float EMISSION_Z_POS = 0.0f;

    const int DUST_PARTICLE_POOL_SIZE = 10;
    const int STEAM_PARTICLE_POOL_SIZE = 30;

    const float DUST_PARTICLE_LIFESPAN_DURATION = 0.3f;
    const float STEAM_PARTICLE_LIFESPAN_DURATION = 2.0f;

    const float DUST_PARTICLE_SCALE = 1.0f;
    const float STEAM_PARTICLE_SCALE = 0.1f;

    float m_dustEmissionCooldown = 0.3f;
    float m_dustEmissionTimer = 0.0f;

    unordered_map<ParticleType, CSimpleSprite*> m_typeSprites = {};
    unordered_map<ParticleType, ParticleEmitter> m_typeEmitters = {};
};