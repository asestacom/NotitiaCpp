#pragma once

#include <string>

struct NodeInfo {
    std::string title;
    std::string year;
    std::string extra;
    std::string format;
    std::string title_optional;

    static NodeInfo parse_node_name(const std::string& node_name) noexcept {
        enum {
            TITLE = 1, YEAR, EXTRA, FORMAT
        };
        const std::regex match_regex(R"((.*)\((.*)\)(.*)?[.](.*)?)");
        // TODO: if there is ". " -> "  " and it won't find it
        std::smatch match_result;
        std::regex_match(node_name, match_result, match_regex);

        auto title = match_result[TITLE].str();
        std::string title_original;
        auto pos = title.find('~');
        if (pos) {
            title_original = title.substr(pos + 1);
            title = title.substr(0, pos);
        }
        title = rtrim(ltrim(title));

        auto year = match_result[YEAR].str();
        auto extra = match_result[EXTRA].str();
        auto format = match_result[FORMAT].str();
        return { .title = title, .year = year, .extra = extra, .format = format, .title_optional = rtrim(ltrim(title_original)) };
    }
};
