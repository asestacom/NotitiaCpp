#pragma once

#include <string>
#include <vector>
#include "common/Json.h"

struct Scoring {
    std::string imdb_score;
    std::string scoring_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Scoring{
            .imdb_score = item["imdbScore"].to_string(),
            .scoring_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["imdbScore"] = imdb_score;
        json["__typename"] = scoring_typename;
        return json;
    }
};
