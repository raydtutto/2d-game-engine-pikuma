#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

struct SpriteComponent {
    std::string assetId;
    int width;
    int height;

    // Constructor
    SpriteComponent(std::string assetId = "", int width = 0, int height = 0) {
        this->assetId = assetId;
        this->width = width;
        this->height = height;
    }
};

#endif //SPRITECOMPONENT_H
