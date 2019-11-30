#include <uecs/component_manager.hpp>

using namespace uecs;

ComponentManager::ComponentContainer& ComponentManager::_check_exists(
    id_type entity_id) {
  return _entity_components.try_emplace(entity_id).first->second;
}