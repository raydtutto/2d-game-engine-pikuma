#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "ECS/ECS.h"

constexpr int FPS = 60;
constexpr int MS_PER_FRAME = 1000 / FPS;

class Game {
private:
    bool isRunning;
    int msPrevFrame = MS_PER_FRAME;
    SDL_Window* window;
    SDL_Renderer* renderer;

    Registry* registry;

public:
    // API - Application programming interface - START ---------------------------------------
    Game();
    ~Game();

    void Initialize();
    void Run();
    void Setup();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
    // ---------------------------------------------------------------------------------------

    int windowWidth;
    int windowHeight;
};

#endif  // GAME_H
