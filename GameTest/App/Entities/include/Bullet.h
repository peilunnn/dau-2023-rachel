#ifndef BULLET_H
#define BULLET_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Collider.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Damage.h"

class Bullet {
public:
    static Entity Create();
};

#endif // BULLET_H
