#pragma once

#include <utility>

#include <uecs/type.hpp>

namespace uecs {
class EntityManager;
class Entity {
 public:
  id_type id() const noexcept;
  void destroy() noexcept;
  bool valid() const noexcept;

  bool operator==(const Entity &other) const noexcept;
  bool operator!=(const Entity &other) const noexcept;

 private:
  Entity(EntityManager *manager, id_type id) noexcept;
  Entity(const Entity &) = delete;
  Entity(Entity &&) = delete;
  friend class EntityManager;
  friend struct std::pair<const uecs::id_type, uecs::Entity>;
  bool _valid{true};
  EntityManager *_manager;
  id_type _id;
};
}  // namespace uecs