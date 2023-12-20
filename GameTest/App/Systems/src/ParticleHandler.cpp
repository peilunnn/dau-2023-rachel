#include "stdafx.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/SpriteManager.h"
#include "Systems/include/ParticleHandler.h"
#include "Utilities/include/Helper.h"
using std::find;

ParticleHandler& ParticleHandler::GetInstance() 
{
    static ParticleHandler instance;
    return instance;
}

void ParticleHandler::Init() 
{
    InitParticlePool(ParticleType::Dust);
}

void ParticleHandler::Update(float deltaTime) 
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    Velocity* playerVelocityComponent = entityManager.GetComponent<Velocity>(playerEntityId);

    m_emissionTimer -= deltaTime;

    if (length(playerVelocityComponent->GetVelocity()) > 0.0f && m_emissionTimer <= 0.0f)
    {
        vec2 playerVelocity = playerVelocityComponent->GetVelocity();
        constexpr float velocityMultiplier = 0.25f;
        float particleVelocityX = -playerVelocity.x * velocityMultiplier;
        float particleVelocityY = -playerVelocity.y * velocityMultiplier;
        vec2 particleVelocity = vec2(particleVelocityX, particleVelocityY);
        
        vec3 emissionPos = GetEmissionPos();
        
        EmitParticle(ParticleType::Dust, emissionPos, particleVelocity);
        
        m_emissionTimer = m_emissionCooldown;
    }
}

void ParticleHandler::ReturnParticleToPool(ParticleType type, EntityId particleEntityId) {
    EntityManager& entityManager = EntityManager::GetInstance();

    RemoveActiveParticle(type, particleEntityId);

    entityManager.SetEntityStateAndVisibility(particleEntityId, EntityState::Dead, false);
    m_particlePools[type].push_back(particleEntityId);
}

void ParticleHandler::InitParticlePool(ParticleType particleType) 
{
    SpriteManager& spriteManager = SpriteManager::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();
    vector<EntityId>& pool = m_particlePools[particleType];

    for (size_t i = 0; i < m_poolSize; ++i) 
    {
        EntityId particleEntityId = entityManager.CreateParticleEntity(spriteManager, particleType);
        pool.push_back(particleEntityId);
    }
}

EntityId ParticleHandler::GetParticleFromPool(ParticleType type) 
{
    EntityManager& entityManager = EntityManager::GetInstance();
    vector<EntityId>& pool = m_particlePools[type];
    EntityId particleEntityId = pool.back();
    entityManager.SetEntityStateAndVisibility(particleEntityId, EntityState::Alive, true);
    
    pool.pop_back();
    return particleEntityId;
}

void ParticleHandler::RemoveActiveParticle(ParticleType particleType, EntityId particleEntityId) 
{
    auto& activeParticles = m_activeParticles[particleType];
    auto it = find(activeParticles.begin(), activeParticles.end(), particleEntityId);
    if (it != activeParticles.end())
        activeParticles.erase(it);
}

vec3 ParticleHandler::GetEmissionPos()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    Transform* playerTransform = entityManager.GetComponent<Transform>(playerEntityId);
    CSimpleSprite* playerSprite = entityManager.GetComponent<Renderable>(playerEntityId)->GetSprite();
    Velocity* playerVelocityComponent = entityManager.GetComponent<Velocity>(playerEntityId);
    vec2 playerVelocity = playerVelocityComponent->GetVelocity();
    
    float playerWidth = playerSprite->GetWidth() * playerTransform->GetScale().x;
    float playerHeight = playerSprite->GetHeight() * playerTransform->GetScale().y;
    float emissionXPos = playerTransform->GetPosition().x;
    float emissionYPos = playerTransform->GetPosition().y - playerHeight / 4.0f;

    // Adjust emission pos based on player movement direction
    // Horizontal movement
    if (abs(playerVelocity.x) > abs(playerVelocity.y))
    {
        emissionXPos += (playerVelocity.x > 0) ? playerWidth / 10.0f : -playerWidth / 10.0f;
        emissionYPos -= playerHeight / 8.0f;
    }
    // Vertical movement
    else
    {
        // If moving up
        if (playerVelocity.y > 0)
            emissionYPos -= playerHeight / 5.0f;
        else
            emissionYPos += playerHeight;
    }
    constexpr float emissionZPos = 0.0f;
    
    return vec3(emissionXPos, emissionYPos, emissionZPos);
}

void ParticleHandler::EmitParticle(ParticleType type, const vec3& position, const vec2& velocity)
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId particleEntityId = GetParticleFromPool(type);

    Transform* particleTransform = entityManager.GetComponent<Transform>(particleEntityId);
    Velocity* particleVelocity = entityManager.GetComponent<Velocity>(particleEntityId);

    particleTransform->SetPosition(position);
    particleVelocity->SetVelocity(velocity);
    m_activeParticles[type].push_back(particleEntityId);
}
