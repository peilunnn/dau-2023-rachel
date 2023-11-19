#include "stdafx.h"
#include "../include/SystemManager.h"

void SystemManager::addSystem(std::unique_ptr<System> system) {
    systems.push_back(std::move(system));
}

void SystemManager::sendEvent(SystemEvent event) {
    eventQueue.push(event);
}

void SystemManager::processEvents() {
    while (!eventQueue.empty()) {
        SystemEvent event = eventQueue.front();
        eventQueue.pop();

        // Process each event based on its type
        switch (event.type) {
        case SystemEvent::BulletHitEnemy:
            // Handle BulletHitEnemy event
            break;
        case SystemEvent::PlayerHitEnemy:
            // Handle PlayerHitEnemy event
            break;
        case SystemEvent::PlayerHitReloadingCircle:
            // Handle PlayerHitReloadingCircle event
            break;
        case SystemEvent::PlayerHealthReachZero:
            // Handle PlayerHealthReachZero event
            break;
        case SystemEvent::CountdownReachZero:
            // Handle CountdownReachZero event
            break;
        }
    }
}
