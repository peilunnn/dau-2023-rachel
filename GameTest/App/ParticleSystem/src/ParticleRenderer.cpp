#include "stdafx.h"
#include "ParticleSystem/include/ParticleRenderer.h"

ParticleRenderer& ParticleRenderer::GetInstance()
{
    static ParticleRenderer instance;
    return instance;
}

void ParticleRenderer::RenderParticles(const ParticleManager& particleManager)
{
    for (const auto& typeEmitter : particleManager.GetEmitters()) 
    {
        const ParticleEmitter& emitter = typeEmitter.second;

        for (const Particle* particle : emitter.GetActiveParticles()) 
        {
            if (particle->active)
                RenderParticle(particle);
        }
    }
}

void ParticleRenderer::RenderParticle(const Particle* particle)
{
    CSimpleSprite* sprite = particle->sprite;
    sprite->SetPosition(particle->position.x, particle->position.y);
    sprite->SetScale(particle->scale);
    sprite->Draw();
}