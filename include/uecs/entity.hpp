#pragma once

#include <utility>

#include <uecs/type.hpp>

namespace uecs {
class EntityManager;
class Entity {
 public:
  const id_type id() const;
  void destroy();
  bool valid();

  bool operator==(const Entity &other);
  bool operator!=(const Entity &other);

 private:
  Entity(EntityManager *manager, id_type id) : _manager(manager), _id(id) {}
  Entity(const Entity &) = delete;
  Entity(Entity &&) = delete;
  friend class EntityManager;
  friend struct std::pair<const uecs::id_type, uecs::Entity>;
  id_type _id;
  bool _valid{true};
  EntityManager *_manager;
};
}  // namespace uecs