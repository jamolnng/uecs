#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <type_traits>

#include <uecs/type.hpp>

namespace uecs {
template <typename T, size_t MAX_T>
class UniqueContainer {
 protected:
  template <typename C>
  using enable_if_T = std::enable_if<std::is_base_of<T, C>::value>;

 public:
  using mask_type = std::bitset<MAX_T>;
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
    _ts[cid].reset();
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
  std::shared_ptr<C> insert(std::shared_ptr<C> c) {
    id_type cid = TypeID<C, T>::value();
    if (_mask.test(cid)) {
      return std::shared_ptr<C>(nullptr);
    }
    _mask.set(cid);
    _ts[cid] = std::move(c);
    return std::shared_ptr<C>(std::static_pointer_cast<C>(_ts[cid]));
  }

  template <typename C, typename = enable_if_T<C>>
  std::shared_ptr<C> insert(C&& c) {
    id_type cid = TypeID<C, T>::value();
    if (_mask.test(cid)) {
      return std::shared_ptr<C>(nullptr);
    }
    _mask.set(cid);
    _ts[cid] = std::make_shared<C>(std::move(c));
    return std::shared_ptr<C>(std::static_pointer_cast<C>(_ts[cid]));
  }

  template <typename C, typename = enable_if_T<C>>
  bool has() {
    return _mask.test(TypeID<C, T>::value());
  }

  const mask_type& mask() { return _mask; }

 protected:
  mask_type _mask;
  std::array<std::shared_ptr<T>, MAX_T> _ts;
};
}  // namespace uecs