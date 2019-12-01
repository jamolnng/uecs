#pragma once

#include <functional>
#include <map>
#include <memory>
#include <type_traits>

#include <uecs/config.hpp>
#include <uecs/event.hpp>
#include <uecs/event_receiver.hpp>
#include <uecs/type.hpp>

namespace uecs {
class EventManager {
 private:
  template <typename E>
  using enable_if_E = std::enable_if<std::is_base_of<Event, E>::value>;
  template <typename R>
  using enable_if_R = std::enable_if<std::is_base_of<EventReceiver, R>::value>;

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
    auto range = _receivers.equal_range(TypeID<E, Event>::value());
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
    auto range = _receivers.equal_range(TypeID<E, Event>::value());
    for (auto i = range.first; i != range.second; ++i) {
      i->second.second(&e);
    }
  }

  template <typename E, typename Receiver, typename = enable_if_E<E>,
            typename = enable_if_R<Receiver>>
  void subscribe(Receiver& receiver) {
    void (Receiver::*f)(const E& e) = &Receiver::receive;
    auto cbw =
        CallbackWrapper<E>(std::bind(f, &receiver, std::placeholders::_1));
    _receivers.insert(std::make_pair(TypeID<E, Event>::value(),
                                     std::make_pair(&receiver, cbw)));
  }

  template <typename E, typename Receiver, typename = enable_if_E<E>,
            typename = enable_if_R<Receiver>>
  void unsubscribe(Receiver& receiver) {
    auto range = _receivers.equal_range(TypeID<E, Event>::value());
    auto i = range.first;
    for (; i != range.second; ++i) {
      if (i->second.first == &receiver) {
        break;
      }
    }
    if (i != range.second) {
      _receivers.erase(i);
    }
  }

 private:
  std::multimap<id_type, std::pair<const EventReceiver*,
                                   std::function<void(const void*)>>>
      _receivers;
};
}  // namespace uecs