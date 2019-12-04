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
 private:
  using EntityContainer = std::unordered_map<id_type, Entity>;
  template <typename... Components>
  using view_fn = typename identity<
      std::function<void(Entity& entity, Components&...)>>::type;

  struct ComponentTestBase {
   public:
    ComponentTestBase() = delete;
  };

 public:
  class iterator : std::iterator<std::forward_iterator_tag, Entity> {
   private:
    using int_iter = EntityContainer::iterator;

   public:
    iterator(int_iter index) : _index(index) {}
    inline iterator& operator++() {
      ++_index;
      return *this;
    }
    inline iterator operator++(int) { return ++(*this); }
    inline bool operator==(iterator& it) const { return it._index == _index; }
    inline bool operator!=(iterator& it) { return it._index != _index; }
    inline Entity& operator*() { return _index->second; }

   private:
    int_iter _index;
  };

  template <typename validv, typename... Components>
  class ComponentView {
   public:
    class iterator : public std::iterator<std::forward_iterator_tag, Entity> {
     private:
      using int_iter = EntityContainer::iterator;

     public:
      iterator(EntityManager& em, int_iter index, int_iter end)
          : _em(em),
            _mask(ComponentManager::component_mask<Components...>()),
            _index(index),
            _end(end) {
        while (_index != _end &&
               !validv::valid(_mask, _em._component_manager.component_mask(
                                         _index->second))) {
          ++_index;
        }
      }
      inline iterator& operator++() {
        if (_index == _end) {
          return *this;
        }
        do {
          ++_index;
        } while (_index != _end &&
                 !validv::valid(_mask, _em._component_manager.component_mask(
                                           _index->second)));
        return *this;
      }
      inline iterator operator++(int) { return ++(*this); }
      inline bool operator==(iterator& it) const { return it._index == _index; }
      inline bool operator!=(iterator& it) const { return it._index != _index; }
      inline Entity& operator*() const { return _index->second; }

     private:
      EntityManager& _em;
      ComponentManager::ComponentMask _mask;
      int_iter _index, _end;
    };
    ComponentView(EntityManager& em) noexcept : _em(em) {}
    inline iterator begin() {
      return iterator(_em, _em._entities.begin(), _em._entities.end());
    }
    inline iterator end() {
      return iterator(_em, _em._entities.end(), _em._entities.end());
    }
    inline size_t size() {
      size_t s = 0;
      for (auto it = begin(); it != end(); ++it, ++s) {
      }
      return s;
    }
    inline bool empty() { return size() == 0; }
    void for_each(view_fn<Components...> f) {
      for (auto& e : *this) {
        f(e, *(_em._component_manager.get<Components>(e).get())...);
      }
    }

   protected:
    EntityManager& _em;
  };

  struct ContainsComponentsTest : public ComponentTestBase {
    static bool valid(const ComponentManager::ComponentMask& m1,
                      const ComponentManager::ComponentMask& m2) {
      return (m1 & m2) == m1;
    };
  };

  struct ExactComponentsTest : public ComponentTestBase {
    static bool valid(const ComponentManager::ComponentMask& m1,
                      const ComponentManager::ComponentMask& m2) {
      return m1 == m2;
    };
  };

  struct EntityCreatedEvent : public Event {
    EntityCreatedEvent(Entity& e) noexcept : entity(e) {}
    Entity& entity;
  };
  struct EntityDestroyedEvent : public Event {
    EntityDestroyedEvent(Entity& e) noexcept : entity(e) {}
    Entity& entity;
  };

  EntityManager(ComponentManager& component_manager,
                EventManager& event_manager);

  Entity& create();
  void destroy(Entity& e);

  bool exists(id_type entity_id);
  Entity& get(id_type entity_id);

  inline iterator begin() noexcept { return iterator(_entities.begin()); }
  inline iterator end() noexcept { return iterator(_entities.end()); }
  inline size_t size() const noexcept { return _entities.size(); }
  inline bool empty() const noexcept { return _entities.empty(); }

  template <typename ComponentTest, typename... Components>
  typename std::enable_if<
      std::is_base_of<ComponentTestBase, ComponentTest>::value,
      ComponentView<ComponentTest, Components...>>::type
  component_view() {
    return ComponentView<ComponentTest, Components...>(*this);
  }

  template <typename... Components>
  ComponentView<ContainsComponentsTest, Components...>
  contains_component_view() {
    return ComponentView<ContainsComponentsTest, Components...>(*this);
  }
  template <typename... Components>
  inline void contains_for_each(view_fn<Components...> f) {
    contains_component_view<Components...>().for_each(f);
  }

  template <typename... Components>
  ComponentView<ExactComponentsTest, Components...> exact_component_view() {
    return ComponentView<ExactComponentsTest, Components...>(*this);
  }
  template <typename... Components>
  inline void exact_for_each(view_fn<Components...> f) {
    exact_component_view<Components...>().for_each(f);
  }

 private:
  id_type _next_id{0};
  std::set<id_type> _unused_ids{};
  EntityContainer _entities{};
  ComponentManager& _component_manager;
  EventManager& _event_manager;

  id_type reserve_id();
  void release_id(id_type id);
};  // namespace uecs
}  // namespace uecs