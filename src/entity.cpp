#include <uecs/entity.hpp>

#include <uecs/entity_manager.hpp>

using namespace uecs;

Entity::Entity(EntityManager *manager, id_type id) noexcept
    : _manager(manager), _id(id) {}

id_type Entity::id() const noexcept { return _id; }

void Entity::destroy() noexcept {
  if (valid()) {
    _valid = false;
    _manager->destroy(*this);
  }
}
bool Entity::valid() const noexcept { return _valid; }

bool Entity::operator==(const Entity &other) const noexcept {
  return _id == other._id;
}
bool Entity::operator!=(const Entity &other) const noexcept {
  return _id != other._id;
}