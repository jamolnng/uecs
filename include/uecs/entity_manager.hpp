#pragma once

#include <iostream>

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
  using ComponentMask = ComponentManager::ComponentMask;
  using ComponentContainer = ComponentManager::ComponentContainer;
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

  template <typename... Args>
  class ComponentView {
   public:
    class iterator : public std::iterator<std::forward_iterator_tag,
                                          std::shared_ptr<Component>> {
      using int_iter = EntityContainer::iterator;

     public:
      iterator(EntityManager& em, int_iter index, int_iter end)
          : _em(em),
            _index(index),
            _end(end),
            _mask(EntityManager::component_mask<Args...>()) {
        while (_index != _end &&
               (_mask & _em.component_mask(_index->second)) != _mask) {
          ++_index;
        }
      }

      inline iterator& operator++() {
        do {
          ++_index;
        } while (_index != _end &&
                 (_mask & _em.component_mask(_index->second)) != _mask);
        return *this;
      }
      inline bool operator!=(iterator& it) { return it._index != _index; }
      inline Entity& operator*() { return _index->second; }

     private:
      EntityManager& _em;
      ComponentMask _mask;
      int_iter _index, _end;
    };
    ComponentView(EntityManager& em) : _em(em) {}

    iterator begin() {
      return iterator(_em, _em._entities.begin(), _em._entities.end());
    }
    iterator end() {
      return iterator(_em, _em._entities.end(), _em._entities.end());
    }

   private:
    EntityManager& _em;
  };

  EntityManager(ComponentManager& component_manager);

  Entity& create();
  void destroy(id_type id);

  const ComponentMask& component_mask(id_type id);
  const ComponentMask& component_mask(const Entity& e);

  iterator begin() { return iterator(_entities.begin()); }
  iterator end() { return iterator(_entities.end()); }
  size_t size() { return _entities.size(); }
  bool empty() { return _entities.empty(); }

  template <typename... Components>
  ComponentView<Components...> component_view() {
    return ComponentView<Components...>(*this);
  }

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