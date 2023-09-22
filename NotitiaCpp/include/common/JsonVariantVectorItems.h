#pragma once

#include <vector>

template<typename T> struct JsonVariantVectorItems : std::vector<T> {
    JsonVariantVectorItems() = default;
    JsonVariantVectorItems(JsonVariantVectorItems<T>&&) = default;
    JsonVariantVectorItems(const JsonVariantVectorItems<T>&) = default;
    
    JsonVariantVectorItems(JsonVariantVector&& items) {
        emplace_items(std::forward<JsonVariantVector>(items));
    }
    auto& operator =(JsonVariantVector&& items) {
        return emplace_items(std::forward<JsonVariantVector>(items));
    }
    auto& operator =(const JsonVariantVectorItems<T>& items) {
        return emplace_items(std::forward<JsonVariantVector>(items));
    }

    JsonVariantVectorItems& emplace_items(JsonVariantVector&& items) {
        this->clear();
        for (auto &item : items)
            this->emplace_back(T::create_object(item)); // TODO: create_object
        return *this;
    }
};
