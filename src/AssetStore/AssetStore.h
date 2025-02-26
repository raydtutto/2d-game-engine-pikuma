#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL.h>
#include <map>
#include <string>

// Forward declaration
struct AsepriteObject;
namespace tmx {
    class Map;
}

class AssetStore {
    // The list of textures
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, std::shared_ptr<tmx::Map>> tileMaps;
    std::map<std::string, std::vector<SDL_Texture*>> tileLayers;
    std::map<std::string, std::shared_ptr<AsepriteObject>> asepriteObjects;

    // TODO: map for fonts
    // TODO: map for audio

public:
    AssetStore();
    ~AssetStore();

    void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    void AddTmxFile(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
    void AddAsepriteObject(SDL_Renderer* renderer, const std::string& assetId, const std::string& jsonPath);
    std::vector<SDL_Texture*> GetTmxLayers(const std::string& assetId);
    std::shared_ptr<tmx::Map> GetTmxMap(const std::string& assetId);
    SDL_Texture* GetTexture(const std::string& assetId);

private:
    void ClearAssets();
};



#endif //ASSETSTORE_H
