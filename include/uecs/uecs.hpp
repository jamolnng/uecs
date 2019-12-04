#pragma once
#include <uecs/component.hpp>
#include <uecs/component_manager.hpp>
#include <uecs/config.hpp>
#include <uecs/entity.hpp>
#include <uecs/entity_manager.hpp>
#include <uecs/event.hpp>
#include <uecs/event_manager.hpp>
#include <uecs/system.hpp>
#include <uecs/system_manager.hpp>

namespace uecs {
class UECS {
 public:
  UECS();

  uecs::EventManager _events{};
  uecs::ComponentManager _components;
  uecs::EntityManager _entities;
  uecs::SystemManager _systems;
};
UECS::UECS()
    : _components(_events),
      _entities(_components, _events),
      _systems(_entities, _components, _events){};
}  // namespace uecs