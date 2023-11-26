#ifndef SCORE_HANDLER_H
#define SCORE_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Systems/include/System.h"
#include "../../Utilities/include/Helper.h"

class ScoreHandler : public System 
{
public:
    void ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime);
    Type GetSystemType() const override {
        return System::Type::ScreenHandler;
    }
};

#endif // SCORE_HANDLER_H
