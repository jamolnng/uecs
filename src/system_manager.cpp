#include <uecs/system_manager.hpp>

using namespace uecs;

SystemManager::SystemManager(EntityManager& entity_manager,
                             ComponentManager& component_manager)
    : _entity_manager(entity_manager), _component_manager(component_manager) {}

void SystemManager::update(double dt) {
  for (auto& s : _ts) {
    s->update(dt);
  }
}