#pragma once
#include "ParticleSystem/include/ParticleManager.h"

class ParticleRenderer
{
public:
	static ParticleRenderer& GetInstance();
	ParticleRenderer(const ParticleRenderer&) = delete;
	void operator=(const ParticleRenderer&) = delete;

	void RenderParticles(const ParticleManager& particleManager);

private:
	ParticleRenderer() = default;

	void RenderParticle(const Particle* particle);
};

