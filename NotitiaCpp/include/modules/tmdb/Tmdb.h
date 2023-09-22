#pragma once

#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"

struct Tmdb {
    std::string id;
    std::string imdb_id;
    std::string overview;
    std::string poster_path;
    std::string vote_average;

    [[nodiscard]] static Tmdb create_object(JsonVariant movie) {
        return Tmdb{
            .id = movie["id"].to_string(),
            .imdb_id = movie["imdb_id"].to_string(),
            .overview = movie["overview"].to_string(),
            .poster_path = movie["poster_path"].to_string(),
            .vote_average = movie["vote_average"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["id"] = id;
        json["imdb_id"] = imdb_id;
        json["overview"] = overview;
        json["poster_path"] = poster_path;
        json["vote_average"] = vote_average;
        return json;
    }

    using Tmdbs = JsonVariantVectorItems<Tmdb>;
};