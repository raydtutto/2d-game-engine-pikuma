#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL.h>
#include <string>
#include <vector>

// Forward declaration
class Entity;

enum RenderLayers {
    LAYER_TILEMAP,
    LAYER_VEGETATION,
    LAYER_OBSTACLES,
    LAYER_ENEMIES,
    LAYER_PLAYER,
    LAYER_GUI,
    LAYER_COUNT  // track the total amount layers
};

enum class SpriteType : int {
    SPRITE = 0,
    TILED,
};

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    RenderLayers layer;  // group of layers with z-index
    SDL_Rect srcRect;
    SpriteType spriteType;
    std::vector<size_t> tileLayerIndexes;  // indexes that should draw as a tiled element, if empty
                                           // the draw all

    // Constructor
    SpriteComponent(const std::string& _assetId = "", int _width = 0, int _height = 0,
                    const RenderLayers _layer = LAYER_TILEMAP, int _srcRectX = 0, int _srcRectY = 0,
                    SpriteType _spriteType = SpriteType::SPRITE) {
        assetId = _assetId;
        width = _width;
        height = _height;
        layer = _layer;
        srcRect = { _srcRectX, _srcRectY, _width, _height };
        spriteType = _spriteType;
    }
};

#endif  // SPRITECOMPONENT_H
