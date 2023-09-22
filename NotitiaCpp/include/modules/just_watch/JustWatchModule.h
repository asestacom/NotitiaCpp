#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "JustWatch.h"
#include "common/Utils.h"
#include "common/JsonFile.h"

class JustWatchModule {
public:
    JustWatchModule(auto& db_items, boolean ignoreLoaded = false) {
        if (loaded && !ignoreLoaded)
            return;
        loaded.store(true);

        load_data(db_items);
    }

    auto get_justwatchs(auto &imdbs, const auto &title) {
        // TODO: to check this maybe we should use English version of the title, so if akas...
        std::cout << "justwatch_edges...\n";
        std::vector<Justwatch> justwatchs;
        for (const auto &edge : justwatch_edges) {
            auto edge_found = edge.node.content.external_ids.imdb_id.compare("") ?
                std::find_if(imdbs.begin(), imdbs.end(), [&edge](const auto &item) { return item.tconst == edge.node.content.external_ids.imdb_id; })
                : imdbs.end();
            if (edge_found != imdbs.end() || edge.node.content.title == title)
            {
                std::cout << edge.node.id << "\n";
                justwatchs.emplace_back(Justwatch{
                    .id = edge.node.id,
                    .edge = edge,
                    });
            }
        }
        return justwatchs;
    }

private:
    void load_data(auto& db_items) noexcept {
        justwatch_edges = JsonFile::read_json(db_items["justwatch_edges"].to_string_vector()).to_vector();
    }

    static inline std::atomic_bool loaded{false};
    static inline Edge::Edges justwatch_edges;
};
