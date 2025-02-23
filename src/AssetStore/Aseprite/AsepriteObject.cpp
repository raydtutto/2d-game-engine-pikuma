#include "AsepriteObject.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <Logger/Logger.h>

using json = nlohmann::json;

AsepriteObject::AsepriteObject(const std::string& jsonPath) {
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    if (data.is_null() || !data.is_object()) {
        Logger::Error("Json file doesn't exist.");
        return;
    }
    if (!data["meta"].is_object()) {
        Logger::Error("Meta isn't an object.");
        return;
    }

    // Read meta data
    auto meta = data["meta"];
    Logger::Log("Aseprite json opened.");

    size.first = meta["size"]["w"].get<int>();
    size.second = meta["size"]["h"].get<int>();
    scale = std::atof(meta["scale"].get<std::string>().c_str());
    imagePath = meta["image"].get<std::string>();

    // Read frames data
    auto framesArray = data["frames"];
    for (auto item : framesArray) {
        if (!item.is_object()) {
            Logger::Warning("JSON data isn't valid.");
            continue;
        }
        FrameObject frame;
        if (!frame.objectFrames.x || !frame.objectFrames.y || !frame.objectFrames.width || !frame.objectFrames.height) {
            Logger::Warning("JSON frame data is incorrect.");
            continue;
        }
        frame.objectFrames.x = item["frame"]["x"].get<int>();
        frame.objectFrames.y = item["frame"]["y"].get<int>();
        frame.objectFrames.width = item["frame"]["w"].get<int>();
        frame.objectFrames.height = item["frame"]["h"].get<int>();

        if (!frame.spriteSize.first || !frame.spriteSize.second) {
            Logger::Warning("JSON sourceSize data is incorrect.");
            continue;
        }
        frame.spriteSize.first = item["sourceSize"]["w"].get<int>();
        frame.spriteSize.second = item["sourceSize"]["h"].get<int>();

        if (!frame.frameDuration) {
            Logger::Warning("JSON duration data is incorrect.");
            continue;
        }
        frame.frameDuration = item["duration"].get<int>();

        frames.push_back(frame);
    }

    for (auto item : meta["frameTags"]) {
        std::string name = item["name"].get<std::string>();
        // todo check range
        frameTags[name] = std::make_pair(item["from"].get<int>(), item["to"].get<int>());
    }

    Logger::Log("All Aseprite data extracted.");
}