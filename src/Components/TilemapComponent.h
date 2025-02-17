#ifndef TILEDCOMPONENT_H
#define TILEDCOMPONENT_H

#include "Logger/Logger.h"
#include <SDL.h>
#include <vector>


struct TilemapComponent {
    std::string tileMapId;

    TilemapComponent(std::string _tileMapId = "") {
        if (!_tileMapId.empty())
            tileMapId = _tileMapId;
    }

    // TilemapComponent(std::string _tilesetPath = "")
    //     : tilesetPath(std::move(_tilesetPath))
    //     , width(0)
    //     , height(0)
    //     , tiledTexture(nullptr) {
    //     if (tilesetPath.empty()) {
    //         Logger::Error("Tileset doesn't exist.");
    //         return;
    //     }
    //     tmx::Map map;
    //     if (map.load(tilesetPath)) {
    //         const auto& tiesets = map.getTilesets();
    //         const auto& layers = map.getLayers();
    //         auto count = layers.size();
    //         Logger::Log("Layers = " + count);
    //     }
    // }
};

#endif  // TILEDCOMPONENT_H
