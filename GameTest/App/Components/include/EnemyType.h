#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class EnemyType : public Component
{
public:
    EnemyType(EnemyMovementType type) : m_movementType(type) {}

    EnemyMovementType GetMovementType() const { return m_movementType; }

private:
    EnemyMovementType m_movementType = EnemyMovementType::None;
};
