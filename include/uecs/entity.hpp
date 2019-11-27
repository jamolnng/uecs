#pragma once

#include <iostream>

#include <uecs/component.hpp>
#include <uecs/config.hpp>
#include <uecs/type.hpp>
#include <uecs/unique_container.hpp>
#include <uecs/util.hpp>

namespace uecs {
class Entity : public UniqueContainer<Component, MAX_ENTITY_COMPONENTS> {
 public:
  const id_type id() const;

 private:
  Entity(id_type id) : _id(id) {}
  Entity(const Entity &) = delete;
  Entity(Entity &&) = delete;
  friend class EntityManager;
  friend struct std::pair<const uecs::id_type, uecs::Entity>;
  id_type _id;
};
}  // namespace uecs