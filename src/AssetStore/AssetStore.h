#ifndef ASSETSTORE_H
#define ASSETSTORE_H
#include <SDL.h>
#include <map>
#include <string>

class AssetStore {
    // The list of textures
    std::map<std::string, SDL_Texture*> textures;
    // TODO: map for fonts
    // TODO: map for audio

public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    SDL_Texture* GetTexture(const std::string& assetId);
};



#endif //ASSETSTORE_H
