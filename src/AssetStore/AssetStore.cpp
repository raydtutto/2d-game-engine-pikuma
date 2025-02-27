#include "AssetStore.h"

#include "Logger/Logger.h"
#include "ResourceManager/Tiled/MapLayer.h"
#include "ResourceManager/Tiled/Texture.h"
#include "ResourceManager/Aseprite/AsepriteObject.h"

#include <SDL_image.h>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Map.hpp>

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called.");
}

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("AssetStore destructor called.");
}

void AssetStore::ClearAssets() {
    // Loop through all textures
    for (auto& pair : textures) {
        // Deallocate textures in memory
        SDL_DestroyTexture(pair.second);  // pair.second - delete the value of "texture"
    }

    // Loop through Tiled layers
    for (auto& [_, val] : tileLayers) { // range-based loop, xx17
        for (SDL_Texture* texture : val) {
            SDL_DestroyTexture(texture);
        }
    }

    // Clear the map
    textures.clear();
    tileMaps.clear();
    tileLayers.clear();
    asepriteObjects.clear();

    Logger::Log("Textures, tiled and Aseprite objects have been cleared.");
}

void AssetStore::LoadTexture(SDL_Renderer* renderer, const std::string& assetId,
                            const std::string& filePath) {
    // Create a texture from a surface
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!surface || !texture) {
        Logger::Error("Failed to load image: {}", filePath);
        return;
    }
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);  // emplace(key, value)

    Logger::Log("New texture added to the Asset Store with id: {}", assetId);
}

void AssetStore::LoadTmxFile(SDL_Renderer* renderer, const std::string& assetId,
                            const std::string& filePath) {
    if (filePath.empty()) {
        Logger::Error("Tileset doesn't exist: {}", filePath);
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

        Logger::Log("Tile map loaded: {}", assetId);
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
                Logger::Error("Failed opening: {} ", ts.getImagePath());
        }

        // load the layers
        const auto& mapLayers = map->getLayers();
        for (auto i = 0u; i < mapLayers.size(); ++i) {
            if (mapLayers[i]->getType() == tmx::Layer::Type::Tile) {
                renderLayers.emplace_back(std::make_unique<tiled::MapLayer>());
                if (renderLayers.back()->Create(renderer, map, i, textures))
                    tileLayers[assetId].push_back(renderLayers.back()->GenerateTexture(renderer));
            }
        }
    }
}

void AssetStore::LoadAseprite(SDL_Renderer* renderer, const std::string& assetId, const std::string& jsonPath) {
    auto aseprite = std::make_shared<AsepriteObject>();
    if (aseprite->Load(jsonPath)) {
        // Get image path
        const std::string basePath = jsonPath.substr(0, jsonPath.find_last_of("/\\"));
        const std::string imagePath = basePath + "/" + aseprite->imageName;

        // Add data
        LoadTexture(renderer, assetId, imagePath);
        asepriteObjects[assetId] = aseprite;
    }

    Logger::Log("New Aseprite object added to the Asset Store with id: {}", assetId);
}

std::vector<SDL_Texture*> AssetStore::GetTmxLayers(const std::string& assetId) {
    auto item = tileLayers.find(assetId);
    if (item == tileLayers.end()) {
        Logger::Error("Can't find TMX layer with assetId: {}", assetId);
        return {};
    }
    return item->second;
}

std::shared_ptr<tmx::Map> AssetStore::GetTmxMap(const std::string& assetId) {
    auto item = tileMaps.find(assetId);
    if (item == tileMaps.end()) {
        Logger::Error("Can't find tilemap with assetId: {}", assetId);
        return nullptr;
    }
    return item->second;
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    auto item = textures.find(assetId);
    if (item == textures.end()) {
        Logger::Error("Can't find texture with assetId: {}", assetId);
        return nullptr;
    }
    return item->second;
}

std::shared_ptr<AsepriteObject> AssetStore::GetAsepriteObject(const std::string& assetId)  {
    auto item = asepriteObjects.find(assetId);
    if (item == asepriteObjects.end()) {
        Logger::Error("Can't find Aseprite object with assetId: {}", assetId);
        return nullptr;
    }
    return item->second;
}

