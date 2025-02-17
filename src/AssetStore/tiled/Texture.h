#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>  // todo replace with forward declaration, see: IWYU
#include <string>

namespace tiled {
    class Texture {
    public:
        Texture();
        ~Texture();

        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;

        bool loadFromFile(const std::string& path, SDL_Renderer* renderer);
        SDL_Point getSize() const;

        operator SDL_Texture*();

    private:
        SDL_Texture* m_texture;
        SDL_Point m_size;
    };
}  // namespace tiled

#endif  // TEXTURE_H
