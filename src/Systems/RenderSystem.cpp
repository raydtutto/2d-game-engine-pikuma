#include "RenderSystem.h"

#include "AssetStore/AssetStore.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

#include <SDL.h>
#include <vector>
#include <glm/glm.hpp>
#include <tmxlite/Map.hpp>

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

        switch (sprite.spriteType) {
            case SpriteType::SPRITE:
                UpdateSprites(renderer, assetStore, transform, sprite);
                break;
            case SpriteType::TILED:
                UpdateTiles(renderer, assetStore, transform, sprite);
                break;
        }
    }
}

void RenderSystem::UpdateSprites(SDL_Renderer* renderer,
                                 const std::unique_ptr<AssetStore>& assetStore,
                                 const TransformComponent& transform, const SpriteComponent& sprite) {
    // Set the source rectangle of our original sprite texture
    SDL_Rect srcRect = sprite.srcRect;

    // Set the destination rectangle with x, y position to be rendered
    SDL_Rect dstRect = { static_cast<int>(transform.position.x),
                         static_cast<int>(transform.position.y),
                         static_cast<int>(sprite.width * transform.scale.x),
                         static_cast<int>(sprite.height * transform.scale.y) };

    // Draw the png texture in the renderer window
    if (!sprite.assetId.empty()) {
        SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite.assetId), &srcRect, &dstRect,
                         transform.rotation, NULL, SDL_FLIP_NONE);
    }
}

void RenderSystem::UpdateTiles(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore,
                               const TransformComponent& transform, const SpriteComponent& sprite) {

    auto const tilemap = assetStore->GetTmxMap(sprite.assetId);
    if (!tilemap.get())
        return;
    // Set the source rectangle of our original sprite texture
    const int width = tilemap->getTileCount().x * tilemap->getTileSize().x;
    const int height = tilemap->getTileCount().y * tilemap->getTileSize().y;
    SDL_Rect srcRect = {sprite.srcRect.x, sprite.srcRect.y, width, height};

    // Set the destination rectangle with x, y position to be rendered
    SDL_Rect dstRect = { static_cast<int>(transform.position.x),
                         static_cast<int>(transform.position.y),
                         static_cast<int>(width * transform.scale.x),
                         static_cast<int>(height * transform.scale.y) };

    const auto layers = assetStore->GetTmxLayers(sprite.assetId);

    for (size_t i = 0U; i < layers.size(); i++) {
        bool isLayerValid = sprite.tileLayerIndexes.empty()
            || std::find(sprite.tileLayerIndexes.begin(), sprite.tileLayerIndexes.end(), i) != sprite.tileLayerIndexes.end();
        if (!isLayerValid)
            continue;
        if (const auto item = layers[i]) {
            SDL_RenderCopyEx(renderer, item, &srcRect, &dstRect,
                             transform.rotation, NULL, SDL_FLIP_NONE);
        }
    }

}