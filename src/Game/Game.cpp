#include "Game.h"

// TODO: Create one header file for components
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"

// TODO: Create one header file for systems
#include "AssetStore/AssetStore.h"
#include "ECS/ECS.h"
#include "Logger/Logger.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"

#include <SDL.h>
#include <fstream>
#include <glm/glm.hpp>
#include <string>

Game::Game() {
    isRunning = false;

    // Create unique pointers
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();

    Logger::Log("Game constructor called.");
}

Game::~Game() {
    Logger::Log("Game destructor called.");
}

void Game::Initialize() {
    // Avoid SDL error
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Error("Error initializing SDL.");
        return;
    }

    // ---- Fake fullscreen - START --------------------------------------------------------------
    // Get the window width/height
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);  // Get and set the data to display object
    windowWidth = 800;                           // Use `displayMode.w` to get the window width
    windowHeight = 600;                          // Use `displayMode.h` to get the window width

    // Create a window
    window = SDL_CreateWindow(NULL,  // NULL for title for not having screen borders
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth,
                              windowHeight, SDL_WINDOW_BORDERLESS);
    // Check window
    if (!window) {
        Logger::Error("Error creating SDL window.");
        return;
    }
    // ---- Fake fullscreen - END ----------------------------------------------------------------

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, 0 | SDL_RENDERER_PRESENTVSYNC);
    // `-1` means "get the default" display number in this case, `0` - for no flags
    if (!renderer) {
        Logger::Error("Error creating SDL renderer.");
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
            case SDL_QUIT:  // event when user tries to close the window
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                    isRunning = false;
                break;
        }
    }
}

void Game::LoadLevel(int level) {
    // Add the systems that need to be processed
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();

    // Adding assets to the asset store
    assetStore->AddTexture(renderer, "tank-image", "assets/images/tank-panther-right.png");
    assetStore->AddTexture(renderer, "truck-image", "assets/images/truck-ford-down.png");
    // assetStore->AddTmxFile(renderer, "jungle", "assets/tests/test1.tmx");
    // assetStore->AddTmxFile(renderer, "jungle", "assets/tilemaps/demo.tmx");
    assetStore->AddTmxFile(renderer, "jungle", "assets/tilemaps/map-jungle.tmx");

    /*// ---- Load the tilemap - START -------------------------------------------------------------
    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("assets/tilemaps/jungle.map");

    // Validate file
    if (!mapFile.is_open()) {
        Logger::Error("Error opening the tilemap file.");
        return;
    }

    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile  = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y *
    (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, srcRectX,
    srcRectY);
        }
    }
    mapFile.close();
    // ---- Load the tilemap - END ---------------------------------------------------------------*/

    Entity tmxTemp = registry->CreateEntity();
    tmxTemp.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(0.8f, 0.8f));
    tmxTemp.AddComponent<SpriteComponent>("jungle", 0, 0, 0, 0, SpriteType::TILED);
    tmxTemp.GetComponent<SpriteComponent>().tileLayerIndexes = {0};

    // Create an entity
    Entity tank = registry->CreateEntity();
    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);
    // tank.RemoveComponent<RigidBodyComponent>();

    // Create an entity
    Entity truck = registry->CreateEntity();
    truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);

    Entity skyTemp = registry->CreateEntity();
    skyTemp.AddComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(0.8f, 0.8f));
    skyTemp.AddComponent<SpriteComponent>("jungle", 0, 0, 0, 0, SpriteType::TILED);
    skyTemp.GetComponent<SpriteComponent>().tileLayerIndexes = {1, 2, 3};
}

void Game::Setup() {
    LoadLevel(1);
}

void Game::Update() {
    // Cap framerate
    int timeToWait = MS_PER_FRAME - (SDL_GetTicks() - msPrevFrame);  // calculate how much to wait
    if (timeToWait > 0 && timeToWait <= MS_PER_FRAME)
        SDL_Delay(timeToWait);  // wait

    // Delta time - difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - msPrevFrame) / 1000.0;

    // Store the current frame time
    msPrevFrame = SDL_GetTicks();

    // Invoke all the systems that we need to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);

    // Invoke all the systems that we need to update
    registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

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
