#pragma once

//#define ONE_THREAD_IN_SEARCH_INDICES
#define USING_ASYNC

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

#include <thread>
#include <future>
#include <mutex>

#include "PrintDuration.h"

inline auto split(const std::string& text, char delim) noexcept {
    std::vector<std::string> result;
    std::stringstream ss(text);
    std::string item;
    while (getline(ss, item, delim)) {
        result.emplace_back(item);
    }
    return result;
}

// trim from start
inline auto& ltrim(auto& text) noexcept {
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), [](auto ch) {
        return !std::isspace(ch);
        }));
    return text;
}

// trim from end
inline auto& rtrim(auto& text) noexcept {
    text.erase(std::find_if(text.rbegin(), text.rend(), [](auto ch) {
        return !std::isspace(ch);
        }).base(), text.end());
    return text;
}

inline auto tolower(auto& text) noexcept {
    std::transform(text.begin(), text.end(), text.begin(),
        [](auto c) { return static_cast<char>(std::tolower(c)); }
    );
    return text;
}

inline std::string get_line_by_index(const size_t result, const std::string& data) noexcept {
    if (result != std::string::npos) {
        auto begin_of_line = data.rfind("\n", result);
        auto end_of_line = data.find("\n", result);
        if (begin_of_line != std::string::npos) {
            while (data[begin_of_line] == '\n' || data[begin_of_line] == '\r')
                ++begin_of_line;
            if (begin_of_line != std::string::npos) {
                return data.substr(begin_of_line, end_of_line - begin_of_line);
            }
            return data.substr(begin_of_line);
        }
    }
    return "";
}

inline auto title_ready(const auto& current_title) noexcept {
    auto title = current_title;
    tolower(title);
    const char char_list[] = ":.,'?;";
    for (char char_list_item : char_list)
        std::erase(title, char_list_item);
    return title;
}

inline auto get_filename(std::filesystem::path& path) noexcept {
    return path.filename();
}

inline auto search_indices(const std::string& data, std::vector<size_t>& indices, const auto& text_to_search, size_t data_pos, size_t data_end) {
    auto text = title_ready(text_to_search);
    const auto special_chrs = { ':', '.', '\'', '?', ';' };

    size_t last_eol{ 0 };
    while (data_pos < data_end) {
        bool current_index = true;
        auto* data_pos_aux = &data[data_pos];
        if (*data_pos_aux == '\n' || *data_pos_aux == '\r')
            last_eol = data_pos + 1;

        auto* text_pos = &text[0];
        while (*text_pos != '\0') {
            if (std::count(special_chrs.begin(), special_chrs.end(), *data_pos_aux)) { // ignore certain chrs
                ++data_pos_aux;
                continue;
            }
            if ((*data_pos_aux != *text_pos // compare chrs
                && *data_pos_aux + ('a' - 'A') != *text_pos) // compare lower case chrs
                || !(*data_pos_aux)) { // last char?
                current_index = false;
                break;
            }
            ++text_pos;
            ++data_pos_aux;
        }

        if (current_index) {
            indices.emplace_back(last_eol);
            // std::cout << std::this_thread::get_id() << " - " << last_eol << "\n";
        }
        ++data_pos;
    }
    return indices;
}

#ifdef ONE_THREAD_IN_SEARCH_INDICES
inline auto search_indices(const std::string& data, const std::string& text_to_search) {
    PrintDuration dur;
    std::vector<size_t> indices;
    return search_indices(data, indices, text_to_search, 0, data.size());
}
#else
inline auto search_indices(const std::string& data, const std::string& text_to_search) {
    PrintDuration dur;
    std::mutex indices_mutex;
    
    std::vector<size_t> indices;

    auto num_chunks = std::thread::hardware_concurrency();
    size_t data_size = data.size();
    if (!num_chunks || data_size < 1024)
        return search_indices(data, indices, text_to_search, 0, data_size);
    size_t data_chunk_size = data_size / num_chunks;
    size_t begin_of_line{ 0 };

#ifdef USING_ASYNC
    {
        std::vector<std::future<void>> workers;
        for (; begin_of_line < data_size;) {
            size_t end_of_line = std::min(data_size, data.find("\n", begin_of_line + data_chunk_size));
            workers.emplace_back(std::async(std::launch::async, [&indices, &data, &indices_mutex](auto text_to_search, auto begin_of_line, auto end_of_line) {
                std::vector<size_t> indicesAux;
                search_indices(data, indicesAux, text_to_search, begin_of_line, end_of_line);
                std::lock_guard<std::mutex> guard(indices_mutex);
                indices.insert(indices.end(), indicesAux.begin(), indicesAux.end());
                }, text_to_search, begin_of_line, end_of_line));
            begin_of_line = end_of_line;
        }
    }
#else
    {
        std::vector<std::jthread> workers;
        for (; begin_of_line < data_size;) {
            size_t end_of_line = std::min(data_size, data.find("\n", begin_of_line + data_chunk_size));
            workers.emplace_back(std::jthread([&indices, &data, &indices_mutex](auto text_to_search, auto begin_of_line, auto end_of_line) {
                std::vector<size_t> indicesAux;
                search_indices(data, indicesAux, text_to_search, begin_of_line, end_of_line);
                std::lock_guard<std::mutex> guard(indices_mutex);
                indices.insert(indices.end(), indicesAux.begin(), indicesAux.end());
                }, text_to_search, begin_of_line, end_of_line));
            begin_of_line = end_of_line;
        }
        // jthread calls join in destructor
    }
#endif

    return indices;
}
#endif

inline std::string get_text_in_quotes(auto current, const auto last, const char delimiter) noexcept {
    std::string str{ "" };
    bool bEscaping{ false };
    while (current != last) {
        if (!bEscaping && *current == '\\')
            bEscaping = true;
        else if (!bEscaping && *current == delimiter)
            break;
        else bEscaping = false;
        str += *current;
        ++current;
    }
    return str;
}
