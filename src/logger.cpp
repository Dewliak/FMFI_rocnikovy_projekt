//
// Created by dewliak on 4/3/26.
//

#include "logger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>

//basic logger setup

namespace logger {

    void init() {
        static bool initialized = false;
        if (initialized) return;
        initialized = true;

        std::filesystem::create_directories("logs");

        auto search_logger = spdlog::basic_logger_mt("search", "logs/app.log");
        auto error_logger = spdlog::basic_logger_mt("error", "logs/error.log");

        search_logger->set_level(spdlog::level::trace);
        error_logger->set_level(spdlog::level::err);

        spdlog::set_default_logger(search_logger);
    }
}