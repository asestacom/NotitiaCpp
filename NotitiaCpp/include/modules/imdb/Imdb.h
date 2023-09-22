#pragma once

#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"

struct Imdb {
    std::string tconst;
    std::string titleType; // "movie", "short", "tvSeries", "tvMiniSeries", "tvMovie"
    std::string primaryTitle;
    std::string originalTitle;
    std::string startYear;
    std::string runtimeMinutes;
    std::string genres;
    std::string ratings;

    [[nodiscard]] static Imdb create_object(JsonVariant movie) {
        return Imdb{
            .tconst = movie["tconst"].to_string(),
            .titleType = movie["titleType"].to_string(),
            .primaryTitle = movie["primaryTitle"].to_string(),
            .originalTitle = movie["originalTitle"].to_string(),
            .startYear = movie["startYear"].to_string(),
            .runtimeMinutes = movie["runtimeMinutes"].to_string(),
            .genres = movie["genres"].to_string(),
            .ratings = movie["ratings"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["tconst"] = tconst;
        json["titleType"] = titleType;
        json["primaryTitle"] = primaryTitle;
        json["originalTitle"] = originalTitle;
        json["year"] = startYear;
        json["runtimeMinutes"] = escape_quotes(runtimeMinutes);
        json["genres"] = genres;
        json["ratings"] = ratings;
        return json;
    }

    using Imdbs = JsonVariantVectorItems<Imdb>;

    std::string escape_quotes(const std::string text) const {
        return text;
    }
};
