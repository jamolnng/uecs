#pragma once

#include <stdint.h>
#include <cstdlib>

namespace uecs {
constexpr uint32_t make_version(uint32_t major, uint32_t minor,
                                uint32_t patch) {
  return (major << 20) | (minor << 10) | patch;
}
constexpr const char* VERSION_STR = "0.1.0";
constexpr uint32_t VERSION = make_version(0, 1, 0);
constexpr size_t MAX_ENTITY_COMPONENTS = 64;
constexpr size_t MAX_SYSTEMS = 64;
using id_type = uint64_t;
}  // namespace uecs
