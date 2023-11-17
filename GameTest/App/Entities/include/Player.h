#ifndef PLAYER_H
#define PLAYER_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Collider.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Health.h"

class Player {
public:
    static Entity Create();
};

#endif // PLAYER_H
