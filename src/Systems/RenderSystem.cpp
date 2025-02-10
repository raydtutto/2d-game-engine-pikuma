#include "RenderSystem.h"

#include "AssetStore/AssetStore.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

#include <SDL.h>
#include <vector>
#include <glm/glm.hpp>

RenderSystem::RenderSystem() {
    RequireComponent<SpriteComponent>();
    RequireComponent<TransformComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore) {
    // Loop all entities that the system is interested in
    for (auto entity : GetSystemEntities()) {
        // Get components, not modified
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto sprite = entity.GetComponent<SpriteComponent>();

        // Set the source rectangle of our original sprite texture
        SDL_Rect srcRect = sprite.srcRect;

        // Set the destination rectangle with x, y position to be rendered
        SDL_Rect dstRect = { static_cast<int>(transform.position.x),
                             static_cast<int>(transform.position.y),
                             static_cast<int>(sprite.width * transform.scale.x),
                             static_cast<int>(sprite.height * transform.scale.y) };

        // Draw the png texture in the renderer window
        SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect,
                         transform.rotation, NULL, SDL_FLIP_NONE);
    }
}