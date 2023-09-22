#pragma once

#include <string>
#include <vector>
#include "common/JsonFile.h"

class ConfigData {
public:
    bool loaded{ false };

    int version{ 1 };
    bool updating{ false }; // update existing list of movies or create a new list from scratch
    std::string language{ "EN" };
    std::string working_file{ "" }; // current list of files to update
    std::string output_file{ "" }; // output files with list of files, it could be same as working_file
    std::string raw_list_movie_file{ "" }; // list of names to process
    std::vector<std::string> filter_node_format{};

    JsonVariant config;
};
