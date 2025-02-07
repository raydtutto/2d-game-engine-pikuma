#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "ECS/ECS.h"
#include <SDL.h>
#include <SDL_image.h>

// Inherits from the parent class `System`
class RenderSystem : public System {
public:
    RenderSystem() {
        RequireComponent<SpriteComponent>();
        RequireComponent<TransformComponent>();
    }

    void Update(SDL_Renderer* renderer) {
        // Loop all entities that the system is interested in
        for (auto entity : GetSystemEntities()) {
            // Get components, not modified
            const auto transform = entity.GetComponent<TransformComponent>();
            const auto sprite = entity.GetComponent<SpriteComponent>();

            // Create a rectangle
            SDL_Rect objRect = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                sprite.width,
                sprite.height
            };

            // Set color and draw a rectangle
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &objRect);
        }
    }
};

#endif //RENDERSYSTEM_H
