#pragma once

#include <string>
#include <vector>
#include "common/Json.h"
#include "common/JsonVariantVectorItems.h"

struct Credit {
    std::string role; // ACTOR, DIRECTOR
    std::string name;
    std::string character_name;
    std::string person_id;
    std::string credit_typename; // CREDIT

    template<typename T> [[nodiscard]] static auto create_object(T&& item) {
        return Credit{
            .role = item["role"].to_string(),
            .name = item["name"].to_string(),
            .character_name = item["characterName"].to_string(),
            .person_id = item["personId"].to_string(),
            .credit_typename = item["__typename"].to_string()
        };
    }

    [[nodiscard]] JsonVariant to_variant() const {
        JsonVariantMap json;
        json["role"] = role;
        json["name"] = name;
        json["characterName"] = character_name;
        json["personId"] = person_id;
        json["__typename"] = credit_typename;
        return json;
    }

    using Credits = JsonVariantVectorItems<Credit>;
};
