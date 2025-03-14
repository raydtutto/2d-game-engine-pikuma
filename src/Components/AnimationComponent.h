#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <memory>
#include <string>
#include <SDL.h>

// Forward declaration
struct AsepriteObject;

struct AnimationComponent {
    std::shared_ptr<AsepriteObject> animationData;
    SDL_Texture* spriteSheet;
    int currentFrame;
    float elapsedTime;
    bool isPlaying;
};


#endif //ANIMATIONCOMPONENT_H
