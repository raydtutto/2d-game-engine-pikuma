#ifndef ASEPRITEOBJECT_H
#define ASEPRITEOBJECT_H

#include <map>
#include <string>
#include <tuple>
#include <vector>

// Stores Aseprite json "frame": { "x": 0, "y": 0, "w": 224, "h": 224 }
struct FrameData {
    int x;
    int y;
    int width;
    int height;

    FrameData(int _x, int _y, int _width, int _height)
        : x(_x)
        , y(_y)
        , width(_width)
        , height(_height) {
    }

    FrameData()
        : x(0)
        , y(0)
        , width(0)
        , height(0) {
    }
};

// Stores frame data
struct FrameObject {
    std::shared_ptr<FrameData> objectFrames;
    std::pair<int, int> spriteSize;
    int frameDuration;

    FrameObject()
        : objectFrames(std::make_shared<FrameData>())
        , spriteSize({ 0, 0 })
        , frameDuration(0) {}
};

// Stores Aseprite json data
struct AsepriteObject {
    std::pair<int, int> size;
    float scale;
    std::string imageName;
    std::shared_ptr<std::map<std::string, std::pair<int, int>>> frameTags;  // { "name": "***",
                                                                            // "from": 2, "to": 4}
    std::vector<std::shared_ptr<FrameObject>> frames;

    AsepriteObject()
        : size{ 0, 0 }
        , scale(1.0f)
        , frameTags(std::make_shared<std::map<std::string, std::pair<int, int>>>()) {
    }

    bool Load(const std::string& jsonPath);
};

#endif  // ASEPRITEOBJECT_H
