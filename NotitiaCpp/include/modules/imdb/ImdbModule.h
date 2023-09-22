#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Imdb.h"
#include "config/ConfigData.h"
#include "common/Utils.h"
#include "common/JsonFile.h"

class ImdbModule {
public:
    ImdbModule(auto& db_items, boolean ignoreLoaded = false) {
        if (loaded && !ignoreLoaded)
            return;
        loaded.store(true);

        load_data(db_items);
    }

protected:
    void load_data(auto& db_items) {
        data_imdbs = JsonFile::read_file(db_items["data"].to_string());
        data_imdbs_akas = JsonFile::read_file(db_items["data_akas"].to_string());
        data_imdbs_ratings = JsonFile::read_file(db_items["data_ratings"].to_string());
    }

    static inline std::atomic_bool loaded{ false };
    static inline std::string data_imdbs;
    static inline std::string data_imdbs_akas;
    static inline std::string data_imdbs_ratings;
};
