#ifndef GAME_H
#define GAME_H
#include <SDL.h>

class Game {
private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;

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
    // API - END -----------------------------------------------------------------------------

    int windowWidth;
    int windowHeight;
};

#endif //GAME_H
