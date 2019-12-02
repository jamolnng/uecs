#pragma once

#include <iostream>

#include <functional>
#include <iterator>
#include <set>
#include <unordered_map>

#include <uecs/component.hpp>
#include <uecs/component_manager.hpp>
#include <uecs/entity.hpp>
#include <uecs/event.hpp>
#include <uecs/event_manager.hpp>
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
    inline iterator& operator++(int) {
      ++(*this);
      return *this;
    }
    inline bool operator==(iterator& it) const { return it._index == _index; }
    inline bool operator!=(iterator& it) { return it._index != _index; }
    inline Entity& operator*() { return _index->second; }

   private:
    int_iter _index;
  };

  template <typename... Components>
  class ComponentView {
   public:
    class iterator : public std::iterator<std::forward_iterator_tag, Entity> {
      using int_iter = EntityContainer::iterator;

     public:
      iterator(ComponentManager& cm, int_iter index, int_iter end)
          : _cm(cm),
            _index(index),
            _end(end),
            _mask(ComponentManager::component_mask<Components...>()) {
        while (_index != _end &&
               (_mask & _cm.component_mask(_index->second)) != _mask) {
          ++_index;
        }
      }

      inline iterator& operator++() {
        do {
          ++_index;
        } while (_index != _end &&
                 (_mask & _cm.component_mask(_index->second)) != _mask);
        return *this;
      }
      inline iterator& operator++(int) {
        ++(*this);
        return *this;
      }
      inline bool operator==(iterator& it) const { return it._index == _index; }
      inline bool operator!=(iterator& it) const { return it._index != _index; }
      inline Entity& operator*() const { return _index->second; }

     private:
      ComponentManager& _cm;
      ComponentMask _mask;
      int_iter _index, _end;
    };
    ComponentView(EntityManager& em) : _em(em) {}

    iterator begin() {
      return iterator(_em._component_manager, _em._entities.begin(),
                      _em._entities.end());
    }
    iterator end() {
      return iterator(_em._component_manager, _em._entities.end(),
                      _em._entities.end());
    }

    size_t size() {
      size_t s = 0;
      for (auto it = begin(); it != end(); ++it, ++s) {
      }
      return s;
    }
    bool empty() { return size() == 0; }

    void for_each(std::function<void(Entity& entity, Components&...)> f) {
      for (auto& e : *this) {
        f(e, *(_em._component_manager.get<Components>(e).get())...);
      }
    }

   private:
    EntityManager& _em;
  };

  struct EntityCreatedEvent : public Event {
    EntityCreatedEvent(Entity& entity) : entity(entity) {}
    Entity& entity;
  };
  struct EntityDestroyedEvent : public Event {
    EntityDestroyedEvent(Entity& entity) : entity(entity) {}
    Entity& entity;
  };

  EntityManager(ComponentManager& component_manager,
                EventManager& event_manager);

  Entity& create();
  void destroy(Entity& e);

  iterator begin() { return iterator(_entities.begin()); }
  iterator end() { return iterator(_entities.end()); }
  size_t size() { return _entities.size(); }
  bool empty() { return _entities.empty(); }

  template <typename... Components>
  ComponentView<Components...> component_view() {
    return ComponentView<Components...>(*this);
  }

  template <typename... Components>
  void for_each(typename identity<
                std::function<void(Entity& entity, Components&...)>>::type f) {
    component_view<Components...>().for_each(f);
  }

 private:
  id_type _next_id{0};
  std::set<id_type> _unused_ids;
  EntityContainer _entities;
  ComponentManager& _component_manager;
  EventManager& _event_manager;

  id_type reserve_id();
  void release_id(id_type id);
};
}  // namespace uecs