#pragma once

#include <string>
#include <vector>
#include "common/JsonFile.h"
#include "config/ConfigData.h"

class ConfigHelper : public ConfigData {
public:
    ConfigHelper() {
        version = 1;
        language = "EN";
        updating = false;
        working_file = "";
        output_file = "";
        raw_list_movie_file = "";
        filter_node_format = {};

        loaded = true;
    }
};
