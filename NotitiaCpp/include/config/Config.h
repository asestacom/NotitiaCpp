#pragma once

#include <string>
#include <vector>
#include "common/JsonFile.h"
#include "config/ConfigData.h"

class Config : public ConfigData {
public:
    Config(auto &&path) {
        if (std::filesystem::exists(path)) {
            config = JsonFile::read_json(path);
            version = std::stoi(config["version"].to_string());
            switch (version) {
            case 1:
                language = config["language"].to_string();
                updating = config["updating"].to_string() != "false"; // any value different than 'false'
                working_file = config["working_file"].to_string();
                output_file = config["output_file"].to_string();
                raw_list_movie_file = config["raw_list_movie_file"].to_string();
                filter_node_format = config["filter_node_format"].to_string_vector();
                loaded = true;
                break;
            [[unlikely]] default:
                assert("Wrong config file version.");
            }
        }
    }
};
