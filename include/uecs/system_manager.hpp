#pragma once

#include <uecs/component_manager.hpp>
#include <uecs/config.hpp>
#include <uecs/entity_manager.hpp>
#include <uecs/event_manager.hpp>
#include <uecs/system.hpp>
#include <uecs/unique_container.hpp>
#include <uecs/util.hpp>

namespace uecs {
class SystemManager : public NonCopyable,
                      public UniqueContainer<System, MAX_SYSTEMS> {
 public:
  SystemManager(EntityManager& entity_manager,
                ComponentManager& component_manager,
                EventManager& event_manager);

  template <typename S, typename = if_type<S>>
  void update(EntityManager& entity_manager, double dt) {
    get<S>()->update(dt);
  }

  void update(double dt);

 private:
  EntityManager& _entity_manager;
  ComponentManager& _component_manager;
  EventManager& _event_manager;
};
}  // namespace uecs