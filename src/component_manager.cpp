#include <uecs/component_manager.hpp>

using namespace uecs;

ComponentManager::ComponentManager(EventManager& event_manager)
    : _event_manager(event_manager) {}

const ComponentManager::ComponentMask& ComponentManager::component_mask(
    Entity& e) {
  return _check_exists(e.id()).first.mask();
}

void ComponentManager::remove_all(Entity& e) {
  _check_exists(e.id());
  for (auto c : _entity_component_helpers.at(e.id())) {
    c->remove(*this, e);
  }
  _entity_components.at(e.id()).clear();
}

std::pair<ComponentManager::ComponentContainer&,
          ComponentManager::ComponentHelperContainer&>
ComponentManager::_check_exists(id_type entity_id) {
  ComponentContainer& cc =
      _entity_components.emplace(entity_id, ComponentContainer()).first->second;
  ComponentHelperContainer& chc =
      _entity_component_helpers.emplace(entity_id, ComponentHelperContainer())
          .first->second;
  return std::pair<ComponentContainer&, ComponentHelperContainer&>(cc, chc);
}