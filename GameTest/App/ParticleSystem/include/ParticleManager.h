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

    const int STEAM_PARTICLES_PER_ENEMY = 5;
    
    void Init();
    void Update(float deltaTime);
    void EmitParticles(ParticleType type, const vec3& position, const vec3& velocity);
    // void ReturnParticleToPool(ParticleType type, EntityId particleEntityId);
    // vec3 GetEmissionPos(EntityType entityType, EntityId entityId);
    // void EmitParticle(ParticleType type, const vec3& position, const vec2& velocity);

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

    const float DUST_PARTICLE_LIFESPAN_DURATION = 0.4f;
    const float STEAM_PARTICLE_LIFESPAN_DURATION = 2.0f;

    //float m_emissionCooldown = 0.2f;
    //float m_emissionTimer = 0.0f;
    //unordered_map<ParticleType, vector<EntityId>> m_particlePools;
    //unordered_map<ParticleType, vector<EntityId>> m_activeParticles;

    // void InitParticlePool(ParticleType type);
    //EntityId GetParticleFromPool(ParticleType type);
    //void RemoveActiveParticle(ParticleType type, EntityId particleEntityId);

    unordered_map<ParticleType, CSimpleSprite*> m_typeSprites = {};
    unordered_map<ParticleType, ParticleEmitter> m_typeEmitters = {};
};