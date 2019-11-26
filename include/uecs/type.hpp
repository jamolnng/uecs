#pragma once

#include <uecs/config.hpp>

namespace uecs {
struct TypeIDCounter {
 public:
  TypeIDCounter() = delete;
  ~TypeIDCounter() = delete;
  static id_type counter;
};
template <typename T, typename Base = void>
class TypeID {
 public:
  TypeID() = delete;
  ~TypeID() = delete;

  const static id_type value() {
    static id_type v = TypeIDCounter::counter++;
    return v;
  }
};
}  // namespace uecs