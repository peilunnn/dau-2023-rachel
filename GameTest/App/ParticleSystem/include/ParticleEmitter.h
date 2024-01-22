#pragma once
#include "Utilities/include/Enums.h"
#include "Utilities/include/Particle.h"
#include "Utilities/include/SimpleSprite.h"
#include <glm/glm.hpp>
#include <vector>

using glm::vec3;
using std::vector;

class ParticleEmitter
{
public:
    ParticleEmitter(ParticleType type, size_t poolSize, CSimpleSprite* sharedSprite);
    void Update(float deltaTime);
    void EmitParticle(const vec3& position, float scale, const vec3& velocity, float lifetime);
    const vector<Particle*>& GetActiveParticles() const { return m_activeParticles; };

private:
    CSimpleSprite* m_sharedSprite = nullptr;
    ParticleType m_emitterType = ParticleType::None;
    vector<Particle> m_particlePool = vector<Particle>();
    vector<Particle*> m_activeParticles = vector<Particle*>();

    Particle* GetInactiveParticle();
};
