#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "ECS/ECS.h"
#include "glm/glm.hpp"

#include <vector>

// Inherits from the parent class `System`
class MovementSystem : public System {
public:
    MovementSystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void Update(double deltaTime) {
        // Loop all entities that the system is interested in
        for (auto entity : GetSystemEntities()) {
            // Get components:
            // Get and modified
            auto& transform = entity.GetComponent<TransformComponent>();
            // Only get, not modified
            const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

            // Update entity position based on its velocity
            transform.position.x += rigidbody.velocity.x * deltaTime;
            transform.position.y += rigidbody.velocity.y * deltaTime;

            // Logger::Log("Entity id = " + std::to_string(entity.GetId()) + " position is now ("
            //             + std::to_string(transform.position.x) + ", "
            //             + std::to_string(transform.position.y) + ")");
        }
    }
};

#endif  // MOVEMENTSYSTEM_H
