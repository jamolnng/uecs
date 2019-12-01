#pragma once

#include <memory>
#include <unordered_map>

#include <uecs/component.hpp>
#include <uecs/entity.hpp>
#include <uecs/unique_container.hpp>
#include <uecs/util.hpp>

namespace uecs {
class ComponentManager : public NonCopyable {
 public:
  using ComponentContainer = UniqueContainer<Component, MAX_ENTITY_COMPONENTS>;
  using EntityComponentContainer =
      std::unordered_map<id_type, ComponentContainer>;
  using ComponentMask = ComponentContainer::mask_type;

  template <typename C, typename... Args>
  std::shared_ptr<C> create(id_type entity_id, Args&&... args) {
    return _check_exists(entity_id).add<C>(std::forward<Args>(args)...);
  }

  template <typename C, typename... Args>
  inline std::shared_ptr<C> create(const Entity& e, Args&&... args) {
    return create<C, Args...>(e.id(), std::forward<Args>(args)...);
  }

  template <typename C>
  std::shared_ptr<C> bind(id_type entity_id, C&& component) {
    return _check_exists(entity_id).insert<C>(component);
  }

  template <typename C>
  inline std::shared_ptr<C> bind(const Entity& e, C&& component) {
    return bind<C>(e.id(), component);
  }

  template <typename C>
  std::shared_ptr<C> bind(id_type entity_id, std::shared_ptr<C> c) {
    return _check_exists(entity_id).insert<C>(c);
  }

  template <typename C>
  inline std::shared_ptr<C> bind(const Entity& e, std::shared_ptr<C> c) {
    return bind<C>(e.id(), c);
  }

  template <typename C>
  std::shared_ptr<C> transfer(id_type entity_to, id_type entity_from) {
    auto& f = _check_exists(entity_from);
    std::shared_ptr<C> from = f.get<C>();
    f.remove<C>();
    return _check_exists(entity_to).insert(from);
  }

  template <typename C>
  inline std::shared_ptr<C> transfer(const Entity& to, const Entity& from) {
    return transfer<C>(to.id(), from.id());
  }

  template <typename C>
  std::shared_ptr<C> get(id_type entity_id) {
    return _check_exists(entity_id).get<C>();
  }

  template <typename C>
  inline std::shared_ptr<C> get(const Entity& e) {
    return get<C>(e.id());
  }

  template <typename C>
  bool has(id_type entity_id) {
    return _check_exists(entity_id).has<C>();
  }

  template <typename C>
  inline bool has(const Entity& e) {
    return has<C>(e.id());
  }

  template <typename C>
  void remove(id_type entity_id) {
    _check_exists(entity_id).remove<C>();
  }

  template <typename C>
  inline void remove(const Entity& e) {
    remove<C>(e.id());
  }

  const ComponentMask& component_mask(id_type entity_id);
  const ComponentMask& component_mask(const Entity& e);

 private:
  friend class EntityManager;
  EntityComponentContainer _entity_components;

  ComponentContainer& _check_exists(id_type entity_id);
};
}  // namespace uecs