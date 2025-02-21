#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Components/SpriteComponent.h"
#include "ECS/ECS.h"

#include <memory>

// Forward declaration
struct SpriteComponent;
struct TransformComponent;
struct SDL_Renderer;
class AssetStore;

// Inherits from the parent class `System`
class RenderSystem : public System {
    std::vector<Entity> renderBuckets[LAYER_COUNT];
public:
    RenderSystem();
    void Update(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore);

private:
    void SortEntitiesIntoBuckets();
    void UpdateSprites(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore, const TransformComponent& t, const SpriteComponent& s);
    void UpdateTiles(SDL_Renderer* renderer, const std::unique_ptr<AssetStore>& assetStore, const TransformComponent& t, const SpriteComponent& s);
};

#endif //RENDERSYSTEM_H
