#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL.h>
#include <map>
#include <string>
#include <tmxlite/Map.hpp>

class AssetStore {
    // The list of textures
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, std::shared_ptr<tmx::Map>> tileMaps;
    std::map<std::string, std::vector<SDL_Texture*>> tileLayers;

    // TODO: map for fonts
    // TODO: map for audio

public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    void AddTmxFile(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    std::vector<SDL_Texture*> GetTmxLayers(const std::string& assetId);
    std::shared_ptr<tmx::Map> GetTmxMap(const std::string& assetId);
    SDL_Texture* GetTexture(const std::string& assetId);

};



#endif //ASSETSTORE_H
