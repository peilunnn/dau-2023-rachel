#pragma once
#include "Utilities/include/Enums.h"
#include <glm/glm.hpp>
using glm::vec3;

struct Particle
{
    CSimpleSprite* sprite;
    vec3 position;
    vec3 velocity;
    float lifetime;
    float age = 0.0f;
    ParticleType type;
    bool active = false;
};