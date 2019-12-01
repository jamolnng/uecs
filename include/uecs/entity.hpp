#pragma once

#include <utility>

#include <uecs/type.hpp>

namespace uecs {
class Entity {
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