#include "pch.h"

#include <variant>
#include <vector>
#include <algorithm>
#include "common/csv.h"
#include "common/Json.h"
#include "common/JsonFile.h"

std::string clean_string(const auto& str_input) {
    char chars_to_remove[] = "\r\n\t ";
    std::string str(str_input);
    for (size_t i = 0; i < strlen(chars_to_remove); ++i) {
        str.erase(std::remove(str.begin(), str.end(), chars_to_remove[i]), str.end());
    }
    return str;
}
bool equals(const std::string_view& str1, const std::string_view& str2) {
    return !clean_string(str1).compare(clean_string(str2));
}

TEST(CSV, CSVLineTest) {

    const std::string_view str = "abc,def";
    auto cols = csv::read_csv_line_to_map(str.cbegin(), str.cend());

    EXPECT_EQ(cols.size(), 2);
}

TEST(Json, JsonTest) {

    JsonVariant json;
    JsonVariant leaf1("Hello");
    JsonVariant leaf2("world");
    JsonVariantVector children { leaf1, leaf2 };
    JsonVariantMap map;
    map["map1"] = 1;
    map["map2"] = "text";
    json["root"] = JsonVariant(JsonVariantVector{ JsonVariant(children), JsonVariant(map) });

    std::stringstream json_stream;
    JsonFile::write_level(json_stream, json);

    EXPECT_TRUE(equals(json_stream.view(), R"({
        "root": [
            [
                "Hello",
                "world"
            ],
            {
            "map1":"1",
            "map2" : "text"
            }
        ]
    })"));
}

TEST(Json, LoadJsonTest) {
    JsonVariant config = JsonFile::read_json("resources/files/utils/config_example.json");
    auto version = std::stoi(config["version"].to_string());
    auto language = config["language"].to_string();
    auto items = config["items"].to_string_vector();
    auto db_items = config["db"]["test_array"].to_string_vector();
    auto db_test = config["db"]["test"].to_string();

    std::stringstream json_stream;
    JsonFile::save_json(json_stream, config);

    EXPECT_EQ(1, version);
    EXPECT_EQ("ES", language);
    EXPECT_EQ("Hello world", db_test);
    // order is important to check the full output
    EXPECT_TRUE(equals(json_stream.view(), R"({
            "db":	{
	            "test":"Hello world",
	            "test_array":		[
                        "item 1",
                        "item 2"
		            ]
	            },
            "items":	[
                    "movie",
                    "tvSeries",
                    "short"
	            ],
            "language":"ES",
            "version":"1"
        })"));
}
