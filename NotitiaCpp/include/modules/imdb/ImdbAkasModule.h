#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Imdb.h"
#include "ImdbModule.h"
#include "ImdbBasicsModule.h"
#include "common/Utils.h"

class ImdbAkasModule : public ImdbModule{
public:
    ImdbAkasModule(ConfigData& config, auto& db_items, boolean ignoreLoaded = false) : ImdbModule(db_items, ignoreLoaded), config(config) {
    }

    auto get_akas(const auto &title, const auto &title_optional) {
        auto imdbs = std::vector<Imdb>();
        search_akas(imdbs, title, true);
        if (imdbs.size() == 0 && title_optional.size() > 0)
            search_akas(imdbs, title_optional, true);
        return imdbs;
    }

    void update_imdbs_with_akas(auto &imdbs, auto &imdbs_akas, auto &year) {
        if (imdbs_akas.size() == 0)
            return;

        for (auto aka : imdbs_akas) {
            auto basic_imdbs = std::vector<Imdb>();
            ImdbBasicsModule(config.config["db"]).search_basics(basic_imdbs, aka.tconst, year, false);
            if (basic_imdbs.size() > 0) {
                for (const auto &basic : basic_imdbs) {
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

private:
    ConfigData& config;

    struct TitleAkasRecord {
        std::string titleId;
        std::string ordering;
        std::string title;
        std::string region;
        std::string language;
        std::string types;
        std::string attributes;
        std::string isOriginalTitle;

        [[nodiscard]] static TitleAkasRecord get_aka(const std::string& line) {
            // titleId	ordering	title	region	language	types	attributes	isOriginalTitle
            enum {
                TITLEID, ORDERING, TITLE, REGION, LANGUAGE, TYPES, ATTRIBUTES, ISORIGINALTITLE
            };

            auto cols = split(line, line.contains('\t') ? '\t' : ',');
            return TitleAkasRecord{
                .titleId = cols[TITLEID],
                .ordering = cols[ORDERING],
                .title = cols[TITLE],
                .region = cols[REGION],
                .language = cols[LANGUAGE],
                .types = cols[TYPES],
                .attributes = cols[ATTRIBUTES],
                .isOriginalTitle = cols[ISORIGINALTITLE]
            };
        }
    };

    void search_akas(std::vector<Imdb>& imdbs, const auto& current_title, bool tab_at_the_beginning) {
        std::cout << "search_akas...\n";
        auto title = title_ready(current_title);
        auto title_tabs = tab_at_the_beginning ? std::format("\t{}\t", title_ready(current_title)) : format("{}\t", title_ready(current_title));

        for (auto i : search_indices(data_imdbs_akas, title_tabs)) {
            std::string line = get_line_by_index(i, data_imdbs_akas);
            auto aka = TitleAkasRecord::get_aka(line);
            if (title_ready(aka.title) == title) {
                if (aka.region == config.language) {
                    if (std::find_if(imdbs.begin(), imdbs.end(), [&aka](const auto& item) {
                        return item.tconst == aka.titleId;
                        }) == imdbs.end())
                    {
                        auto imdb = Imdb{
                            .tconst = aka.titleId
                        };
                        imdbs.emplace_back(imdb);
                    }
                }
            }
        }
    }
};
