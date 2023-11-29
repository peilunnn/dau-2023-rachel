#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/SimpleSprite.h"
#include "Systems/include/Event.h"
#include <glm/glm.hpp>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
using namespace std;

class EntityManager
{
private:
	EntityId playerEntity;
	EntityId enemyEntity;
	EntityId reloadingCircleEntity;
	EntityId ammoEmptyEntity;
	EntityId ammoFilledEntity;
	EntityId healthBarEntity;
	EntityId scoreEntity;
	EntityId timerEntity;

	static EntityId nextEntityId;
	unordered_map<EntityId, vector<shared_ptr<Component>>> entityComponents;
	vector<EntityId> entitiesToDelete;
	vector<EntityId> ammoEmptyEntities;
	vector<EntityId> ammoFilledEntities;

public:
	EntityId GetPlayerEntityId() const
	{
		return playerEntity;
	}
	EntityId GetEnemyEntityId() const
	{
		return enemyEntity;
	}
	EntityId GetReloadingCircleEntityId() const
	{
		return reloadingCircleEntity;
	}
	EntityId GetHealthBarEntityId() const
	{
		return healthBarEntity;
	}
	EntityId GetScoreEntityId() const
	{
		return scoreEntity;
	}
	EntityId GetTimerEntityId() const
	{
		return timerEntity;
	}

	void Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> ammoEmptySprite, shared_ptr<CSimpleSprite> ammoFilledSprite, shared_ptr<CSimpleSprite> healthBarSprite);
	vector<EntityId> GetAllEntities();
	static EntityId CreateEntity();
	EntityId CreatePlayerEntity(shared_ptr<CSimpleSprite> playerSprite);
	EntityId CreateEnemyEntity(const glm::vec3& playerPos, shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight);
	EntityId CreateBulletEntity(shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3& position, const glm::vec2& targetVelocity);
	EntityId CreateReloadingCircleEntity(shared_ptr<CSimpleSprite> reloadingCircleSprite);
	EntityId CreateAmmoEntity(shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos);
	EntityId CreateHealthBarEntity(shared_ptr<CSimpleSprite> sprite, float xPos, float yPos);

	EntityId CreateScoreEntity();
	EntityId CreateTimerEntity();

	void HideAmmoFilledEntity(int index);
	void ShowAllAmmoFilledEntity();
	void MoveEntityToRandomPos(EntityId entityId);
	void MarkEntityForDeletion(EntityId entityId);
	void ProcessDeletions();

	void ProcessBulletHitEnemy(EntityManager& entityManager, Event event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight);
	void ProcessEnemyHitPlayer(EntityManager& entityManager, Event event, float deltaTime);

	template <typename T>
	void AddComponent(EntityId entityId, shared_ptr<T> component)
	{
		entityComponents[entityId].push_back(component);
	}

	template <typename T>
	shared_ptr<T> GetComponent(EntityId entityId)
	{
		auto it = entityComponents.find(entityId);
		if (it != entityComponents.end())
		{
			for (auto& comp : it->second)
			{
				shared_ptr<T> casted = dynamic_pointer_cast<T>(comp);
				if (casted)
				{
					return casted;
				}
			}
		}
		return nullptr;
	}

	template <typename... Components>
	vector<EntityId> GetEntitiesWithComponents()
	{
		vector<EntityId> entitiesWithComponents;
		for (const auto& pair : entityComponents)
		{
			bool hasAllComponents = ((GetComponent<Components>(pair.first) != nullptr) && ...);
			if (hasAllComponents)
			{
				entitiesWithComponents.push_back(pair.first);
			}
		}
		return entitiesWithComponents;
	}
};