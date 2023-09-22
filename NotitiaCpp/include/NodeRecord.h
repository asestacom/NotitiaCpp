#pragma once

#include "common/Json.h"

struct NodeRecord {
    std::string node_name;
    std::string full_node_name;
    std::string extra;
    std::string format;
    std::tuple<std::string, std::string> resolution;

    [[nodiscard]] static NodeRecord create_object(JsonVariant nodeRecord) {
        return NodeRecord{
            .node_name = nodeRecord["node_name"].to_string(),
            .full_node_name = nodeRecord["full_node_name"].to_string(),
            .extra = nodeRecord["extra"].to_string(),
            .format = nodeRecord["format"].to_string(),
            .resolution = to_tuple(nodeRecord["resolution"].to_vector())
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["node_name"] = node_name;
        json["full_node_name"] = full_node_name;
        json["extra"] = extra;
        json["format"] = format;
        auto &[width, height] = resolution;
        json["resolution"] = width + "," + height;
        return json;
    }

    using NodeRecords = JsonVariantVectorItems<NodeRecord>;
};
