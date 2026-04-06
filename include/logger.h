//
// Created by dewliak on 4/3/26.
//

#ifndef ROCNIKOVY_PROJEKT_LOG_H
#define ROCNIKOVY_PROJEKT_LOG_H

#include <spdlog/spdlog.h>

namespace logger {
    void init();
}

inline std::shared_ptr<spdlog::logger> search() {
    return spdlog::get("search");
}

inline std::shared_ptr<spdlog::logger> error() {
    return spdlog::get("error");
}


#define LOG_TRACE(...)  do { if(auto l = search()) l->trace(__VA_ARGS__); } while(0)
#define LOG_DEBUG(...)  do { if(auto l = search()) l->debug(__VA_ARGS__); } while(0)
#define LOG_INFO(...)   do { if(auto l = search()) l->info(__VA_ARGS__); } while(0)
#define LOG_WARN(...)   do { if(auto l = search()) l->warn(__VA_ARGS__); } while(0)
#define LOG_ERROR(...) do { if(auto l1 = search()) l1->error(__VA_ARGS__);if(auto l2 = error())  l2->error(__VA_ARGS__);} while(0)
#define LOG_CRITICAL(...) do { if(auto l1 = search()) l1->critical(__VA_ARGS__); if(auto l2 = error())  l2->critical(__VA_ARGS__); } while(0)

#endif //ROCNIKOVY_PROJEKT_LOG_H