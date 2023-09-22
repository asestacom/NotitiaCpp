#include <iostream>
#include <string>

#include "config/Config.h"
#include "common/JsonFile.h"
#include "MovieRecord.h"
#include "NodeInfo.h"
#include "modules/imdb/ImdbBasicsModule.h"
#include "modules/imdb/ImdbAkasModule.h"
#include "modules/imdb/ImdbRatingsModule.h"
#include "modules/tmdb/TmdbModule.h"
#include "modules/just_watch/JustWatchModule.h"
#include <execution>

void update_movie(ConfigData& config, MovieRecord &movie, const auto &node_name, const auto &full_node_name) {
    auto [title, year, extra, format, title_optional] = NodeInfo::parse_node_name(node_name);

    std::cout << "----------------------------------------------------------------\n";
    std::cout << std::format("# Title: '{}' ~ '{}' - Year: ({}) '{}' '. {}'\n", title, title_optional, year, extra, format);

    if (!movie.imdb_confirmed && year != "") {
        auto imdbs = ImdbBasicsModule(config.config["db"]).get_basics(title, title_optional, year);

        if (!imdbs.size()) {
            ImdbAkasModule imdbAkasModule(config, config.config["db"]);
            auto imdbs_akas = imdbAkasModule.get_akas(title, title_optional);
            imdbAkasModule.update_imdbs_with_akas(imdbs, imdbs_akas, year);
        }

        for (auto &imdb : imdbs) {
            imdb.ratings = ImdbRatingsModule(config.config["db"]).get_ratings(imdb.tconst).averageRating;
            movie.imdb.emplace_back(imdb);
        }

        movie.imdb_confirmed = false;
    }

    if (!movie.justwatch_confirmed) {
        JustWatchModule justWatchModule(config.config["db"]);
        auto justwatchs = justWatchModule.get_justwatchs(movie.imdb, title);
        for (auto &justwatch : justwatchs)
            movie.justwatch.emplace_back(justwatch);
        movie.justwatch_confirmed = false;
    }

    if (!movie.tmdb_confirmed) {
        TmdbModule tmdbModule(config.config["db"]);
        auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);
        for (auto &tmdb : tmdbs)
            movie.Tmdb.emplace_back(tmdb);
        movie.tmdb_confirmed = false;
    }

    movie.title = title;
    movie.year = year;

    auto file_iter = std::find_if(movie.node.begin(), movie.node.end(), [&node_name](auto &node) {
        return node.node_name == node_name;
        });
    if (file_iter == movie.node.end()) {
        NodeRecord fileRecord = NodeRecord{
            .node_name = node_name,
            .full_node_name = full_node_name.string(), // TODO: convert to / -> //
            .extra = extra,
            .format = format
        };
        movie.node.emplace_back(fileRecord);
    };
}

void save_movies(const std::string &path, MovieRecord::MovieRecords &movies) {
    std::ofstream outfile(path);
    JsonFile::save_json(outfile, MovieRecord::to_variant(movies));
}

void proces_movies(ConfigData& config, auto& movies) {
    const std::vector<std::string> filter_node_format(config.filter_node_format);

    std::string lines_raw = JsonFile::read_file(config.raw_list_movie_file);
    auto lines = split(lines_raw, '\n');

    for (auto line : lines) {
        std::filesystem::path full_node_name = ltrim(rtrim(line));
        std::string node_name = full_node_name.filename().string();

        auto [title, year, extra, format, title_optional] = NodeInfo::parse_node_name(node_name);

        if (title.empty())
            continue;
        if (std::find(filter_node_format.begin(), filter_node_format.end(), format) == filter_node_format.end())
            continue;

        auto movie_iter = std::find_if(movies.begin(), movies.end(), [&title](auto& movie) {
            return movie.title == std::string(title);
            });
        if (movie_iter == movies.end()) {
            MovieRecord movie;
            update_movie(config, movie, node_name, full_node_name);
            if (!movie.node.size())
                movie.node.emplace_back(
                    NodeRecord{
                        .node_name = node_name,
                        .full_node_name = full_node_name.string() // TODO: convert to / -> //
                    }
            );
            movies.emplace_back(movie); // TODO: find first if exists
        }
        else {
            update_movie(config, *movie_iter, node_name, full_node_name);
        }
    }
}

int main() {
    PrintDuration dur;
    const std::string config_file_name = "config.json";

    std::cout << "reading files...\n";
    Config config(config_file_name);
    if (config.loaded) {
        JsonVariant movies_json;
        if (config.updating) {
            if (std::filesystem::exists(config.working_file))
                movies_json = JsonFile::read_json(config.working_file);
            else
                std::cout << "Working file '" + config.working_file + "' doesn't exist.\n";
        }

        try {
            std::cout << "checking files...\n";
            MovieRecord::MovieRecords movies = movies_json.to_vector();
            proces_movies(config, movies);

            std::cout << "saving data...\n";
            save_movies(config.output_file.size() ? config.output_file : config.working_file, movies);
        } 
        catch (std::filesystem::filesystem_error ex) {
            std::cout << "Some data files (config::db) don't exist.\n";
            std::cout << "Nothig was stored into the output file.\n";
        }
        catch (std::exception ex) {
            std::cout << ex.what() << "\n";
        }
    }
    else {
        std::cout << "Config file '" + config_file_name + "' doesn't exist.\n";
    }
}
