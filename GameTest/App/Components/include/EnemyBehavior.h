#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class EnemyBehavior : public Component
{
public:
    EnemyBehavior(EnemyBehaviorType type) : m_behaviorType(type) {}

    EnemyBehaviorType GetBehaviorType() const { return m_behaviorType; }

private:
    EnemyBehaviorType m_behaviorType;
};
