#pragma once
#include "Systems/include/System.h"
#include "Utilities/include/Enums.h"
#include <glm/glm.hpp>
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

    void Init();
    void Update(float deltaTime);
    void EmitParticle(ParticleType type, const vec3& position);

private:
    ParticleHandler() = default;
    
    const int m_poolSize = 100;
    unordered_map<ParticleType, vector<EntityId>> m_particlePools;

    void InitParticlePool(ParticleType type);
    EntityId GetParticleFromPool(ParticleType type);
    void ReturnParticleToPool(ParticleType type, EntityId particleEntityId);
};

