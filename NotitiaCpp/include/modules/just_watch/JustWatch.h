#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"
#include "modules/just_watch/Edge.h"

struct Justwatch {
    std::string id;
    Edge edge;

    [[nodiscard]] static Justwatch create_object(JsonVariant movie) {
        return Justwatch{
            .id = movie["id"].to_string(),
            .edge = Edge::create_object(movie["edge"])
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["id"] = id;
        json["edge"] = edge.to_variant();
        return json;
    }

    using Justwatchs = JsonVariantVectorItems<Justwatch>;
};
