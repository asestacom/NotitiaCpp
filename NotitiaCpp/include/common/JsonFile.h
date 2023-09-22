#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>
#include <variant>
#include "Utils.h"
#include "Json.h"

class JsonFile
{
public:
    [[nodiscard]] static std::string read_file(const std::filesystem::path path) {
        std::ifstream file_content(path, std::ios::in | std::ios::binary);
        const auto sz = std::filesystem::file_size(path);
        std::string result(sz, '\0');
        file_content.read(result.data(), static_cast<std::streamsize>(sz));
        return result;
    }

    [[nodiscard]] static JsonVariant read_json(const std::filesystem::path path) {
        JsonVariant json;
        std::string str = read_file(path);
        read_level(str.begin(), str.end(), json);
        return json[0]; // TODO: [0]
    }

    template<typename T>
    [[nodiscard]] static JsonVariant read_json(const std::vector<T> paths) {
        JsonVariantVector result;
        for (const auto& path : paths) {
            auto json = read_json(path);
            auto& items = std::get<JsonVariantVector>(json);
            for (const auto& item : items)
                result.emplace_back(item);
        }
        return result;
    }

    static void write_level(auto& out, const JsonVariant& json, size_t level = 0) {
        if (std::holds_alternative<JsonVariantVector>(json)) {
            out << write_ident(level);
            out << "[" << "\n";
            size_t index{ 0 };
            auto& items = std::get<JsonVariantVector>(json);
            for (const auto& json_item : items)
            {
                write_level(out, json_item, level + 1);
                if (++index < items.size())
                    out << ",";
                out << "\n";
            }
            out << write_ident(level);
            out << "]";
        }
        else if (std::holds_alternative<JsonVariantMap>(json)) {
            out << write_ident(level);
            out << "{" << "\n";
            size_t index{ 0 };
            auto& items = std::get<JsonVariantMap>(json);
            for (const auto& [key, value] : items)
            {
                out << write_ident(level);
                out << write_quotes(key) << ":";
                write_level(out, value, level + 1);
                if (++index < items.size())
                    out << ",";
                out << "\n";
            }
            out << write_ident(level);
            out << "}";
        }
        else if (std::holds_alternative<int>(json)) {
            out << write_quotes(std::get<int>(json));
        }
        else if (std::holds_alternative<std::string>(json)) {
            out << write_quotes(std::get<std::string>(json));
        }
        else if (std::holds_alternative<bool>(json)) {
            out << (std::get<bool>(json) ? "true" : "false");
        }
        else if (std::holds_alternative<double>(json)) {
            // TODO: IGNORE if index() == 0 == double but do it differently
            //output << write_quotes(std::get<double>(json));
            out << "[]";
        }
        else {
            out << "<<UNKNOWN>>";
        }
    }

    static const auto to_utf8(const std::string &codepage_str) {
        return std::u8string(codepage_str.begin(), codepage_str.end());
    }
    static const auto from_utf8(const std::u8string& codepage_str) {
        return std::string(codepage_str.begin(), codepage_str.end());
    }

    static void save_json(auto &output, const JsonVariant &json, size_t level = 0) {
        write_level(output, json, level);
    }

private:
    static void add_array_item_if(const std::string key, const std::string value, const bool in_value, JsonVariant &json, const size_t index) {
        if (key.size() && !value.size() && !in_value)
            json[index] = key;
    }

    // first will work by value for begin() but by ref for recursive calls
    static void read_level(auto&& current, const auto last, JsonVariant &json, size_t index = 0) {
        bool in_value{ false };
        std::string key{}, value{};
        while (current != last) {
            if (*current != ' ' && *current != '\n' && *current != '\r' && *current != '\t') {
                if (in_value) {
                    if (*current == 't' || *current == 'T' || *current == 'f' || *current == 'F') { // boolean true/false
                        bool is_true = *current == 't' || *current == 'T';
                        current += static_cast<const __int64>(is_true ? sizeof("true") : sizeof("false")) - 1; // assumes that it's well formatted
                        json[key] = is_true;
                        key = "";
                        value = "";
                        in_value = false;
                    }
                    else if (*current >= '0' && *current <= '9') { // numbers
                        value = "";
                        while ((*current >= '0' && *current <= '9') || *current == '.') { // it can have . anywhere
                            value += *current++;
                        }
                        json[key] = value;
                        key = "";
                        value = "";
                        in_value = false;
                    }
                }

                switch (*current)
                {
                case '"':
                case '\'':
                {
                    // "name":"value"
                    std::string token = get_text_in_quotes(++current, last, *current);
                    if (!in_value) {
                        key = token;
                    }
                    else {
                        value = token;
                        json[key] = value;
                        key = "";
                        value = "";
                        in_value = false;
                    }
                    current += static_cast<const __int64>(token.size());
                }
                break;
                case '}':
                    return;
                case '{': [[fallthrough]];
                case '[':
                    read_level(++current, last, key.size() ? json[key] : json[index]);
                    break;
                case ']':
                    add_array_item_if(key, value, in_value, json, index);
                    return;
                case ',':
                    // new line/value
                    add_array_item_if(key, value, in_value, json, index);
                    key = "";
                    value = "";
                    in_value = false;
                    index++;
                    break;
                case ':':
                    in_value = true;
                    break;
                [[unlikely]] default:
                    break;
                }
            }
            ++current;
        }
    }

    static auto write_ident(size_t level) {
        return std::string(level, '\t');
    }
    static auto write_quotes(auto &text) {
        std::stringstream result;
        result << "\"" << text << "\"";
        return result.str();
    }
};
