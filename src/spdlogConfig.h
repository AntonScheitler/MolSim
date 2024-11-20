// change active spdlog level to SPDLOG_LEVEL_OFF to exclude any macro logging statements in compile time
#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"
