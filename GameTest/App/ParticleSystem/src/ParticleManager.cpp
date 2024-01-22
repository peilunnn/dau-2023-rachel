#include "stdafx.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Renderable.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/SpriteManager.h"
#include "ParticleSystem/include/ParticleManager.h"
#include "Utilities/include/Helper.h"
using std::find;

ParticleManager& ParticleManager::GetInstance() 
{
    static ParticleManager instance;
    return instance;
}

void ParticleManager::Init() 
{
     SpriteManager& spriteManager = SpriteManager::GetInstance();
     m_typeSprites[ParticleType::Dust] = spriteManager.CreateSprite(Helper::PATH_TO_DUST_PARTICLE, 1, 1);
     m_typeSprites[ParticleType::Steam] = spriteManager.CreateSprite(Helper::PATH_TO_STEAM_PARTICLE, 1, 1);

     m_typeEmitters.try_emplace(ParticleType::Dust, ParticleType::Dust, DUST_PARTICLE_POOL_SIZE, m_typeSprites[ParticleType::Dust]);
     m_typeEmitters.try_emplace(ParticleType::Steam, ParticleType::Steam, STEAM_PARTICLE_POOL_SIZE, m_typeSprites[ParticleType::Steam]);
}

void ParticleManager::Update(float deltaTime) 
{
    m_dustEmissionTimer += deltaTime;

    for (auto& typeEmitter : m_typeEmitters)
    {
        ParticleEmitter& emitter = typeEmitter.second;
        emitter.Update(deltaTime);
    }

    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    Velocity* playerVelocityComponent = entityManager.GetComponent<Velocity>(playerEntityId);
    Tag* playerTag = entityManager.GetComponent<Tag>(playerEntityId);

    if (playerTag->GetEntityState() == EntityState::Dead)
        return;

    if (length(playerVelocityComponent->GetVelocity()) > 0.0f && m_dustEmissionTimer >= m_dustEmissionCooldown)
    {
        vec2 playerVelocity = playerVelocityComponent->GetVelocity();
        Transform* playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

        float particleVelocityX = -playerVelocity.x * VELOCITY_MULTIPLIER;
        float particleVelocityY = -playerVelocity.y * VELOCITY_MULTIPLIER;
        constexpr float particleVelocityZ = 0.0f;
        vec3 particleVelocity = vec3(particleVelocityX, particleVelocityY, particleVelocityZ);
        
        vec3 emissionPos = GetPlayerDustEmissionPos();
        EmitParticles(ParticleType::Dust, emissionPos, particleVelocity);

        m_dustEmissionTimer = 0.0f;
    }
}

void ParticleManager::EmitParticles(ParticleType type, const vec3& position, const vec3& velocity)
{
    float scale = 1.0f;
    float lifetime = 1.0f;

    switch (type) 
    {
    case ParticleType::Dust:
        scale = DUST_PARTICLE_SCALE;
        lifetime = DUST_PARTICLE_LIFESPAN_DURATION;
        break;
    case ParticleType::Steam:
        scale = STEAM_PARTICLE_SCALE;
        lifetime = STEAM_PARTICLE_LIFESPAN_DURATION;
        break;
    }

    auto it = m_typeEmitters.find(type);
    if (it != m_typeEmitters.end())
    {
        ParticleEmitter& emitter = it->second;
        emitter.EmitParticle(position, scale, velocity, lifetime);
    }
}

vec3 ParticleManager::GetPlayerDustEmissionPos()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId entityId = entityManager.GetPlayerEntityId();
    Transform* transform = entityManager.GetComponent<Transform>(entityId);
    CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();

    float width = sprite->GetWidth() * transform->GetScale().x;
    float height = sprite->GetHeight() * transform->GetScale().y;
    float emissionXPos = transform->GetPosition().x;
    float emissionYPos = transform->GetPosition().y - width * EMISSION_POS_Y_MULTIPLIER;

    Velocity* velocityComponent = entityManager.GetComponent<Velocity>(entityId);
    vec2 velocity = velocityComponent->GetVelocity();

    // Adjust emission pos based on player movement direction
    if (abs(velocity.x) > abs(velocity.y))
    {
        emissionXPos += (velocity.x > 0) ? width * EMISSION_POS_HORIZONTAL_OFFSET_MULTIPLIER : -width * EMISSION_POS_HORIZONTAL_OFFSET_MULTIPLIER;
        emissionYPos -= height * EMISSION_POS_VERTICAL_OFFSET_DOWN_MULTIPLIER;
    }
    else
    {
        if (velocity.y > 0)
            emissionYPos -= height * EMISSION_POS_VERTICAL_OFFSET_UP_MULTIPLIER;
        else
            emissionYPos += height;
    }

    return vec3(emissionXPos, emissionYPos, EMISSION_Z_POS);
}

