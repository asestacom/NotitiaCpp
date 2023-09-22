#include "pch.h"

#include "modules/just_watch/JustWatchModule.h"
#include "MovieRecord.h"

inline const std::string JUSTWATCH_EDGES = "resources/files/justwatch/test.json";
inline const std::string JUSTWATCH_EDGES_2 = "resources/files/justwatch/test2.json";

TEST(JustWatchModule, get_justwatchs_title_found) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	std::string title = "Nightmare Alley";
	MovieRecord movie;

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 1);
	EXPECT_EQ("tm855362", justWatchs[0].id);
}

TEST(JustWatchModule, get_justwatchs_title_not_found) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	std::string title = "not found";
	MovieRecord movie;

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 0);
}

TEST(JustWatchModule, get_justwatchs_blank) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	std::string title = "";
	MovieRecord movie;

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 0);
}

TEST(JustWatchModule, get_justwatchs_imdb_id_found) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	std::string title = "not found";
	MovieRecord movie;
	movie.imdb.emplace_back(Imdb{.tconst = "tt7740496"});

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 1);
	EXPECT_EQ("tm855362", justWatchs[0].id);
}

TEST(JustWatchModule, get_justwatchs_several_imdb_ids_found) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	std::string title = "not found";
	MovieRecord movie;
	movie.imdb.emplace_back(Imdb{ .tconst = "tt1950186" });
	movie.imdb.emplace_back(Imdb{ .tconst = "tt7740496" });

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 2);
	EXPECT_EQ("tm855362", justWatchs[0].id);
	EXPECT_EQ("tm420873", justWatchs[1].id);
}

TEST(JustWatchModule, get_justwatchs_imdb_id_not_found) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	std::string title = "not found";
	MovieRecord movie;
	movie.imdb.emplace_back(Imdb{ .tconst = "not found" });

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 0);
}

TEST(JustWatchModule, get_justwatchs_title_found_in_1_using_more_than_one_input_file) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	db_items["justwatch_edges"][1] = JUSTWATCH_EDGES_2;
	std::string title = "Nightmare Alley";
	MovieRecord movie;

	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 1);
	EXPECT_EQ("tm855362", justWatchs[0].id);
}

TEST(JustWatchModule, get_justwatchs_title_found_in_2_using_more_than_one_input_file) {
	JsonVariant db_items;
	db_items["justwatch_edges"][0] = JUSTWATCH_EDGES;
	db_items["justwatch_edges"][1] = JUSTWATCH_EDGES_2;
	std::string title = "Derry Girls";
	MovieRecord movie;	
	
	JustWatchModule justWatchModule(db_items, true);
	auto justWatchs = justWatchModule.get_justwatchs(movie.imdb, title);

	EXPECT_EQ(justWatchs.size(), 1);
	EXPECT_EQ("ts77795", justWatchs[0].id);
}
