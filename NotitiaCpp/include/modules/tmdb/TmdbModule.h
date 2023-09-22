#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "modules/tmdb/Tmdb.h"
#include "modules/imdb/Imdb.h"

#include "common/Utils.h"
#include "common/csv.h"
#include "common/JsonFile.h"

class TmdbModule {
public:
    TmdbModule(auto& db_items, boolean ignoreLoaded = false) {
        if (loaded && !ignoreLoaded)
            return;
        loaded.store(true);

        load_data(db_items);
    }

    auto get_tmdbs(std::vector<Imdb> &imdbs, const auto &title, const auto &title_optional, const auto &year) {
        auto tmdbs = search_tmdbs_by_imdb(imdbs, title, title_optional, year);

        std::cout << "search_tmdbs_big...\n";
        std::vector<Tmdb> tmdbs_big;
        if (tmdbs.size() > 0) {
            for (const auto &Tmdb : tmdbs) {
                search_tmdbs_big(tmdbs_big, Tmdb.id, year, false);
            }
            for (auto &tmdb_big : tmdbs_big) {
                for (auto &Tmdb : tmdbs) {
                    if (Tmdb.id == tmdb_big.id) {
                        Tmdb.poster_path = tmdb_big.poster_path;
                        tmdb_big.id = "0"; // TODO: ?
                        break;
                    }
                }
            }
        }
        else {
            search_tmdbs_big(tmdbs_big, title, year, true);
            if (title_optional.size() > 0) {
                search_tmdbs_big(tmdbs_big, title_optional, year, true);
            }
        }

        for (auto &tmdb : tmdbs_big) {
            if (tmdb.id != "0") { // TODO: ?
                tmdbs.emplace_back(std::move(tmdb));
            }
        }

        return tmdbs;
    }

private:
    struct TitleTmdbsBigRecord {
        // adult,belongs_to_collection,budget,genres,homepage,id,imdb_id,original_language,original_title,overview,popularity,poster_path,production_companies,production_countries,release_date,revenue,runtime,spoken_languages,status,tagline,title,video,vote_average,vote_count
        // False, "{'id': 468552, 'name': 'Wonder Woman Collection', 'poster_path': '/yEczguvMSFJRuUiOFgWXsZG3CsG.jpg', 'backdrop_path': '/n9KlvCOBFDmSyw3BgNrkUkxMFva.jpg'}", 149000000, "[{'id': 28, 'name': 'Action'}, {'id': 12, 'name': 'Adventure'}, {'id': 14, 'name': 'Fantasy'}]", http://www.warnerbros.com/wonder-woman,297762,tt0451279,en,Wonder Woman,An Amazon princess comes to the world of Man to become the greatest of the female superheroes.,294.337037,/imekS7f1OuHyUP2LAiTEM0zBzUz.jpg,"[{'name': 'Dune Entertainment', 'id': 444}, {'name': 'Atlas Entertainment', 'id': 507}, {'name': 'Warner Bros.', 'id': 6194}, {'name': 'DC Entertainment', 'id': 9993}, {'name': 'Cruel & Unusual Films', 'id': 9995}, {'name': 'TENCENT PICTURES', 'id': 81620}, {'name': 'Wanda Pictures', 'id': 83838}]","[{'iso_3166_1': 'US', 'name': 'United States of America'}]",2017-05-30,820580447,141.0,"[{'iso_639_1': 'de', 'name': 'Deutsch'}, {'iso_639_1': 'en', 'name': 'English'}]",Released,Power. Grace. Wisdom. Wonder.,Wonder Woman,False,7.2,5025
        std::string adult;
        std::string belongs_to_collection;
        std::string budget;
        std::string genres;
        std::string homepage;
        std::string id;
        std::string imdb_id;
        std::string original_language;
        std::string original_title;
        std::string overview;
        std::string popularity;
        std::string poster_path;
        std::string production_companies;
        std::string production_countries;
        std::string release_date;
        std::string revenue;
        std::string runtime;
        std::string spoken_languages;
        std::string status;
        std::string tagline;
        std::string title;
        std::string video;
        std::string vote_average;
        std::string vote_count;

