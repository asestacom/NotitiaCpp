#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "modules/just_watch/Clip.h"
#include "modules/just_watch/Credit.h"
#include "modules/just_watch/Genre.h"
#include "modules/just_watch/Scoring.h"
#include "modules/just_watch/ExternalIds.h"

struct Content {
    std::string title;
    std::string original_release_year;
    std::string full_path;
    std::string age_certification;
    std::vector<std::string> production_countries;
    Clip::Clips clips;
    Credit::Credits credits;
    Genre::Genres genres;
    Scoring scoring;
    ExternalIds external_ids;
    std::string short_description;
    std::string runtime;
    std::string poster_url;
    std::string content_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Content{
            .title = item["title"].to_string(),
            .original_release_year = item["originalReleaseYear"].to_string(),
            .full_path = item["fullPath"].to_string(),
            .age_certification = item["ageCertification"].to_string(),
            .production_countries = item["productionCountries"].to_string_vector(),
            .clips = item["clips"].to_vector(),
            .credits = item["credits"].to_vector(),
            .genres = item["genres"].to_vector(),
            .scoring = Scoring::create_object(item["scoring"]),
            .external_ids = ExternalIds::create_object(item["externalIds"]),
            .short_description = item["shortDescription"].to_string(),
            .runtime = item["runtime"].to_string(),
            .poster_url = item["posterUrl"].to_string(),
            .content_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["title"] = title;
        json["originalReleaseYear"] = original_release_year;
        json["fullPath"] = full_path;
        json["ageCertification"] = age_certification;
        json["productionCountries"] = string_array_to_variant(production_countries),
        json["clips"] = clips;
        json["credits"] = credits;
        json["genres"] = genres;
        json["scoring"] = scoring.to_variant();
        json["externalIds"] = external_ids.to_variant();
        json["shortDescription"] = short_description;
        json["runtime"] = runtime;
        json["posterUrl"] = poster_url;
        json["__typename"] = content_typename;
        return json;
    }

    [[nodiscard]] static JsonVariant string_array_to_variant(const std::vector<std::string>& items) {
        JsonVariantVector json_array;
        for (const auto &item : items)
            json_array.emplace_back(item);
        return static_cast<JsonVariant>(json_array);
    }
};
