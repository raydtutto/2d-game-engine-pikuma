#include "AsepriteObject.h"

#include "AssetStore/AssetStore.h"

#include <Logger/Logger.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool AsepriteObject::load(const std::string& jsonPath) {
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    if (data.is_null() || !data.is_object()) {
        Logger::Error("Json file doesn't exist.");
        return false;
    }
    if (!data["meta"].is_object()) {
        Logger::Error("Meta isn't an object.");
        return false;
    }

    // Read meta data
    auto meta = data["meta"];
    Logger::Log("Aseprite json opened.");

    size.first = meta["size"]["w"].get<int>();
    size.second = meta["size"]["h"].get<int>();
    scale = std::atof(meta["scale"].get<std::string>().c_str());
    imagePath = meta["image"].get<std::string>();
    if (imagePath.empty())
        return false;

    // Read frames data
    auto framesArray = data["frames"];
    for (auto item : framesArray) {
        if (!item.is_object()) {
            Logger::Warning("JSON data is incorrect.");
            continue;
        }

        FrameObject frame;
        if (!item["frame"].is_object()) {
            Logger::Warning("JSON frame data is incorrect.");
            continue;
        }
        frame.objectFrames.x = item["frame"]["x"].get<int>();
        frame.objectFrames.y = item["frame"]["y"].get<int>();
        frame.objectFrames.width = item["frame"]["w"].get<int>();
        frame.objectFrames.height = item["frame"]["h"].get<int>();
        frame.spriteSize.first = item["sourceSize"]["w"].get<int>();
        frame.spriteSize.second = item["sourceSize"]["h"].get<int>();
        frame.frameDuration = item["duration"].get<int>();

        frames.push_back(frame);
    }

    // Read animation tags
    for (auto item : meta["frameTags"]) {
        std::string name = item["name"].get<std::string>();
        // Check frame range
        if (item["from"].get<int>() < 0 || item["from"].get<int>() >= framesArray.size()
            || item["to"].get<int>() < 0 || item["to"].get<int>() >= framesArray.size()) {
            Logger::Warning("JSON frame range is incorrect.");
            continue;
        }
        frameTags[name] = std::make_pair(item["from"].get<int>(), item["to"].get<int>());
    }

    Logger::Log("All Aseprite data extracted.");
    return true;
}