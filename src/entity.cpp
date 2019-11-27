#include <uecs/entity.hpp>

using namespace uecs;

const id_type Entity::id() const { return _id; }

const Entity::mask_type& Entity::component_mask() const { return _mask; }