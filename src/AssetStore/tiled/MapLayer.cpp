#include "MapLayer.h"

#include "Logger/Logger.h"

#include <array>
#include <iostream>
#include <tmxlite/TileLayer.hpp>

using namespace tiled;

/*SDL_Point getTextureSize(SDL_Texture *texture) {
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}*/

MapLayer::MapLayer() {
}

bool MapLayer::create(const std::shared_ptr<tmx::Map>& map, std::uint32_t layerIndex,
                      const std::vector<std::unique_ptr<Texture>>& textures) {
    const auto& layers = map->getLayers();
    if (layers[layerIndex]->getType() != tmx::Layer::Type::Tile)
        Logger::Error("Invalid map layer.");
        return false;

    const auto& layer = layers[layerIndex]->getLayerAs<tmx::TileLayer>();
    const auto mapSize = map->getTileCount();
    const auto mapTileSize = map->getTileSize();
    const auto& tileSets = map->getTilesets();
    m_size.x = mapSize.x * mapTileSize.x;
    m_size.y = mapSize.y * mapTileSize.y;

    const auto tintColour = layer.getTintColour();
    const SDL_Colour vertColour = { tintColour.r, tintColour.g, tintColour.b, tintColour.a };

    for (auto i = 0u; i < tileSets.size(); ++i) {
        // check tile ID to see if it falls within the current tile set
        const auto& ts = tileSets[i];
        const auto& tileIDs = layer.getTiles();

        const auto texSize = textures[i]->getSize();
        const auto tileCountX = texSize.x / mapTileSize.x;
        const auto tileCountY = texSize.y / mapTileSize.y;

        const float uNorm = static_cast<float>(mapTileSize.x) / texSize.x;
        const float vNorm = static_cast<float>(mapTileSize.y) / texSize.y;

        std::vector<SDL_Vertex> verts;
        for (auto y = 0u; y < mapSize.y; ++y) {
            for (auto x = 0u; x < mapSize.x; ++x) {
                const auto idx = y * mapSize.x + x;
                if (idx < tileIDs.size() && tileIDs[idx].ID >= ts.getFirstGID()
                    && tileIDs[idx].ID < (ts.getFirstGID() + ts.getTileCount())) {
                    // tex coords
                    auto idIndex = (tileIDs[idx].ID - ts.getFirstGID());
                    float u = static_cast<float>(idIndex % tileCountX);
                    float v = static_cast<float>(idIndex / tileCountY);
                    u *= mapTileSize.x;  // TODO we should be using the tile set size, as this may
                                         // be different from the map's grid size
                    v *= mapTileSize.y;

                    // normalise the UV
                    u /= textures[i]->getSize().x;
                    v /= textures[i]->getSize().y;

                    // vert pos
                    const float tilePosX = static_cast<float>(x) * mapTileSize.x;
                    const float tilePosY = (static_cast<float>(y) * mapTileSize.y);

                    // push back to vert array
                    SDL_Vertex vert = { { tilePosX, tilePosY }, vertColour, { u, v } };
                    verts.emplace_back(vert);
                    vert = { { tilePosX + mapTileSize.x, tilePosY }, vertColour, { u + uNorm, v } };
                    verts.emplace_back(vert);
                    vert = { { tilePosX, tilePosY + mapTileSize.y }, vertColour, { u, v + vNorm } };
                    verts.emplace_back(vert);

                    vert = { { tilePosX, tilePosY + mapTileSize.y }, vertColour, { u, v + vNorm } };
                    verts.emplace_back(vert);
                    vert = { { tilePosX + mapTileSize.x, tilePosY }, vertColour, { u + uNorm, v } };
                    verts.emplace_back(vert);
                    vert = { { tilePosX + mapTileSize.x, tilePosY + mapTileSize.y },
                             vertColour,
                             { u + uNorm, v + vNorm } };
                    verts.emplace_back(vert);
                }
            }
        }

        if (!verts.empty()) {
            m_subsets.emplace_back();
            m_subsets.back().texture = *textures[i];
            m_subsets.back().vertexData.swap(verts);
            /*auto size = getTextureSize(m_subsets.back().texture);
            m_size.x = std::max(m_size.x, size.x);
            m_size.y = std::max(m_size.y, size.y);*/
        }
    }

    return !m_subsets.empty();
}

SDL_Texture* MapLayer::generateTexture(SDL_Renderer* renderer) {
    if (!renderer) {
        Logger::Error("Map layer can't render.");
        return nullptr;
    }
    if (m_subsets.empty()) {
        Logger::Error("Map layer doesn't exist.");
        return nullptr;
    }

    /*for (const auto& s : m_subsets)
    {
        s.vertexData.
        SDL_RenderGeometry(renderer, s.texture, s.vertexData.data(), static_cast<std::int32_t>(s.vertexData.size()), nullptr, 0);
    }
    return result;*/

    // Create a blank texture with the required size
    SDL_Texture* finalTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_size.x, m_size.y);
    if (!finalTexture) {
        Logger::Error("Failed to create texture.");
        return nullptr;
    }

    // Set blend mode to allow transparency
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    // Set render target to the final texture
    SDL_SetRenderTarget(renderer, finalTexture);

    // Clear the texture (optional, ensures a transparent background)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Render each subset's texture onto the final texture
    for (const auto& subset : m_subsets)
    {
        if (!subset.texture) continue;

        // Assuming each subset's texture needs to be drawn at a specific position
        SDL_Rect destRect = { 0, 0, m_size.x, m_size.y }; // Adjust this as needed
        SDL_RenderCopy(renderer, subset.texture, nullptr, &destRect);
    }

    // Reset render target to default (screen)
    SDL_SetRenderTarget(renderer, nullptr);

    return finalTexture;
}