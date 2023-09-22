#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"
#include "modules/just_watch/Node.h"

struct Edge {
    std::string cursor;
    Node node;
    std::string edge_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Edge{
            .cursor = item["cursor"].to_string(),
            .node = Node::create_object(item["node"]),
            .edge_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["cursor"] = cursor;
        json["node"] = node.to_variant();
        json["__typename"] = edge_typename;
        return json;
    }

    using Edges = JsonVariantVectorItems<Edge>;
};
