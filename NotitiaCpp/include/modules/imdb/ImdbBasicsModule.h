#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Imdb.h"
#include "ImdbModule.h"
#include "common/Utils.h"

class ImdbBasicsModule : public ImdbModule {
public:
    ImdbBasicsModule(auto& db_items, boolean ignoreLoaded = false) : ImdbModule(db_items, ignoreLoaded) {}
    auto operator= (const ImdbBasicsModule& db_items) { 
        ImdbModule::operator=(db_items);
    }

    void search_basics(std::vector<Imdb> &imdbs, const auto &current_title, const auto &current_year, bool tab_at_the_beginning) {
        std::cout << "search_basics...\n";
        auto title = title_ready(current_title);
        auto title_tabs = tab_at_the_beginning ? std::format("\t{}\t", title_ready(current_title)) : format("{}\t", title_ready(current_title));

        for (auto i : search_indices(data_imdbs, title_tabs)) {
            std::string line = get_line_by_index(i, data_imdbs);
            auto basic = TitleBasicsRecord::get_basic(line);
            if (std::find(titleTypes.begin(), titleTypes.end(), basic.titleType) != titleTypes.end())
                if (basic.tconst == title
                    || title_ready(basic.primaryTitle) == title
                    || title_ready(basic.originalTitle) == title) {
                    if (basic.startYear == current_year) {
                        if (std::find_if(imdbs.begin(), imdbs.end(), [&basic](const auto &item) {
                            return item.tconst == basic.tconst;
                            }) == imdbs.end())
                        {
                            auto imdb = Imdb{
                                .tconst = basic.tconst,
                                .titleType = basic.titleType,
                                .primaryTitle = basic.primaryTitle,
                                .originalTitle = basic.originalTitle,
                                .startYear = basic.startYear,
                                .runtimeMinutes = basic.runtimeMinutes,
                                .genres = basic.genres
                            };
                            imdbs.emplace_back(imdb);
                        }
                    }
                }
        }
    }

    auto get_basics(const auto &title, const auto &title_optional, const auto &year) {
        auto imdbs = std::vector<Imdb>();
        search_basics(imdbs, title, year, true);
        if (imdbs.size() == 0 && title_optional.size() > 0) 
            search_basics(imdbs, title_optional, year, true);
        return imdbs;
    }

private:
    struct TitleBasicsRecord {
        std::string tconst;
        std::string titleType;
        std::string primaryTitle;
        std::string originalTitle;
        std::string isAdult;
        std::string startYear;
        std::string endYear;
        std::string runtimeMinutes;
        std::string genres;

        [[nodiscard]] static TitleBasicsRecord get_basic(const std::string& line) {
            // tconst	titleType	primaryTitle	originalTitle	isAdult	startYear	endYear	runtimeMinutes	genres
            enum {
                TCONST, TITLETYPE, PRIMARYTITLE, ORIGINALTITLE, ISADULT, STARTYEAR, ENDYEAR, RUNTIMEMINUTES, GENRES
            };

            auto cols = split(line, line.contains('\t') ? '\t' : ',');
            return TitleBasicsRecord{
                .tconst = cols[TCONST],
                .titleType = cols[TITLETYPE],
                .primaryTitle = cols[PRIMARYTITLE],
                .originalTitle = cols[ORIGINALTITLE],
                .isAdult = cols[ISADULT],
                .startYear = cols[STARTYEAR],
                .endYear = cols[ENDYEAR],
                .runtimeMinutes = cols[RUNTIMEMINUTES],
                .genres = cols[GENRES]
            };
        }
    };

    const std::vector<std::string> titleTypes{ "movie", "short", "tvSeries", "tvMiniSeries", "tvMovie" };
};
