#pragma once

#include <string>
#include <vector>
#include "common/Json.h"

struct Package {
    std::string package_id;
    std::string clear_name;
    std::string package_typename;

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Package{
            .package_id = item["packageId"].to_string(),
            .clear_name = item["clearName"].to_string(),
            .package_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["packageId"] = package_id;
        json["clearName"] = clear_name;
        json["__typename"] = package_typename;
        return json;
    }
};
