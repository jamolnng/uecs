#include <uecs/component_manager.hpp>

using namespace uecs;

ComponentManager::ComponentManager(EventManager& event_manager)
    : _event_manager(event_manager) {}

const ComponentManager::ComponentMask& ComponentManager::component_mask(
    Entity& e) {
  return _entity_components[e.id()].mask();
}

void ComponentManager::remove_all(Entity& e) {
  for (auto c : _entity_component_helpers[e.id()]) {
    c->remove(*this, e);
  }
  _entity_components[e.id()].clear();
}