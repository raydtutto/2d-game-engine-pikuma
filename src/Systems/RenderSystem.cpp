#include "RenderSystem.h"

#include "AssetStore/AssetStore.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TilemapComponent.h"

#include <SDL.h>
#include <vector>
#include <glm/glm.hpp>

RenderSystem::RenderSystem() {
    RequireComponent<SpriteComponent>();
    RequireComponent<TransformComponent>();
    // RequireComponent<TilemapComponent>();
}

void RenderSystem::Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore) {
    // Loop all entities that the system is interested in
    for (auto entity : GetSystemEntities()) {
        // Get components, not modified
        const auto transform = entity.GetComponent<TransformComponent>();
        const auto sprite = entity.GetComponent<SpriteComponent>();
        // const auto tileMap = entity.GetComponent<TilemapComponent>();

        // Set the source rectangle of our original sprite texture
        SDL_Rect srcRect = sprite.srcRect;

        // Set the destination rectangle with x, y position to be rendered
        SDL_Rect dstRect = { static_cast<int>(transform.position.x),
                             static_cast<int>(transform.position.y),
                             static_cast<int>(sprite.width * transform.scale.x),
                             static_cast<int>(sprite.height * transform.scale.y) };
        /*if (!tileMap.tileMapId.empty()) {
            for (auto item : assetStore->GetTmxLayers(tileMap.tileMapId)) {
                if (item) {
                    // Draw the tmx texture in the renderer window
                    SDL_RenderCopyEx(renderer, item, &srcRect, &dstRect,
                                     transform.rotation, NULL, SDL_FLIP_NONE);
                }

            }
        }*/

        // Draw the png texture in the renderer window
        if (!sprite.assetId.empty()) {
            SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect,
                         transform.rotation, NULL, SDL_FLIP_NONE);
        }
    }
}