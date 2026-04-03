//
// Created by dewliak on 4/3/26.
//

#ifndef ROCNIKOVY_PROJEKT_LOG_H
#define ROCNIKOVY_PROJEKT_LOG_H

#include <spdlog/spdlog.h>

namespace logger {
    void init();
}


#define LOG_TRACE(...) ::spdlog::get("search")->trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::spdlog::get("search")->debug(__VA_ARGS__)
#define LOG_INFO(...)  ::spdlog::get("search")->info(__VA_ARGS__)
#define LOG_WARN(...)  ::spdlog::get("search")->warn(__VA_ARGS__)
#define LOG_ERROR(...) do {spdlog::get("search")->error(__VA_ARGS__);spdlog::get("error")->error(__VA_ARGS__);} while(0)
#define LOG_CRITICAL(...) do {spdlog::get("search")->critical(__VA_ARGS__);spdlog::get("error")->critical(__VA_ARGS__);} while(0)

#endif //ROCNIKOVY_PROJEKT_LOG_H