#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"
#include <glm/glm.hpp>
using glm::vec3;

class Particle : public Component
{
public:
    Particle(ParticleType type) : m_particleType(type) {}

    ParticleType GetParticleType() const { return m_particleType; }

private:
    ParticleType m_particleType;
    vec3 m_position = vec3(0.0f);
    vec3 m_velocity = vec3(0.0f);
};
