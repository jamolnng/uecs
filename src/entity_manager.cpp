#include <uecs/entity_manager.hpp>

#include <utility>

using namespace uecs;

EntityManager::EntityManager(ComponentManager& component_manager)
    : _component_manager(component_manager) {}

id_type EntityManager::reserve_id() {
  id_type id;
  if (!_unused_ids.empty()) {
    auto b = _unused_ids.begin();
    id = *b;
    _unused_ids.erase(b);
  } else {
    id = _next_id++;
  }
  return id;
}

void EntityManager::release_id(id_type id) { _unused_ids.insert(id); }

Entity& EntityManager::create() {
  id_type id = reserve_id();
  _entities.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                    std::forward_as_tuple(id));
  return _entities.at(id);
}

void EntityManager::destroy(id_type id) {
  release_id(id);
  _entities.erase(_entities.find(id));
}

const EntityManager::ComponentMask& uecs::EntityManager::component_mask(
    Entity& e) {
  return _component_manager.component_mask(e);
}

const EntityManager::ComponentMask& EntityManager::component_mask(id_type id) {
  return _component_manager.component_mask(id);
}
