#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"

struct Genre {
    std::string short_name;
    std::string genre_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Genre{
            .short_name = item["shortName"].to_string(),
            .genre_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["shortName"] = short_name;
        json["__typename"] = genre_typename;
        return json;
    }

    using Genres = JsonVariantVectorItems<Genre>;
};
