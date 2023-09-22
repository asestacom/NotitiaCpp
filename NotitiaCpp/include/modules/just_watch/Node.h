#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "modules/just_watch/Content.h"
#include "modules/just_watch/WatchNowOffer.h"

struct Node {
    std::string id;
    std::string object_id;
    std::string object_type;
    Content content;
    WatchNowOffer watch_now_offer;
    std::string seenlist_entry;
    std::string node_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Node{
            .id = item["id"].to_string(),
            .object_id = item["objectId"].to_string(),
            .object_type = item["objectType"].to_string(),
            .content = Content::create_object(item["content"]),
            .watch_now_offer = WatchNowOffer::create_object(item["watchNowOffer"]),
            .seenlist_entry = item["seenlistEntry"].to_string(),
            .node_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["id"] = id;
        json["objectId"] = object_id;
        json["objectType"] = object_type;
        json["content"] = content.to_variant();
        json["watchNowOffer"] = watch_now_offer.to_variant();
        json["seenlistEntry"] = seenlist_entry;
        json["__typename"] = node_typename;
        return json;
    }
};
