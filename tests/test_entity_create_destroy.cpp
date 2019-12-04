#include <tests.hpp>

#include <uecs/uecs.hpp>

using uecs::Entity;
using uecs::EntityManager;
using uecs::id_type;
using uecs::UECS;

REGISTER_TEST(test_entity_create_destroy) {
  UECS uecs;
  EntityManager& em = uecs._entities;

  TASSERTD(em.size(), 0)
  TASSERT(em.empty())
  Entity& e1 = em.create();
  TASSERTD(em.size(), 1)
  Entity& e2 = em.create();
  TASSERTD(em.size(), 2)
  em.destroy(e1);
  TASSERTD(em.size(), 1)
  em.destroy(e2);
  TASSERTD(em.size(), 0)
  TASSERT(em.empty())

  TPASS()
}