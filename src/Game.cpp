#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>


Game::Game() {
    isRunning = false;
    std::cout << "Game constructor called." << std::endl;
}

Game::~Game() {
    std::cout << "Game destructor called." << std::endl;
}

void Game::Initialize() {
    // Avoid SDL error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    // ---- Fake fullscreen (pointer to a struct) - Start -------------------------------

    // Get the window width/height
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode); // Get and set the data to display object
    windowWidth = 800; // Use `displayMode.w` to get the window width
    windowHeight = 600; // Use `displayMode.h` to get the window width

    // Create a window
    window = SDL_CreateWindow(
        NULL, // NULL for title for not having screen borders
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    // Check window
    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    // ---- Fake fullscreen - END --------------------------------------------------------

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0 | SDL_RENDERER_PRESENTVSYNC);
    // `-1` means "get the default" display number in this case, `0` - for no flags
    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    // Make a real fullscreen from a fake one
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    isRunning = true;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        // passing address
        switch (sdlEvent.type) {
            case SDL_QUIT: // event when user tries to close the window
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
                break;
        }
    }
}

// Declare player position globally
glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
    // Set start position of an object
    playerPosition = glm::vec2(10.0, 10.0);
    // Set object velocity
    playerVelocity = glm::vec2(100.0,0.0); // every second move `x` pixels to right/left, `y` pixels up/down
}

void Game::Update() {
    // Cap framerate
    int timeToWait = MS_PER_FRAME - (SDL_GetTicks() - msPrevFrame); // calculate how much to wait
    if (timeToWait > 0 && timeToWait <= MS_PER_FRAME) {
        SDL_Delay(timeToWait); // wait
    }

    // Delta time - difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - msPrevFrame) / 1000.0;

    // Store the current frame time
    msPrevFrame = SDL_GetTicks();

    // Update player position
    playerPosition.x += playerVelocity.x * deltaTime;
    playerPosition.y += playerVelocity.y * deltaTime;
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);

    // Draw a PNG texture
    SDL_Surface* surface = IMG_Load("../assets/images/tank-tiger-right.png"); // creates surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // create texture from surface
    SDL_FreeSurface(surface); // once we get the texture, we don't need a surface

    // Draw texture in renderer
    SDL_Rect dstRect = { // destination rectangle for PNG texture
        static_cast<int>(playerPosition.x), // convert float to int
        static_cast<int>(playerPosition.y), // convert float to int
        32,
        32
    };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect); // copy an entire texture to the destination
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
