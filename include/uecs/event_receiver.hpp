#pragma once

#include <type_traits>

#include <uecs/event.hpp>

namespace uecs {
template <typename T,
          typename = std::enable_if<std::is_base_of<Event, T>::value>>
class EventReceiver {
 public:
  virtual void receive(const T& t) = 0;
};
}  // namespace uecs