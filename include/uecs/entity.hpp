#pragma once

#include <bitset>
#include <memory>
#include <type_traits>
#include <unordered_map>

#include <uecs/component.hpp>
#include <uecs/config.hpp>
#include <uecs/type.hpp>
#include <uecs/unique_container.hpp>

namespace uecs {
class Entity : public UniqueContainer<Component, MAX_ENTITY_COMPONENTS> {
 public:
  Entity() : _id(0) {}
  const id_type id() const;

 private:
  id_type _id;
};
}  // namespace uecs