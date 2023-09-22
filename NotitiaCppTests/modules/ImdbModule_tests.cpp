#include "pch.h"

#include "modules/imdb/ImdbModule.h"
#include "modules/imdb/ImdbAkasModule.h"
#include "modules/imdb/ImdbBasicsModule.h"
#include "modules/imdb/ImdbRatingsModule.h"
#include "helper/config_helper.h"
#include "MovieRecord.h"

inline const std::string DATA = "resources/files/imdb/title.basics.reduced.es_test.tsv";
inline const std::string DATA_AKAS = "resources/files/imdb/title.akas.reduced.es_test.tsv";
inline const std::string DATA_RATINGS = "resources/files/imdb/title.ratings_test.tsv";

TEST(ImdbModule, search_basics_found) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "Blade Runner";
	std::string title_optional = "not found";
	std::string year = "1982";

	ImdbBasicsModule imdbModule(db_items, true);
	auto imdbs = imdbModule.get_basics(title, title_optional, year);

	EXPECT_EQ(imdbs.size(), 1);
	EXPECT_EQ("tt0083658", imdbs[0].tconst);
}

TEST(ImdbModule, search_basics_optional_found) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "not found";
	std::string title_optional = "Blade Runner";
	std::string year = "1982";

	ImdbBasicsModule imdbModule(db_items, true);
	auto imdbs = imdbModule.get_basics(title, title_optional, year);

	EXPECT_EQ(imdbs.size(), 1);
	EXPECT_EQ("tt0083658", imdbs[0].tconst);
}

TEST(ImdbModule, search_basics_not_found_wrong_year) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "Blade Runner";
	std::string title_optional = "not found";
	std::string year = "1882";

	ImdbBasicsModule imdbModule(db_items, true);
	auto imdbs = imdbModule.get_basics(title, title_optional, year);

	EXPECT_EQ(imdbs.size(), 0);
}

TEST(ImdbModule, search_basics_not_found) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "not found";
	std::string title_optional = "not found";
	std::string year = "1882";

	ImdbBasicsModule imdbModule(db_items, true);
	auto imdbs = imdbModule.get_basics(title, title_optional, year);

	EXPECT_EQ(imdbs.size(), 0);
}

TEST(ImdbModule, get_akas_found) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "Blade Runner";
	std::string title_optional = "not found";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	auto imdbs = imdbAkasModule.get_akas(title, title_optional);

	EXPECT_EQ(imdbs.size(), 1);
	EXPECT_EQ("tt0083658", imdbs[0].tconst);
}

TEST(ImdbModule, get_akas_found_en_default_language) {
	ConfigHelper config;
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "Blade Runner";
	std::string title_optional = "not found";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	auto imdbs = imdbAkasModule.get_akas(title, title_optional);

	EXPECT_EQ(imdbs.size(), 1);
	EXPECT_EQ("tt0083658", imdbs[0].tconst);
}

TEST(ImdbModule, get_akas_optional_found) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "not found";
	std::string title_optional = "Blade Runner";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	auto imdbs = imdbAkasModule.get_akas(title, title_optional);

	EXPECT_EQ(imdbs.size(), 1);
	EXPECT_EQ("tt0083658", imdbs[0].tconst);
}

TEST(ImdbModule, get_akas_not_found) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "not found";
	std::string title_optional = "not found";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	auto imdbs = imdbAkasModule.get_akas(title, title_optional);

	EXPECT_EQ(imdbs.size(), 0);
}

TEST(ImdbModule, get_akas_blank) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "";
	std::string title_optional = "";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	auto imdbs = imdbAkasModule.get_akas(title, title_optional);

	EXPECT_EQ(imdbs.size(), 0);
}

TEST(ImdbModule, update_imdbs_with_akas_found_and_added) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::vector<Imdb> imdbs;
	std::vector<Imdb> imdbs_akas;
	imdbs_akas.emplace_back(Imdb{ .tconst = "tt0083658" });
	std::string year = "1982";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	imdbAkasModule.update_imdbs_with_akas(imdbs, imdbs_akas, year);

	EXPECT_EQ(imdbs.size(), 1);
	EXPECT_EQ("tt0083658", imdbs[0].tconst);
}

TEST(ImdbModule, update_imdbs_with_akas_not_found) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::vector<Imdb> imdbs;
	std::vector<Imdb> imdbs_akas;
	imdbs_akas.emplace_back(Imdb{ .tconst = "tt18561016" });
	std::string year = "1922";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	imdbAkasModule.update_imdbs_with_akas(imdbs, imdbs_akas, year);

	EXPECT_EQ(imdbs.size(), 0);
}

TEST(ImdbModule, update_imdbs_with_akas_empty) {
	ConfigHelper config;
	config.language = "ES";
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::vector<Imdb> imdbs;
	std::vector<Imdb> imdbs_akas;
	std::string year = "1982";

	ImdbAkasModule imdbAkasModule(config, db_items, true);
	imdbAkasModule.update_imdbs_with_akas(imdbs, imdbs_akas, year);

	EXPECT_EQ(imdbs.size(), 0);
}

TEST(ImdbModule, get_ratings_found) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "tt1856101";

	ImdbRatingsModule imdbRatingsModule(db_items, true);
	auto rating = imdbRatingsModule.get_ratings(title);

	EXPECT_EQ("tt1856101", rating.tconst);
	EXPECT_EQ("8.0", rating.averageRating);
	EXPECT_EQ("578708", rating.numVotes);
}

TEST(ImdbModule, get_ratings_found_similar_to_other_tconst) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "tt18561016";

	ImdbRatingsModule imdbRatingsModule(db_items, true);
	auto rating = imdbRatingsModule.get_ratings(title);

	EXPECT_EQ("tt18561016", rating.tconst);
	EXPECT_EQ("5.7", rating.averageRating);
	EXPECT_EQ("19", rating.numVotes);
}

TEST(ImdbModule, get_ratings_not_found) {
	JsonVariant db_items;
	db_items["data"] = DATA;
	db_items["data_akas"] = DATA_AKAS;
	db_items["data_ratings"] = DATA_RATINGS;
	std::string title = "not found";

	ImdbRatingsModule imdbRatingsModule(db_items, true);
	auto rating = imdbRatingsModule.get_ratings(title);

	EXPECT_EQ("", rating.tconst);
	EXPECT_EQ("", rating.averageRating);
	EXPECT_EQ("", rating.numVotes);
}

