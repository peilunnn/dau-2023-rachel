#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Utilities/include/Enums.h"
#include "Utilities/include/Particle.h"
#include "Utilities/include/SimpleSprite.h"

using glm::vec3;
using std::vector;

class ParticleEmitter
{
public:
    ParticleEmitter(ParticleType type, size_t poolSize, CSimpleSprite* sharedSprite);
    void Update(float deltaTime);
    void EmitParticle(const vec3& position, const vec3& velocity, float lifetime);
    const vector<Particle*>& GetParticles() const { return m_activeParticles; };

private:
    CSimpleSprite* m_sharedSprite = nullptr;
    ParticleType m_emitterType = ParticleType::None;
    vector<Particle> m_particlePool = vector<Particle>();
    vector<Particle*> m_activeParticles = vector<Particle*>();

    Particle* GetInactiveParticle();
};
