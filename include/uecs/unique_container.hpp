#pragma once

#include <array>
#include <bitset>
#include <iterator>
#include <memory>
#include <type_traits>

#include <uecs/type.hpp>

namespace uecs {
template <typename T, size_t MAX_T>
class UniqueContainer {
 protected:
  template <typename C>
  using if_type = std::enable_if<std::is_base_of<T, C>::value>;

 public:
  using mask_type = std::bitset<MAX_T>;
  using array_type = std::array<std::shared_ptr<T>, MAX_T>;

  class iterator
      : public std::iterator<std::forward_iterator_tag, std::shared_ptr<T>> {
   private:
    using int_iter = size_t;

   public:
    iterator(int_iter index, array_type& arr) : _index(index), _arr(arr) {
      while (_index != MAX_T && _arr.at(_index) == nullptr) {
        ++_index;
      }
    }

    inline iterator& operator++() {
      if (_index == MAX_T) {
        return *this;
      }
      do {
        ++_index;
      } while (_index != MAX_T && _arr.at(_index) == nullptr);
      return *this;
    }
    inline iterator operator++(int) { return ++(*this); }
    inline bool operator==(iterator& it) const { return it._index == _index; }
    inline bool operator!=(iterator& it) const { return it._index != _index; }
    inline std::shared_ptr<T> operator*() const { return _arr.at(_index); }

   private:
    int_iter _index;
    array_type& _arr;
  };

  iterator begin() { return iterator(0, _ts); }
  iterator end() { return iterator(_ts.size(), _ts); }

  template <typename C, typename... Args, typename = if_type<C>>
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

  template <typename C, typename = if_type<C>>
  void remove() {
    id_type cid = TypeID<C, T>::value();
    if (!_mask.test(cid)) {
      return;
    }
    _mask.reset(cid);
    _ts[cid].reset();
  }

  template <typename C, typename = if_type<C>>
  std::shared_ptr<C> get() {
    id_type cid = TypeID<C, T>::value();
    if (!_mask.test(cid)) {
      return std::shared_ptr<C>();
    }
    return std::shared_ptr<C>(std::static_pointer_cast<C>(_ts[cid]));
  }

  template <typename C, typename = if_type<C>>
  std::shared_ptr<C> insert(std::shared_ptr<C> c) {
    id_type cid = TypeID<C, T>::value();
    if (_mask.test(cid)) {
      return std::shared_ptr<C>(nullptr);
    }
    _mask.set(cid);
    _ts[cid] = std::move(c);
    return std::shared_ptr<C>(std::static_pointer_cast<C>(_ts[cid]));
  }

  template <typename C, typename = if_type<C>>
  std::shared_ptr<C> insert(C&& c) {
    id_type cid = TypeID<C, T>::value();
    if (_mask.test(cid)) {
      return std::shared_ptr<C>(nullptr);
    }
    _mask.set(cid);
    _ts[cid] = std::make_shared<C>(std::move(c));
    return std::shared_ptr<C>(std::static_pointer_cast<C>(_ts[cid]));
  }

  void clear() {
    _mask.reset();
    for (auto& s : _ts) {
      s = nullptr;
    }
  }

  template <typename C, typename = if_type<C>>
  bool has() const {
    return _mask.test(TypeID<C, T>::value());
  }

  const mask_type& mask() const { return _mask; }

  size_t size() const { return _mask.count(); }
  size_t capacity() const { return MAX_T; }

 protected:
  mask_type _mask{};
  array_type _ts{};
};
}  // namespace uecs