#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "ECS/ECS.h"

// Inherits from the parent class `System`
class MovementSystem : public System {
public:
    MovementSystem() {
        // Get required entities
    }

    void Update() {
        // Update each entity position on every frame of the game loop
        // for (auto entity : GetEntities()) {
            //
        // }
    }
};

#endif  // MOVEMENTSYSTEM_H
