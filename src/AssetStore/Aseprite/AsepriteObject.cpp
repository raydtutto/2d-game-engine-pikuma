#include "AsepriteObject.h"

#include "AssetStore/AssetStore.h"

#include <spdlog/spdlog.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool AsepriteObject::Load(const std::string& jsonPath) {
    std::ifstream f(jsonPath);
    json data = json::parse(f);
    if (data.is_null() || !data.is_object()) {
        spdlog::error("Json file doesn't exist: {}", jsonPath);
        return false;
    }
    if (!data["meta"].is_object()) {
        spdlog::error("Meta data is incorrect: {}", jsonPath);
        return false;
    }

    // Read meta data
    auto meta = data["meta"];
    spdlog::info("Aseprite json opened.");

    if (!meta.contains("size") || !meta["size"].is_object() || !meta.contains("scale")
        || (!meta["scale"].is_string() && !meta["scale"].is_number()) || !meta.contains("image")
        || !meta["image"].is_string()) {
        spdlog::error("Aseprite meta data is incorrect: {}", jsonPath);
        return false;
    }
    size.first = meta["size"]["w"].get<int>();
    size.second = meta["size"]["h"].get<int>();

    // Handle scale as a string or a number
    if (meta["scale"].is_string()) {
        scale = std::atof(meta["scale"].get<std::string>().c_str()); // Convert string to float
    } else {
        scale = meta["scale"].get<float>(); // Directly extract number
    }

    imageName = meta["image"].get<std::string>();

    // Read frames data
    auto framesArray = data["frames"];
    for (auto item : framesArray) {
        if (!item.contains("frame") || !item.contains("sourceSize") || !item.contains("duration")) {
            spdlog::error("JSON frames data is incorrect: {}", jsonPath);
            return false;
        }
        if (!item["frame"].is_object() || !item["sourceSize"].is_object()
            || !item["duration"].is_number()) {
            spdlog::warn("JSON frames data is incorrect: {}", jsonPath);
            continue;
        }
        // Allocate memory
        auto frame = std::make_shared<FrameObject>();

        // Init pointers
        frame->objectFrames = std::make_shared<FrameData>();

        // Set data
        frame->objectFrames->x = item["frame"]["x"].get<int>();
        frame->objectFrames->y = item["frame"]["y"].get<int>();
        frame->objectFrames->width = item["frame"]["w"].get<int>();
        frame->objectFrames->height = item["frame"]["h"].get<int>();
        frame->spriteSize.first = item["sourceSize"]["w"].get<int>();
        frame->spriteSize.second = item["sourceSize"]["h"].get<int>();
        frame->frameDuration = item["duration"].get<int>();

        // Add data
        frames.push_back(frame);
    }

    // Read animation tags
    if (!meta.contains("frameTags") || !meta["frameTags"].is_array()) {
        spdlog::error("JSON does not contain valid frameTags: {}", jsonPath);
        return false;
    }
    for (const auto& item : meta["frameTags"]) {
        if (!item.contains("name") || !item["name"].is_string() || !item.contains("from")
            || !item["from"].is_number_integer() || !item.contains("to")
            || !item["to"].is_number_integer()) {
            spdlog::warn("Invalid frameTag entry in JSON: {}", jsonPath);
            continue;
        }
        auto name = item["name"].get<std::string>();

        // Check frame range
        int start = item["from"].get<int>();
        int end = item["to"].get<int>();
        if (start < 0 || start >= static_cast<int>(framesArray.size()) || end < 0
            || end >= static_cast<int>(framesArray.size())) {
            spdlog::warn("JSON frame range is incorrect: {}", jsonPath);
            continue;
        }
        frameTags->emplace(name, std::make_pair(start, end));
    }

    spdlog::info("All Aseprite data extracted: {}", jsonPath);
    return true;
}