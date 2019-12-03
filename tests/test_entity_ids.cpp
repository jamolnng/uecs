#include <tests.hpp>

#include <uecs/uecs.hpp>

using uecs::EntityManager;
using uecs::UECS;

REGISTER_TEST(test_entity_ids) {
  UECS uecs;
  EntityManager& em = uecs._entities;
  int i;
  // Test IDs are created in ascending order starting at 0
  for (i = 0; i < 10; ++i) {
    TASSERTD(em.create().id(), i)
  }

  // Test in order
  for (--i; i > 5; --i) {
    em.destroy(em.get(i));
  }
  TASSERTD(em.create().id(), i + 1)

  // Test out of order
  em.destroy(em.get(1));
  TASSERTD(em.create().id(), 1)
  TASSERTD(em.create().id(), i + 2)

  TPASS()
}