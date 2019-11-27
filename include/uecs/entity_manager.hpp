#pragma once

#include <set>

#include <uecs/entity.hpp>
#include <uecs/type.hpp>

namespace uecs {
class EntityManager : NonCopyable {
 public:
  EntityManager() = default;

 private:
  id_type _next_id{0};
  std::set<id_type> _unused_ids;

  id_type reserve_id();
  void release_id(id_type id);
};
}  // namespace uecs