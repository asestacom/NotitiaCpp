#pragma once

#include <vector>
#include "Utils.h"

namespace csv {
    inline std::vector<std::string> read_csv_line_to_map(auto current, const auto last) {
        // str.find('\t') != std::string::npos ? '\t' : ','; // TODO: 
        std::vector<std::string> cols;
        auto found{ false };
        while (current != last) {
            switch (*current)
            {
            case ',':
                if (!found)
                    cols.emplace_back("");
                found = false;
                [[fallthrough]];
            case ' ': [[fallthrough]];
            case '\t':
                ++current;
                break;
            case '"':
                {
                    std::string token = get_text_in_quotes(++current, last, *current);
                    current += static_cast<const __int64>(token.size()) + 1;
                    cols.emplace_back(token);
                    found = true;
                }
                break;
            default:
                {
                    std::string token = get_text_in_quotes(current, last, ',');
                    if (token.size()) {
                        cols.emplace_back(token);
                        found = true;
                        current += static_cast<const __int64>(token.size());
                    }
                }
            }
        }
        return cols;
    }
}
