#include "Texture.h"
#include <SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "Logger/Logger.h"
#include "stb_image.h"

#include <iostream>

using namespace tiled;

Texture::Texture() {
}

Texture::~Texture() {
}

bool Texture::loadFromFile(const std::string& path, SDL_Renderer* renderer) {
    if (!renderer || path.empty()) {
        Logger::Error("Tiled texture file doesn't exist.");
        return false;
    }

    std::int32_t x = 0;
    std::int32_t y = 0;
    std::int32_t c = 0;
    unsigned char* data = stbi_load(path.c_str(), &x, &y, &c, 0);

    if (data)
    {
        std::int32_t pitch = x * c;
        pitch = (pitch + 3) & ~3;

        constexpr std::int32_t rmask = 0x000000ff;
        constexpr std::int32_t gmask = 0x0000ff00;
        constexpr std::int32_t bmask = 0x00ff0000;
        const std::int32_t amask = c == 4 ? 0xff000000 : 0;

        auto* surface = SDL_CreateRGBSurfaceFrom(data, x, y, c * 8, pitch, rmask, gmask, bmask, amask);

        if (!surface)
        {
            Logger::Error("Unable to create texture surface.");
            stbi_image_free(data);
            return false;
        }

        m_texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);
        stbi_image_free(data);

        if (!m_texture)
        {
            Logger::Error("Failed to create texture for " + path);
            return false;
        }

        //sets alpha blending
        //SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
        m_size.x = x;
        m_size.y = y;

        return true;
    }

    return false;
}

SDL_Point Texture::getSize() const {
    return m_size;
}

Texture::operator struct SDL_Texture *() {
    return m_texture;
}