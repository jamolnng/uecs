#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

#include <uecs/config.hpp>
#include <uecs/entity_manager.hpp>
#include <uecs/system.hpp>
#include <uecs/unique_container.hpp>

namespace uecs {
class SystemManager : public UniqueContainer<System, MAX_SYSTEMS> {
 public:
  SystemManager(EntityManager& entity_manager);

  template <typename S, typename = enable_if_T<S>>
  void update(EntityManager& entity_manager, double dt) {
    get<S>()->update(dt);
  }

  void update(double dt);

 private:
  EntityManager& _entity_manager;
};
}  // namespace uecs