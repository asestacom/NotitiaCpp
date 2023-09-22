#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "modules/just_watch/Package.h"

struct WatchNowOffer {
    std::string id;
    std::string standard_web_url;
    Package package;
    std::string presentation_type;
    std::string monetization_type;
    std::string watch_now_offer_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return WatchNowOffer{
            .id = item["id"].to_string(),
            .standard_web_url = item["standardWebURL"].to_string(),
            .package = Package::create_object(item["package"]),
            .presentation_type = item["presentationType"].to_string(),
            .monetization_type = item["monetizationType"].to_string(),
            .watch_now_offer_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["id"] = id;
        json["standardWebURL"] = standard_web_url;
        json["package"] = package.to_variant();
        json["presentationType"] = presentation_type;
        json["monetizationType"] = monetization_type;
        json["__typename"] = watch_now_offer_typename;
        return json;
    }
};
