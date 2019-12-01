#pragma once

#include <functional>
#include <map>
#include <memory>
#include <type_traits>

#include <uecs/config.hpp>
#include <uecs/event.hpp>
#include <uecs/event_listener.hpp>
#include <uecs/type.hpp>

namespace uecs {
class EventManager {
 private:
  template <typename E>
  using enable_if_E = std::enable_if<std::is_base_of<Event, E>::value>;
  template <typename R>
  using enable_if_R = std::enable_if<std::is_base_of<EventListener, R>::value>;

  template <typename E>
  struct CallbackWrapper {
    explicit CallbackWrapper(std::function<void(const E&)> callback)
        : _callback(callback) {}
    void operator()(const void* e) { _callback(*(static_cast<const E*>(e))); }
    std::function<void(const E&)> _callback;
  };

 public:
  EventManager() = default;

  template <typename E, typename = enable_if_E<E>>
  void emit(E* e) {
    auto range = _listeners.equal_range(TypeID<E, Event>::value());
    for (auto i = range.first; i != range.second; ++i) {
      i->second.second(e);
    }
  }

  template <typename E, typename = enable_if_E<E>>
  void emit(E& e) {
    emit<E>(&e);
  }

  template <typename E, typename = enable_if_E<E>>
  void emit(std::shared_ptr<E> e) {
    emit<E>(*e);
  }

  template <typename E, typename... Args, typename = enable_if_E<E>>
  void emit(Args&&... args) {
    E e(std::forward<Args>(args)...);
    auto range = _listeners.equal_range(TypeID<E, Event>::value());
    for (auto i = range.first; i != range.second; ++i) {
      i->second.second(&e);
    }
  }

  template <typename E, typename Listener, typename = enable_if_E<E>,
            typename = enable_if_R<Listener>>
  void subscribe(Listener& listener) {
    void (Listener::*f)(const E& e) = &Listener::receive;
    auto cbw =
        CallbackWrapper<E>(std::bind(f, &listener, std::placeholders::_1));
    _listeners.insert(std::make_pair(TypeID<E, Event>::value(),
                                     std::make_pair(&listener, cbw)));
  }

  template <typename E, typename Listener, typename = enable_if_E<E>,
            typename = enable_if_R<Listener>>
  void unsubscribe(Listener& listener) {
    auto range = _listeners.equal_range(TypeID<E, Event>::value());
    auto i = range.first;
    for (; i != range.second; ++i) {
      if (i->second.first == &listener) {
        break;
      }
    }
    if (i != range.second) {
      _listeners.erase(i);
    }
  }

 private:
  std::multimap<id_type, std::pair<const EventListener*,
                                   std::function<void(const void*)>>>
      _listeners;
};
}  // namespace uecs