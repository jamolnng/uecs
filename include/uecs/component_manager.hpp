#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

#include <uecs/component.hpp>
#include <uecs/entity.hpp>
#include <uecs/event.hpp>
#include <uecs/event_manager.hpp>
#include <uecs/unique_container.hpp>
#include <uecs/util.hpp>

namespace uecs {
class ComponentManager : public NonCopyable {
 private:
  class BaseComponentHelper {
   public:
    virtual void remove(ComponentManager& manager, Entity& e) = 0;
  };
  template <typename C>
  using enable_if_C = std::enable_if<std::is_base_of<Component, C>::value>;
  template <typename C>
  class ComponentHelper : public BaseComponentHelper {
   public:
    virtual void remove(ComponentManager& manager, Entity& e) {
      manager.remove<C>(e);
    }
  };

 public:
  using ComponentContainer = UniqueContainer<Component, MAX_ENTITY_COMPONENTS>;
  using ComponentHelperContainer =
      UniqueContainer<BaseComponentHelper, MAX_ENTITY_COMPONENTS>;
  using EntityComponentContainer =
      std::unordered_map<id_type, ComponentContainer>;
  using EntityComponentHelperContainer =
      std::unordered_map<id_type, ComponentHelperContainer>;
  using ComponentMask = ComponentContainer::mask_type;

  template <typename C, typename = enable_if_C<C>>
  struct ComponentCreatedEvent : public Event {
    ComponentCreatedEvent(Entity& entity, std::shared_ptr<C> component)
        : entity(entity), component(component) {}
    Entity& entity;
    std::shared_ptr<C> component;
  };

  template <typename C, typename = enable_if_C<C>>
  struct ComponentBoundEvent : public Event {
    ComponentBoundEvent(Entity& entity, std::shared_ptr<C> component)
        : entity(entity), component(component) {}
    Entity& entity;
    std::shared_ptr<C> component;
  };

  template <typename C, typename = enable_if_C<C>>
  struct ComponentSwappedEvent : public Event {
    ComponentSwappedEvent(Entity& to, Entity& from,
                          std::shared_ptr<C> component)
        : to(to), from(from), component(component) {}
    Entity& to;
    Entity& from;
    std::shared_ptr<C> component;
  };

  template <typename C, typename = enable_if_C<C>>
  struct ComponentRemovedEvent : public Event {
    ComponentRemovedEvent(Entity& entity, std::shared_ptr<C> component)
        : entity(entity), component(component) {}
    Entity& entity;
    std::shared_ptr<C> component;
  };

  template <typename C, typename = enable_if_C<C>>
  struct ComponentDestroyedEvent : public Event {
    ComponentDestroyedEvent(Entity& entity, std::shared_ptr<C> component)
        : entity(entity), component(component) {}
    Entity& entity;
    std::shared_ptr<C> component;
  };

  ComponentManager(EventManager& event_manager);

  template <typename C, typename... Args, typename = enable_if_C<C>>
  std::shared_ptr<C> create(Entity& e, Args&&... args) {
    auto p = _check_exists(e.id());
    std::shared_ptr<C> r = p.first.add<C>(std::forward<Args>(args)...);
    p.second.add<ComponentHelper<C>>();
    _event_manager.emit<ComponentCreatedEvent<C>>(e, r);
    return r;
  }

  template <typename C, typename = enable_if_C<C>>
  std::shared_ptr<C> bind(Entity& e, C&& component) {
    auto p = _check_exists(e.id());
    p.second.add<ComponentHelper<C>>();
    std::shared_ptr<C> r = p.first.insert<C>(component);
    _event_manager.emit<ComponentBoundEvent<C>>(e, r);
    return r;
  }

  template <typename C, typename = enable_if_C<C>>
  std::shared_ptr<C> bind(Entity& e, std::shared_ptr<C> component) {
    auto p = _check_exists(e.id());
    p.second.add<ComponentHelper<C>>();
    std::shared_ptr<C> r = p.first.insert<C>(component);
    _event_manager.emit<ComponentBoundEvent<C>>(e, r);
    return r;
  }

  template <typename C, typename = enable_if_C<C>>
  std::shared_ptr<C> swap(Entity& to, Entity& from) {
    auto f = _check_exists(from.id());
    f.second.add<ComponentHelper<C>>();
    std::shared_ptr<C> c = f.first.get<C>();
    f.first.remove<C>();
    f.second.remove<ComponentHelper<C>>();
    auto p = _check_exists(to.id());
    std::shared_ptr<C> r = p.first.insert<C>(c);
    _event_manager.emit<ComponentSwappedEvent<C>>(to, from, r);
    return r;
  }

  template <typename C, typename = enable_if_C<C>>
  std::shared_ptr<C> get(Entity& e) {
    auto p = _check_exists(e.id());
    return p.first.get<C>();
  }

  template <typename C, typename = enable_if_C<C>>
  bool has(const Entity& e) {
    auto p = _check_exists(e.id());
    return p.first.has<C>();
  }

  template <typename C, typename = enable_if_C<C>>
  void remove(Entity& e) {
    auto p = _check_exists(e.id());
    std::shared_ptr<C> r = p.first.get<C>();
    p.first.remove<C>();
    p.second.remove<ComponentHelper<C>>();
    _event_manager.emit<ComponentRemovedEvent<C>>(e, r);
    if (r.use_count() == 1) {
      _event_manager.emit<ComponentDestroyedEvent<C>>(e, r);
    }
  }

  void remove_all(Entity& e);

  const ComponentMask& component_mask(Entity& e);

  template <typename C>
  static typename std::enable_if<std::is_base_of<Component, C>::value,
                                 ComponentMask>::type
  component_mask() {
    ComponentMask mask;
    mask.set(TypeID<C, Component>::value());
    return mask;
  }

  template <typename C1, typename C2, typename... Components>
  static ComponentMask component_mask() {
    return component_mask<C1>() | component_mask<C2, Components...>();
  }

 private:
  friend class EntityManager;
  EntityComponentContainer _entity_components;
  EntityComponentHelperContainer _entity_component_helpers;
  EventManager& _event_manager;

  std::pair<ComponentContainer&, ComponentHelperContainer&> _check_exists(
      id_type entity_id) {
    ComponentContainer* cc = nullptr;
    ComponentHelperContainer* chc = nullptr;
    auto ec = _entity_components.find(entity_id);
    if (ec != _entity_components.end()) {
      cc = &ec->second;
    } else {
      _entity_components[entity_id] = ComponentContainer();
      cc = &_entity_components[entity_id];
    }
    auto ech = _entity_component_helpers.find(entity_id);
    if (ech != _entity_component_helpers.end()) {
      chc = &ech->second;
    } else {
      _entity_component_helpers[entity_id] = ComponentHelperContainer();
      chc = &_entity_component_helpers[entity_id];
    }
    return std::pair<ComponentContainer&, ComponentHelperContainer&>(*cc, *chc);
  }
};
}  // namespace uecs