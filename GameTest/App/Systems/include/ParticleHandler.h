#pragma once
#include "Systems/include/System.h"
#include "Utilities/include/Enums.h"
#include <unordered_map>
using glm::vec3;
using std::vector;
using std::unordered_map;

class ParticleHandler : public System
{
public:
    static ParticleHandler& GetInstance();
    ParticleHandler(const ParticleHandler&) = delete;
    void operator=(const ParticleHandler&) = delete;

    const float PARTICLE_LIFESPAN_DURATION = 0.4f;
    
    void Init();
    void Update(float deltaTime);
    void ReturnParticleToPool(ParticleType type, EntityId particleEntityId);

    unordered_map<ParticleType, vector<EntityId>> GetActiveParticles() const { return m_activeParticles; }

private:
    ParticleHandler() = default;

    const int m_poolSize = 100;
    float m_emissionCooldown = 0.2f;
    float m_emissionTimer = 0.0f;
    unordered_map<ParticleType, vector<EntityId>> m_particlePools;
    unordered_map<ParticleType, vector<EntityId>> m_activeParticles;

    void InitParticlePool(ParticleType type);
    EntityId GetParticleFromPool(ParticleType type);
    void RemoveActiveParticle(ParticleType type, EntityId particleEntityId);
    vec3 GetEmissionPos();
    void EmitParticle(ParticleType type, const glm::vec3& position, const glm::vec2& velocity);

};

