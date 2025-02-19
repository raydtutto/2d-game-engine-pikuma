#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>
#include <vector>
#include <SDL.h>

enum class SpriteType : int {
    SPRITE = 0,
    TILED,
};

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;
    SDL_Rect srcRect;
    SpriteType spriteType;
    std::vector<size_t> tileLayerIndexes; // indexes that should draw as a tiled element, if empty the draw all

    // Constructor
    SpriteComponent(const std::string& _assetId = "", int _width = 0, int _height = 0, int _srcRectX = 0, int _srcRectY = 0, SpriteType _spriteType = SpriteType::SPRITE) {
        assetId = _assetId;
        width = _width;
        height = _height;
        srcRect = {_srcRectX, _srcRectY, _width, _height};
        spriteType = _spriteType;
    }
};

#endif //SPRITECOMPONENT_H
