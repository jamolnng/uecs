#pragma once

#include <bitset>
#include <memory>
#include <type_traits>
#include <unordered_map>

#include <uecs/config.hpp>
#include <uecs/type.hpp>

namespace uecs {
template <typename T, size_t MAX_T>
class UniqueContainer {
 protected:
  template <typename C>
  using enable_if_T = std::enable_if<std::is_base_of<T, C>::value>;

 public:
  UniqueContainer() = default;

  template <typename C, typename... Args, typename = enable_if_T<C>>
  std::shared_ptr<C> add(Args&&... args) {
    id_type cid = TypeID<C, T>::value();
    if (_mask.test(cid)) {
      return std::shared_ptr<C>(nullptr);
    }
    _mask.set(cid);
    std::shared_ptr<C> c = std::make_shared<C>(std::forward<Args>(args)...);
    _ts[cid] = c;
    return c;
  }

  template <typename C, typename = enable_if_T<C>>
  void remove() {
    id_type cid = TypeID<C, T>::value();
    if (!_mask.test(cid)) {
      return;
    }
    _mask.reset(cid);
    _ts.erase(cid);
  }

  template <typename C, typename = enable_if_T<C>>
  std::shared_ptr<C> get() {
    id_type cid = TypeID<C, T>::value();
    if (!_mask.test(cid)) {
      return std::shared_ptr<C>();
    }
    return std::shared_ptr<C>(std::static_pointer_cast<C>(_ts[cid]));
  }

  template <typename C, typename = enable_if_T<C>>
  bool has() {
    return _mask.test(TypeID<C, T>::value());
  }

 protected:
  std::bitset<MAX_T> _mask;
  std::unordered_map<id_type, std::shared_ptr<T>> _ts;
};
}  // namespace uecs