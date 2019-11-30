#pragma once

#include <memory>
#include <type_traits>

#include <uecs/event.hpp>
#include <uecs/event_receiver.hpp>

namespace uecs {
class EventManager {
 private:
  template <typename E>
  using enable_if_E = std::enable_if<std::is_base_of<Event, E>::value>;

 public:
  EventManager() = default;

  template <typename E, typename = enable_if_E<E>>
  void emit(T& args) {}

  template <typename E, typename = enable_if_E<E>>
  void emit(std::unique_ptr<E> event) {}

  template <typename E, typename... Args, typename = enable_if_E<E>>
  void emit(Args&&... args) {}

  template <typename E>
  void subscribe(const EventReceiver<E>& receiver) {}

 private:
};
}  // namespace uecs