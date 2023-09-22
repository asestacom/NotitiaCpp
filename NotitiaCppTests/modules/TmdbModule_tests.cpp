#include "pch.h"

#include "modules/tmdb/TmdbModule.h"
#include "MovieRecord.h"

inline const std::string DATA_TMDBS = "resources/files/Tmdb/movies_metadata_test.csv";
inline const std::string DATA_TMDBS_BIG = "resources/files/Tmdb/archive_test.csv";

TEST(TmdbModule, search_tmdbs_found) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "Father of the Bride Part II";
	std::string title_optional = "";
	std::string year = "1995";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 1);
	EXPECT_EQ("tt0113041", tmdbs[0].imdb_id);
}

TEST(TmdbModule, search_tmdbs_not_found_wrong_year) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "Father of the Bride Part II";
	std::string title_optional = "";
	std::string year = "1895";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 0);
}

TEST(TmdbModule, search_tmdbs_not_found) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "not found";
	std::string title_optional = "";
	std::string year = "2025";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 0);
}

TEST(TmdbModule, search_tmdbs_big_found) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "Black Panther: Wakanda Forever";
	std::string title_optional = "";
	std::string year = "2022";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 1);
	EXPECT_EQ("505642", tmdbs[0].id);
}

TEST(TmdbModule, search_tmdbs_big_not_found_wrong_year) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "Black Panther: Wakanda Forever";
	std::string title_optional = "";
	std::string year = "1022";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 0);
}

TEST(TmdbModule, search_tmdbs_big_not_found) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "not found";
	std::string title_optional = "";
	std::string year = "2025";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 0);
}

TEST(TmdbModule, search_tmdbs_by_imdb_imdb_list_empty_using_title_optional_found) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "no-found";
	std::string title_optional = "Father of the Bride Part II";
	std::string year = "1995";
	MovieRecord movie;

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 1);
	EXPECT_EQ("tt0113041", tmdbs[0].imdb_id);
}

TEST(TmdbModule, search_tmdbs_by_imdb_one_item_in_imdb_list_found) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "";
	std::string title_optional = "";
	std::string year = "";
	MovieRecord movie;
	movie.imdb.emplace_back(Imdb{ .tconst = "tt0113041" });

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 1);
	EXPECT_EQ("tt0113041", tmdbs[0].imdb_id);
}

TEST(TmdbModule, search_tmdbs_by_imdb_more_than_one_item_in_imdb_list) {
	JsonVariant db_items;
	db_items["data_tmdbs"] = DATA_TMDBS;
	db_items["data_tmdbs_big"] = DATA_TMDBS_BIG;
	std::string title = "";
	std::string title_optional = "";
	std::string year = "";
	MovieRecord movie;
	movie.imdb.emplace_back(Imdb{ .tconst = "tt0113041" });
	movie.imdb.emplace_back(Imdb{ .tconst = "tt0113228" });

	TmdbModule tmdbModule(db_items, true);
	auto tmdbs = tmdbModule.get_tmdbs(movie.imdb, title, title_optional, year);

	EXPECT_EQ(tmdbs.size(), 0); // it shouldn't find any as there are more than one imdb id
}
