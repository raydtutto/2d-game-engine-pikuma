#include "AssetStore.h"

#include "Logger/Logger.h"

#include <SDL_image.h>

AssetStore::AssetStore() {
    Logger::Log("AssetStore constructor called.");
}

AssetStore::~AssetStore() {
    ClearAssets();
    Logger::Log("AssetStore destructor called.");
}

void AssetStore::ClearAssets() {
    // Loop through all textures
    for (auto texture: textures) {
        // Deallocate textures in memory
        SDL_DestroyTexture(texture.second); // texture.second - delete the value of "texture"
    }

    // Clear the map
    textures.clear();

    Logger::Log("Textures map was cleared");
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath) {
    // Create a texture from a surface
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture); // emplace(key, value)

    Logger::Log("New texture added to the Asset Store with id = " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) {
    return textures[assetId];
}

