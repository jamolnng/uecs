#pragma once

#include <queue>

#include <uecs/entity.hpp>
#include <uecs/type.hpp>

namespace uecs {
class EntityManager {
 public:
  EntityManager() = default;

  id_type _next_id{0};
  std::queue<id_type> _unused_ids;

  id_type create_id();

  void release_id(id_type id);
};
}  // namespace uecs