        [[nodiscard]] static TitleTmdbsBigRecord get_tmdb(const std::string& str) {
            enum {
                ADULT, BELONGS_TO_COLLECTION, BUDGET, GENRES, HOMEPAGE, ID, IMDB_ID, ORIGINAL_LANGUAGE, ORIGINAL_TITLE, OVERVIEW, POPULARITY, POSTER_PATH, PRODUCTION_COMPANIES, PRODUCTION_COUNTRIES, RELEASE_DATE, REVENUE, RUNTIME, SPOKEN_LANGUAGES, STATUS, TAGLINE, TITLE, VIDEO, VOTE_AVERAGE, VOTE_COUNT
            };

            auto cols = csv::read_csv_line_to_map(str.begin(), str.end());
            if (cols.size() >= 24)
                return TitleTmdbsBigRecord{
                    .adult = cols[ADULT],
                    .belongs_to_collection = cols[BELONGS_TO_COLLECTION],
                    .budget = cols[BUDGET],
                    .genres = cols[GENRES],
                    .homepage = cols[HOMEPAGE],
                    .id = cols[ID],
                    .imdb_id = cols[IMDB_ID],
                    .original_language = cols[ORIGINAL_LANGUAGE],
                    .original_title = cols[ORIGINAL_TITLE],
                    .overview = cols[OVERVIEW],
                    .popularity = cols[POPULARITY],
                    .poster_path = cols[POSTER_PATH],
                    .production_companies = cols[PRODUCTION_COMPANIES],
                    .production_countries = cols[PRODUCTION_COUNTRIES],
                    .release_date = cols[RELEASE_DATE],
                    .revenue = cols[REVENUE],
                    .runtime = cols[RUNTIME],
                    .spoken_languages = cols[SPOKEN_LANGUAGES],
                    .status = cols[STATUS],
                    .tagline = cols[TAGLINE],
                    .title = cols[TITLE],
                    .video = cols[VIDEO],
                    .vote_average = cols[VOTE_AVERAGE],
                    .vote_count = cols[VOTE_COUNT]
            };
            return TitleTmdbsBigRecord();
        }

        [[nodiscard]] static TitleTmdbsBigRecord get_tmdb_big(const std::string& str) {
            enum {
                ID, TITLE, GENRES, ORIGINAL_LANGUAGE, OVERVIEW, POPULARITY, PRODUCTION_COMPANIES, RELEASE_DATE, BUDGET, REVENUE, RUNTIME, STATUS, TAGLINE, VOTE_AVERAGE, VOTE_COUNT, CREDITS, KEYWORDS, POSTER_PATH, BACKDROP_PATH, RECOMMENDATIONS
            };

            auto cols = csv::read_csv_line_to_map(str.begin(), str.end());
            if (cols.size() >= 20)
                return TitleTmdbsBigRecord{
                    .budget = cols[BUDGET],
                    .genres = cols[GENRES],
                    .id = cols[ID],
                    .original_language = cols[ORIGINAL_LANGUAGE],
                    .overview = cols[OVERVIEW],
                    .popularity = cols[POPULARITY],
                    .poster_path = cols[POSTER_PATH],
                    .production_companies = cols[PRODUCTION_COMPANIES],
                    .release_date = cols[RELEASE_DATE],
                    .revenue = cols[REVENUE],
                    .runtime = cols[RUNTIME],
                    .status = cols[STATUS],
                    .tagline = cols[TAGLINE],
                    .title = cols[TITLE],
                    .vote_average = cols[VOTE_AVERAGE],
                    .vote_count = cols[VOTE_COUNT]
            };
            return TitleTmdbsBigRecord();
        }
    };

    void search_tmdbs(std::vector<Tmdb>& tmdbs, const auto& current_title, const auto& current_year, bool tab_at_the_beginning) {
        auto title = title_ready(current_title);
        if (title.size() == 0)
            return;
        auto title_tabs = tab_at_the_beginning ? std::format(",{},", title_ready(current_title)) : format("{},", title_ready(current_title));

        for (auto i : search_indices(data_tmdbs, title_tabs)) {
            std::string line = get_line_by_index(i, data_tmdbs);
            auto titleTmdbsBigRecord = TitleTmdbsBigRecord::get_tmdb(line);
            if (title_ready(titleTmdbsBigRecord.title) == title) {
                if (titleTmdbsBigRecord.release_date.substr(0, 4) == current_year) {
                    if (std::find_if(tmdbs.begin(), tmdbs.end(), [&titleTmdbsBigRecord](const auto& item) {
                        return item.id == titleTmdbsBigRecord.id;
                        }) == tmdbs.end())
                    {
                        auto tmdb = Tmdb{
                            .id = titleTmdbsBigRecord.id,
                            .imdb_id = titleTmdbsBigRecord.imdb_id,
                            .overview = titleTmdbsBigRecord.overview,
                            .poster_path = titleTmdbsBigRecord.poster_path,
                            .vote_average = titleTmdbsBigRecord.vote_average
                        };
                        tmdbs.emplace_back(tmdb);
                    }
                }
            }
        }
    }

