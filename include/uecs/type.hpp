#pragma once

#include <uecs/config.hpp>

namespace uecs {
namespace detail {
template <typename Base = void>
struct TypeIDCounter {
 public:
  TypeIDCounter() = delete;
  ~TypeIDCounter() = delete;
  static id_type& count() {
    static id_type c = 0;
    return c;
  }
};
}  // namespace detail
template <typename T, typename Base = void>
class TypeID {
 public:
  TypeID() = delete;
  ~TypeID() = delete;
  const static id_type value() {
    static id_type v = detail::TypeIDCounter<Base>::count()++;
    return v;
  }
};
}  // namespace uecs