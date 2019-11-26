#include <uecs/system_manager.hpp>

using namespace uecs;

SystemManager::SystemManager(EntityManager& entity_manager)
    : _entity_manager(entity_manager) {}

void SystemManager::update(double dt) {
  for (auto& s : _ts) {
    s.second->update(dt);
  }
}