#pragma once

#include <stdint.h>

namespace uecs {
constexpr const char* VERSION_STR = "0.0.1";
constexpr uint32_t VERSION = (0 << 16) | (0 << 8) | 1;
constexpr size_t MAX_ENTITY_COMPONENTS = 64;
constexpr size_t MAX_SYSTEMS = 64;
using id_type = uint64_t;
}  // namespace ecs
