#include "AssetStore.h"

#include "Logger/Logger.h"
#include "tiled/Texture.h"
#include "tiled/MapLayer.h"

#include <SDL_image.h>
#include <tmxlite/TileLayer.hpp>

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called.");
}

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("AssetStore destructor called.");
}

void AssetStore::ClearAssets() {
    // Loop through all textures
    for (auto texture : textures) {
        // Deallocate textures in memory
        SDL_DestroyTexture(texture.second);  // texture.second - delete the value of "texture"
    }

    // Clear the map
    textures.clear();

    Logger::Log("Textures map was cleared.");
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId,
                            const std::string& filePath) {
    // Create a texture from a surface
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface || !texture) {
        Logger::Error("Failed to load image: " + filePath);
        return;
    }
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);  // emplace(key, value)

    Logger::Log("New texture added to the Asset Store with id = " + assetId);
}

void AssetStore::AddTmxFile(SDL_Renderer* renderer, const std::string& assetId,
                            const std::string& filePath) {
    if (filePath.empty()) {
        Logger::Error("Tileset doesn't exist.");
        return;
    }

    auto map = std::make_shared<tmx::Map>();
    if (map->load(filePath)) {
        if (tileMaps.count(assetId) > 0U)
            tileMaps.erase(assetId);
        if (tileLayers.count(assetId) > 0U)
            tileLayers.erase(assetId);

        // Add to the AssetStore
        tileMaps[assetId] = map;
        tileLayers[assetId] = {};  // Initialize vector

        Logger::Log("Tile map " + assetId + " loaded");
        // Generate texture
        std::vector<std::unique_ptr<tiled::Texture>> textures;
        std::vector<std::unique_ptr<tiled::MapLayer>> renderLayers;
        // CreateLayerTexture(renderer, assetId);
        // load the textures as they're shared between layers
        const auto& tileSets = map->getTilesets();
        assert(!tileSets.empty());  // todo fix this
        for (const auto& ts : tileSets) {
            textures.emplace_back(std::make_unique<tiled::Texture>());
            if (!textures.back()->loadFromFile(ts.getImagePath(), renderer))
                Logger::Error("Failed opening " + ts.getImagePath());
        }

        // load the layers
        const auto& mapLayers = map->getLayers();
        for (auto i = 0u; i < mapLayers.size(); ++i) {
            if (mapLayers[i]->getType() == tmx::Layer::Type::Tile) {
                renderLayers.emplace_back(std::make_unique<tiled::MapLayer>());
                if (renderLayers.back()->create(map, i, textures))
                    tileLayers[assetId].push_back(renderLayers.back()->generateTexture(renderer));
            }
        }
    }
}

std::vector<SDL_Texture*> AssetStore::GetTmxLayers(const std::string& assetId) {
    if (tileLayers.count(assetId) == 0U)
        return {};
    return tileLayers[assetId];
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    if (textures.count(assetId) == 0U) {
        Logger::Error("Can't find an assetid = " + assetId + "!");
        return nullptr;
    }
    return textures.at(assetId);
}

