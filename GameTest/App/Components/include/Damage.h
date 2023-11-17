#ifndef DAMAGE_H
#define DAMAGE_H

#include "Component.h"

class Damage : public Component {
public:
    float damage;

    Damage() : damage(0.0f) {}
};

#endif // DAMAGE_H
