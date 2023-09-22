#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "common/Utils.h"

struct ImdbRatingsModule : public ImdbModule {
public:
    ImdbRatingsModule(auto& db_items, boolean ignoreLoaded = false) : ImdbModule(db_items, ignoreLoaded) {
    }

    auto get_ratings(const auto &imdb_id) {
        auto imdbRatings = ImdbRatings();
        search_ratings(imdbRatings, imdb_id);
        return imdbRatings;
    }

private:
    struct ImdbRatings {
        std::string tconst;
        std::string averageRating;
        std::string numVotes;

        [[nodiscard]] static ImdbRatings get_rating(const std::string& line) {
            // tconst	averageRating	numVotes
            enum {
                TCONST, AVERAGERATING, NUMVOTES
            };
            auto cols = split(line, line.contains('\t') ? '\t' : ',');
            return ImdbRatings{
                .tconst = cols[TCONST],
                .averageRating = cols[AVERAGERATING],
                .numVotes = cols[NUMVOTES]
            };
        }
    };

    void search_ratings(ImdbRatings& imdbRatings, const auto& imdb_id) {
        std::cout << "search_ratings...\n";
        auto imdb_id_tabs = format("{}\t", title_ready(imdb_id));

        for (auto i : search_indices(data_imdbs_ratings, imdb_id_tabs)) {
            std::string line = get_line_by_index(i, data_imdbs_ratings);
            auto rating = ImdbRatings::get_rating(line);
            if (rating.tconst == imdb_id) {
                imdbRatings = ImdbRatings{
                    .tconst = rating.tconst,
                    .averageRating = rating.averageRating,
                    .numVotes = rating.numVotes
                };
                break; // just one
            }
        }
    }
};
