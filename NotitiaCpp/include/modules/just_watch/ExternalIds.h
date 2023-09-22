#pragma once

#include <string>
#include <vector>
#include "common/Json.h"

struct ExternalIds {
    std::string imdb_id;
    std::string external_ids_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return ExternalIds{
            .imdb_id = item["imdbId"].to_string(),
            .external_ids_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["imdbId"] = imdb_id;
        json["__typename"] = external_ids_typename;
        return json;
    }
};
