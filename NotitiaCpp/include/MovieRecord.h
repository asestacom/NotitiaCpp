#pragma once

#include "common/Json.h"
#include "modules/tmdb/Tmdb.h"
#include "modules/imdb/Imdb.h"
#include "modules/just_watch/JustWatch.h"
#include "NodeRecord.h"

struct MovieRecord {
    std::string title;
    std::string year;
    NodeRecord::NodeRecords node;
    bool imdb_confirmed{ false };
    Imdb::Imdbs imdb;
    bool justwatch_confirmed{ false };
    Justwatch::Justwatchs justwatch;
    bool tmdb_confirmed{ false };
    Tmdb::Tmdbs Tmdb;

    template<typename T> [[nodiscard]] static auto create_object(T&& movie) {
        return MovieRecord{
            .title = movie["title"].to_string(),
            .year = movie["year"].to_string(),
            .node = movie["node"].to_vector(),
            .imdb_confirmed = movie["imdb_confirmed"].to_bool(),
            .imdb = movie["imdb"].to_vector(),
            .justwatch_confirmed = movie["justwatch_confirmed"].to_bool(),
            .justwatch = movie["justwatch"].to_vector(),
            .tmdb_confirmed = movie["tmdb_confirmed"].to_bool(),
            .Tmdb = movie["Tmdb"].to_vector()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["title"] = title;
        json["year"] = year;
        json["node"] = node;
        json["imdb_confirmed"] = imdb_confirmed;
        json["imdb"] = imdb;
        json["justwatch_confirmed"] = justwatch_confirmed;
        json["justwatch"] = justwatch;
        json["tmdb_confirmed"] = tmdb_confirmed;
        json["Tmdb"] = Tmdb;

        return json;
    }

    using MovieRecords = JsonVariantVectorItems<MovieRecord>;

    static JsonVariant to_variant(MovieRecords& movies) {
        JsonVariantVector json_array;
        for (auto& movie : movies)
            json_array.emplace_back(movie.to_variant());
        return static_cast<JsonVariant>(json_array);
    }
};
