#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"

struct Clip {
    std::string external_id;
    std::string clip_typename; // CLIP

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Clip{
            .external_id = item["externalId"].to_string(),
            .clip_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["externalId"] = external_id;
        json["__typename"] = clip_typename;
        return json;
    }

    using Clips = JsonVariantVectorItems<Clip>;
};
