#include <uecs/entity_manager.hpp>
using namespace uecs;

id_type EntityManager::reserve_id() {
  id_type id;
  if (!_unused_ids.empty()) {
    auto b = _unused_ids.begin();
    id = *b;
    _unused_ids.erase(b);
  } else {
    id = _next_id++;
  }
  return id;
}

void EntityManager::release_id(id_type id) {
  _unused_ids.insert(id >> (sizeof(id_type) * 8 / 2));
void EntityManager::release_id(id_type id) { _unused_ids.insert(id); }
}