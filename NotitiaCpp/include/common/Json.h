#pragma once

#include <vector>
#include <variant>
#include <map>
#include <cassert>
#ifdef _WIN32
#include <windows.h>
	#undef min
#endif
#include "Utils.h"

/// Examples:
/// JsonVariant json;
/// json["item"] = 3;
/// json["array"]["level2"] = "aaa";
/// 
/// JsonVariant json;
/// json[0] = 123; // vector

template<typename T> using JsonTypeMap = std::map<std::string, T>;
template<typename T> using JsonTypeVector = std::vector<T>;
template<typename T> using JsonType = std::variant<double, int, bool, std::string, JsonTypeMap<T>, JsonTypeVector<T>>;

struct JsonVariant;
template<typename T> inline JsonVariant to_variant(const T &item) {
	return item.to_variant();
}
struct JsonVariant;
template<typename T> inline JsonVariant to_variant(const T&& item) {
	return item.to_variant();
}

struct JsonVariant : JsonType<JsonVariant> {
	using JsonVariantVector = JsonTypeVector<JsonVariant>;
	using JsonVariantMap = JsonTypeMap<JsonVariant>;

	JsonVariant() = default;

	template<typename T>
	JsonVariant(T&& value) : JsonType<JsonVariant>(std::forward<T>(value)) {}

	// element by name (create or lookup)
	auto &operator[](std::string key) noexcept {
		return get_or_create<JsonVariantMap>().operator [](std::forward<decltype(key)>(key));
	}

	// element by index
	auto& operator [](size_t array_index) noexcept {
		auto& var_vector = get_or_create<JsonVariantVector>();
		if (var_vector.size() <= array_index)
			var_vector.emplace_back(JsonVariant()); // create a new element in vector
		return var_vector[array_index];
	}

	template<typename T>
	auto& operator =(std::vector<T>&& items) {
		auto &var_vector = get_or_create<JsonVariantVector>();
		for (const auto &item : items)
			var_vector.emplace_back(to_variant(item));
		return *this;
	}

	template<typename T>
	auto &operator =(const std::vector<T> &items) {
		auto &var_vector = get_or_create<JsonVariantVector>();
		for (const auto &item : items)
			var_vector.emplace_back(to_variant(item));
		return *this;
	}

	template<typename T>
	T convert_to() {
		if (std::holds_alternative<T>(*this))
			return std::get<T>(*this);
		return default_value<T>;
	}

	std::string to_string() {
		return convert_to<std::string>();
	}
	int to_int() {
		return convert_to<int>();
	}
	bool to_bool() {
		return convert_to<bool>();
	}
	JsonVariantVector to_vector() {
		return convert_to<JsonVariantVector>();
	}
	JsonVariantMap to_map() {
		return convert_to<JsonVariantMap>();
	}
	std::vector<std::string> to_string_vector() {
		std::vector<std::string> result;
		for (auto& item : to_vector())
			result.emplace_back(item.to_string());
		return result;
	}

private:
	template<typename T>
	T &get_or_create() {
		if (!std::holds_alternative<T>(*this)) {
			assert(index() == 0);
			return emplace<T>();
		}
		return std::get<T>(*this);
	}
};

using JsonVariantVector = JsonVariant::JsonVariantVector;
using JsonVariantMap = JsonVariant::JsonVariantMap;

template<typename T> T default_value{};
template<> constexpr double default_value<double> { 0.0 };
template<> constexpr int default_value<int> { 0 };
template<> constexpr bool default_value<bool> { false };
template<> std::string default_value<std::string>{};
template<> JsonVariantMap default_value<JsonVariantMap> = JsonVariantMap{};
template<> JsonVariantVector default_value<JsonVariantVector> = JsonVariantVector{};

inline std::tuple<std::string, std::string> to_tuple(JsonVariantVector&& tuple) {
	if (tuple.size() > 1)
		return std::tuple<std::string, std::string>(tuple[0].to_string(), tuple[1].to_string());
	return std::tuple<std::string, std::string>();
}
