#include <tests.hpp>

#include <uecs/uecs.hpp>

using uecs::Component;
using uecs::ComponentManager;
using uecs::Entity;
using uecs::UECS;

class TestComponent1 : public Component {};
class TestComponent2 : public Component {};
class TestComponent3 : public Component {};
class TestComponent4 : public Component {};

struct TestComponent2Test : public uecs::EntityManager::ComponentTestBase {
  static bool valid(const ComponentManager::ComponentMask& m1,
                    const ComponentManager::ComponentMask& m2) {
    return m2.test(uecs::TypeID<TestComponent2, Component>::value());
  };
};

REGISTER_TEST(test_entity_component_view) {
  UECS uecs;
  auto& em = uecs._entities;
  auto& cm = uecs._components;
  Entity& e1 = em.create();
  Entity& e2 = em.create();
  Entity& e3 = em.create();
  cm.create<TestComponent1>(e1);
  cm.create<TestComponent2>(e1);
  cm.create<TestComponent3>(e1);
  cm.create<TestComponent4>(e1);

  cm.create<TestComponent2>(e2);
  cm.create<TestComponent4>(e2);

  cm.create<TestComponent2>(e3);

  unsigned int i = 0u;
  for (auto& e : em.contains_component_view<TestComponent1, TestComponent2,
                                            TestComponent3, TestComponent4>()) {
    ++i;
  }
  TASSERTD(i, 1u)

  i = 0u;
  for (auto& e : em.contains_component_view<TestComponent2, TestComponent4>()) {
    ++i;
  }
  TASSERTD(i, 2u)

  i = 0u;
  for (auto& e : em.contains_component_view<TestComponent2>()) {
    ++i;
  }
  TASSERTD(i, 3u)

  i = 0u;
  for (auto& e : em.contains_component_view<TestComponent4>()) {
    ++i;
  }
  TASSERTD(i, 2u)

  i = 0u;
  for (auto& e : em.exact_component_view<TestComponent1, TestComponent2,
                                         TestComponent3, TestComponent4>()) {
    ++i;
  }
  TASSERTD(i, 1u)

  i = 0u;
  for (auto& e : em.exact_component_view<TestComponent2, TestComponent4>()) {
    ++i;
  }
  TASSERTD(i, 1u)

  i = 0u;
  for (auto& e : em.exact_component_view<TestComponent1, TestComponent3>()) {
    ++i;
  }
  TASSERTD(i, 0u)

  i = 0u;
  for (auto& e : em.exact_component_view<TestComponent2>()) {
    ++i;
  }
  TASSERTD(i, 1u)

  i = 0u;
  for (auto& e : em.component_view<TestComponent2Test, TestComponent2>()) {
    ++i;
  }
  TASSERTD(i, 3u)

  i = 0u;
  for (auto& e : em.component_view<TestComponent2Test, TestComponent2,
                                   TestComponent3>()) {
    ++i;
  }
  TASSERTD(i, 3u)

  TPASS()
}