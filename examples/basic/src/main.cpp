#include <bitset>
#include <iomanip>
#include <iostream>

#include <uecs/uecs.hpp>

class TestComponent : public uecs::Component {
 public:
  TestComponent(int i) : _i(i){};
  ~TestComponent() { std::cout << _i << " destroyed" << std::endl; }
  int _i;
};

class TestSystem : public uecs::System {
 public:
  TestSystem() = default;
  virtual void update(double dt) override { std::cout << dt << std::endl; }
};

int main() {
  std::cout << std::boolalpha;

  uecs::EntityManager em;
  uecs::ComponentManager cm;
  uecs::SystemManager sm(em, cm);
  uecs::Entity& e1 = em.create();
  uecs::Entity& e2 = em.create();
  int i = 0;

  std::cout << cm.has<TestComponent>(e1) << " " << cm.has<TestComponent>(e2)
            << std::endl;
  cm.create<TestComponent>(e1, i++);
  std::cout << cm.has<TestComponent>(e1) << " " << cm.has<TestComponent>(e2)
            << std::endl;
  cm.transfer<TestComponent>(e2, e1);
  std::cout << cm.has<TestComponent>(e1) << " " << cm.has<TestComponent>(e2)
            << std::endl;
  cm.create<TestComponent>(e1, i++);
  std::cout << cm.has<TestComponent>(e1) << " " << cm.has<TestComponent>(e2)
            << std::endl;
  auto p = cm.get<TestComponent>(e2);
  cm.remove<TestComponent>(e2);
  std::cout << cm.has<TestComponent>(e1) << " " << cm.has<TestComponent>(e2)
            << std::endl;
  cm.bind(e2, p);
  std::cout << cm.has<TestComponent>(e1) << " " << cm.has<TestComponent>(e2)
            << std::endl;
  cm.get<TestComponent>(e1)->_i = 4;

  return 0;
}