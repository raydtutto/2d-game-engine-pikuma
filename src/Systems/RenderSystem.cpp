#include "RenderSystem.h"

#include "AssetStore/AssetStore.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

#include <SDL.h>
#include <SDL_image.h>

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

        // // Create a rectangle
        // SDL_Rect objRect = {
        //     static_cast<int>(transform.position.x),
        //     static_cast<int>(transform.position.y),
        //     sprite.width,
        //     sprite.height
        // };
        // // Set color and draw a rectangle
        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // SDL_RenderFillRect(renderer, &objRect);

        // Set the source rectangle of our original sprite texture
        SDL_Rect srcRect = sprite.srcRect;

        // Set the destination rectangle with x, y position to be rendered
        SDL_Rect dstRect = { static_cast<int>(transform.position.x),
                             static_cast<int>(transform.position.y),
                             static_cast<int>(sprite.width * transform.scale.x),
                             static_cast<int>(sprite.height * transform.scale.y) };

        // Draw the png texture
        SDL_RenderCopyEx(renderer,
                         assetStore->GetTexture(sprite.assetId),
                         &srcRect,
                         &dstRect,
                         transform.rotation,
                         NULL,
                         SDL_FLIP_NONE);
    }
}