    void search_tmdbs_big(std::vector<Tmdb>& tmdbs, const auto& current_title, const auto& current_year, bool tab_at_the_beginning) {
        auto title = title_ready(current_title);
        if (title.size() == 0)
            return;
        auto title_tabs = tab_at_the_beginning ? std::format(",{},", title_ready(current_title)) : format("{},", title_ready(current_title));

        for (auto i : search_indices(data_tmdbs_big, title_tabs)) {
            std::string line = get_line_by_index(i, data_tmdbs_big);
            auto titleTmdbsBigRecord = TitleTmdbsBigRecord::get_tmdb_big(line);
            if (title_ready(titleTmdbsBigRecord.title) == title)
            {
                if (titleTmdbsBigRecord.release_date.substr(0, 4) == current_year) {
                    if (std::find_if(tmdbs.begin(), tmdbs.end(), [&titleTmdbsBigRecord](const auto& item) {
                        return item.id == titleTmdbsBigRecord.id;
                        }) == tmdbs.end())
                    {
                        auto tmdb = Tmdb{
                            .id = titleTmdbsBigRecord.id,
                            .overview = titleTmdbsBigRecord.overview,
                            .poster_path = titleTmdbsBigRecord.poster_path
                        };
                        tmdbs.emplace_back(tmdb);
                    }
                }
            }
        }
    }

    void search_tmdbs_by_tconst(std::vector<Tmdb>& tmdbs, const auto& imdb_id, bool tab_at_the_beginning) {
        if (imdb_id.size() == 0)
            return;
        auto title_tabs = tab_at_the_beginning ? std::format(",{},", title_ready(imdb_id)) : format("{},", title_ready(imdb_id));

        for (auto i : search_indices(data_tmdbs, title_tabs)) {
            std::string line = get_line_by_index(i, data_tmdbs);
            auto titleTmdbsBigRecord = TitleTmdbsBigRecord::get_tmdb(line);
            if (titleTmdbsBigRecord.imdb_id == imdb_id) {
                if (std::find_if(tmdbs.begin(), tmdbs.end(), [&titleTmdbsBigRecord](const auto& item) {
                    return item.id == titleTmdbsBigRecord.id;
                    }) == tmdbs.end())
                {
                    auto tmdb = Tmdb{
                        .id = titleTmdbsBigRecord.id,
                        .imdb_id = titleTmdbsBigRecord.imdb_id,
                        .overview = titleTmdbsBigRecord.overview,
                        .poster_path = titleTmdbsBigRecord.poster_path,
                        .vote_average = titleTmdbsBigRecord.vote_average
                    };
                    tmdbs.emplace_back(tmdb);
                }
            }
        }
    }

    auto search_tmdbs_by_imdb(std::vector<Imdb>& imdbs, const auto& title, const auto& title_optional, const auto& year) {
        auto tmdbs = std::vector<Tmdb>();
        std::cout << "search_tmdbs...\n";
        search_tmdbs(tmdbs, title, year, true);
        if (imdbs.size() == 0 && title_optional.size() > 0) {
            std::cout << "search_tmdbs...\n";
            search_tmdbs(tmdbs, title_optional, year, true);
        }
        else if (imdbs.size() == 1) {
            for (const auto& imdb : imdbs) {
                std::cout << "search_tmdbs...\n";
                search_tmdbs_by_tconst(tmdbs, imdb.tconst, true);
            }
        }
        return tmdbs;
    }

    void load_data(auto& db_items) noexcept {
        data_tmdbs = JsonFile::read_file(db_items["data_tmdbs"].to_string());
        data_tmdbs_big = JsonFile::read_file(db_items["data_tmdbs_big"].to_string());
    }

    static inline std::atomic_bool loaded{ false };
    static inline std::string data_tmdbs;
    static inline std::string data_tmdbs_big;
};
