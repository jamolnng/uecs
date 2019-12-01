#pragma once

#include <iterator>
#include <set>
#include <unordered_map>

#include <uecs/component.hpp>
#include <uecs/component_manager.hpp>
#include <uecs/entity.hpp>
#include <uecs/type.hpp>
#include <uecs/util.hpp>

namespace uecs {
class EntityManager : public NonCopyable {
 public:
  using ComponentMask = ComponentManager::ComponentContainer::mask_type;
  using EntityContainer = std::unordered_map<id_type, Entity>;

  class iterator : std::iterator<std::forward_iterator_tag, Entity> {
   private:
    using int_iter = EntityContainer::iterator;

   public:
    iterator(int_iter index) : _index(index) {}

    inline iterator& operator++() {
      ++_index;
      return *this;
    }
    inline bool operator!=(iterator& it) { return it._index != _index; }
    inline Entity& operator*() { return _index->second; }

   private:
    int_iter _index;
  };
  iterator begin() { return iterator(_entities.begin()); }
  iterator end() { return iterator(_entities.end()); }

  EntityManager(ComponentManager& component_manager);

  Entity& create();
  void destroy(id_type id);

  const ComponentMask& component_mask(Entity& e);
  const ComponentMask& component_mask(id_type id);

  template <typename C>
  static ComponentMask component_mask() {
    ComponentMask mask;
    mask.set(TypeID<C, Component>::value());
    return mask;
  }

  template <typename C1, typename C2, typename... Components>
  static ComponentMask component_mask() {
    return component_mask<C1>() | component_mask<C2, Components...>();
  }

 private:
  id_type _next_id{0};
  std::set<id_type> _unused_ids;
  EntityContainer _entities;
  ComponentManager& _component_manager;

  id_type reserve_id();
  void release_id(id_type id);
};
}  // namespace uecs