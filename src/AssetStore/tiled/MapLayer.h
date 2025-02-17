#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "Texture.h"
#include <SDL.h>
#include <tmxlite/Map.hpp>
#include <vector>
#include <memory>


namespace tiled {
    class MapLayer {
    public:
        explicit MapLayer();

        bool create(const std::shared_ptr<tmx::Map>& map, std::uint32_t index, const std::vector<std::unique_ptr<Texture>>& textures);

        SDL_Texture* generateTexture(SDL_Renderer* renderer);

        // void draw(SDL_Renderer*) const; // todo remove this later

    private:
        struct Subset
        {
            std::vector<SDL_Vertex> vertexData;
            SDL_Texture* texture = nullptr;
        };
        std::vector<Subset> m_subsets;
        SDL_Point m_size;
    };
}  // namespace tiled

#endif  // MAPLAYER_H
