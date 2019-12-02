#include <uecs/entity.hpp>

#include <uecs/entity_manager.hpp>

using namespace uecs;

const id_type Entity::id() const { return _id; }

void Entity::destroy() {
  if (valid()) {
    _valid = false;
    _manager->destroy(*this);
  }
}
bool Entity::valid() { return _valid; }

bool Entity::operator==(const Entity &other) { return _id == other._id; }
bool Entity::operator!=(const Entity &other) { return _id != other._id; }