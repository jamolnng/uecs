#pragma once

#include <bitset>
#include <type_traits>
#include <unordered_map>

#include <uecs/component.hpp>
#include <uecs/config.hpp>
#include <uecs/type.hpp>

namespace uecs {
class Entity {
 private:
  template <typename C>
  using enable_if_component =
      std::enable_if<std::is_base_of<Component, C>::value>;

 public:
  Entity() : _id(0) {}
  const id_type id() const;

  template <typename C, typename... Args, typename = enable_if_component<C>>
  C* add_component(Args&&... args) {
    id_type cid = TypeID<C, Component>::value();
    if (_component_mask.test(cid)) {
      return nullptr;
    }
    _component_mask.set(cid);
    C* c = new C(std::forward<Args>(args)...);
    _components[cid] = c;
    return c;
  }

  template <typename C, typename = enable_if_component<C>>
  void remove_component() {
    id_type cid = TypeID<C, Component>::value();
    if (!_component_mask.test(cid)) {
      return;
    }
    _component_mask.reset(cid);
    C* c = static_cast<C*>(_components[cid]);
    delete c;
    _components.erase(cid);
  }

  template <typename C, typename = enable_if_component<C>>
  C* get_component() {
    id_type cid = TypeID<C, Component>::value();
    if (!_component_mask.test(cid)) {
      return nullptr;
    }
    return static_cast<C*>(_components[cid]);
  }

  template <typename C, typename = enable_if_component<C>>
  bool has_component() {
    return _component_mask.test(TypeID<C, Component>::value());
  }

 private:
  id_type _id;
  std::bitset<MAX_ENTITY_COMPONENTS> _component_mask;
  std::unordered_map<id_type, Component*> _components;
};
}  // namespace uecs