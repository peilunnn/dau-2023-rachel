// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

// Standard Library Headers
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <glm/glm.hpp>
#include <random>
#include <string>
#include <Windows.h>
#include <sstream>

// Third-Party Library Headers
#include <glm/glm.hpp>
#include <SFML/Audio.hpp>

// Common Project Headers
#include "App/Components/include/Ammo.h"
#include "App/Components/include/Animation.h"
#include "App/Components/include/BounceDirection.h"
#include "App/Components/include/Collider.h"
#include "App/Components/include/Color.h"
#include "App/Components/include/Component.h"
#include "App/Components/include/Cooldown.h"
#include "App/Components/include/EnemyType.h"
#include "App/Components/include/Health.h"
#include "App/Components/include/Particle.h"
#include "App/Components/include/Pickup.h"
#include "App/Components/include/Renderable.h"
#include "App/Components/include/Score.h"
#include "App/Components/include/Screen.h"
#include "App/Components/include/Tag.h"
#include "App/Components/include/Timer.h"
#include "App/Components/include/Transform.h"
#include "App/Components/include/Velocity.h"
#include "App/Utilities/include/EntityId.h"
#include "App/Utilities/include/Enums.h"
#include "App/Utilities/include/Event.h"
#include "App/Utilities/include/Helper.h"
#include "App/Utilities/include/QuadtreeNode.h"