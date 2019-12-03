#include <uecs/entity_manager.hpp>

#include <utility>

using namespace uecs;

EntityManager::EntityManager(ComponentManager& component_manager,
                             EventManager& event_manager)
    : _component_manager(component_manager), _event_manager(event_manager) {}

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
                    std::forward_as_tuple(this, id));
  auto& s = _entities.at(id);
  _event_manager.emit<EntityCreatedEvent>(s);
  return s;
}

void EntityManager::destroy(Entity& e) {
  if (e.valid()) {
    release_id(e.id());
    _component_manager.remove_all(e);
    _event_manager.emit<EntityDestroyedEvent>(e);
    _entities.erase(_entities.find(e.id()));
  }
}

bool EntityManager::exists(id_type entity_id) {
  return _entities.find(entity_id) != _entities.end();
}

Entity& EntityManager::get(id_type entity_id) {
  return _entities.at(entity_id);
}