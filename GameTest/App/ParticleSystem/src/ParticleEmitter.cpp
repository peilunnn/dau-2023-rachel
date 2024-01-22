#include "stdafx.h"
#include "ParticleSystem/include/ParticleEmitter.h"
using std::remove_if;

ParticleEmitter::ParticleEmitter(ParticleType type, size_t poolSize, CSimpleSprite* sharedSprite)
    : m_emitterType(type), m_sharedSprite(sharedSprite)
{
    m_particlePool.resize(poolSize);

    for (auto& particle : m_particlePool)
        particle.sprite = sharedSprite;
}

void ParticleEmitter::Update(float deltaTime)
{
    // Update position and age
    for (auto* particle : m_activeParticles) 
    {
        particle->position += particle->velocity * deltaTime;
        particle->age += deltaTime;

        if (particle->age >= particle->lifetime)
            particle->active = false;
    }

    // Remove all inactive particles
    m_activeParticles.erase(
        remove_if(m_activeParticles.begin(), m_activeParticles.end(),
            [](const Particle* p) { return !p->active; }),
        m_activeParticles.end()
    );
}

void ParticleEmitter::EmitParticle(const vec3& position, const vec3& velocity, float lifetime)
{
    Particle* particle = GetInactiveParticle();
    
    if (!particle)
        return;

    particle->position = position;
    particle->velocity = velocity;
    particle->lifetime = lifetime;
    particle->age = 0.0f;
    particle->type = m_emitterType;
    particle->active = true;
    m_activeParticles.push_back(particle);
}

Particle* ParticleEmitter::GetInactiveParticle()
{
    for (Particle& particle : m_particlePool)
    {
        if (!particle.active)
            return &particle;
    }
    return nullptr;
}

