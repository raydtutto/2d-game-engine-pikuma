#include "Game.h"
#include <SDL.h>
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
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

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

void Game::Setup() {
    // TODO: Initialize game objects
}

void Game::Update() {
    // TODO: update game objects
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer); //clear color

    // TODO: render game objects

    // ---- Rectangle - START --------------------------------------------------------

    // Create a rectangle
    SDL_Rect player = { 10, 10, 20, 20 };
    // Set color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Draw a rectangle (destination, &object)
    SDL_RenderFillRect(renderer, &player);

    // ---- Rectangle - END --------------------------------------------------------

    SDL_RenderPresent(renderer); // present to display
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
