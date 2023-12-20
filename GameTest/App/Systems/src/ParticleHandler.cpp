#include "stdafx.h"
#include "Components/include/Velocity.h"
#include "Systems/include/ParticleHandler.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/SpriteManager.h"
#include "Utilities/include/Helper.h"
#include <algorithm>
#include <sstream>

ParticleHandler& ParticleHandler::GetInstance() {
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
    EntityId playerId = entityManager.GetPlayerEntityId();
    Transform* playerTransform = entityManager.GetComponent<Transform>(playerId);
    Velocity* playerVelocity = entityManager.GetComponent<Velocity>(playerId);

    if (glm::length(playerVelocity->GetVelocity()) > 0.0f) 
    {
        EmitParticle(ParticleType::Dust, playerTransform->GetPosition());
    }
    //else
    //{
    //    ReturnParticleToPool(ParticleType::Dust, )
    //}
}

void ParticleHandler::EmitParticle(ParticleType type, const glm::vec3& position) 
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId particleEntityId = GetParticleFromPool(type);
    Transform* particleTransform = entityManager.GetComponent<Transform>(particleEntityId);
    constexpr float zPos = 0.0f;
    vec3 newPos = vec3(position.x, position.y, zPos);
    particleTransform->SetPosition(newPos);
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

void ParticleHandler::ReturnParticleToPool(ParticleType type, EntityId particleEntityId) 
{
    EntityManager& entityManager = EntityManager::GetInstance();
    entityManager.SetEntityStateAndVisibility(particleEntityId, EntityState::Dead, true);

    m_particlePools[type].push_back(particleEntityId);
}
