#include <bitset>
#include <iomanip>
#include <iostream>

#include <uecs/uecs.hpp>

class TestComponent : public uecs::Component {
 public:
  TestComponent(int i) : _i(i){};
  int _i;
};

class TestSystem : public uecs::System {
 public:
  TestSystem() = default;
  virtual void update(double dt) override { std::cout << dt << std::endl; }
};

int main() {
  std::cout << std::boolalpha;

  uecs::EntityManager manager;
  uecs::Entity& e = manager.create();
  std::cout << e.has<TestComponent>() << std::endl;
  auto tc = e.add<TestComponent>(99);
  std::cout << e.has<TestComponent>() << std::endl;
  std::cout << tc->_i << " " << e.get<TestComponent>()->_i << std::endl;
  e.remove<TestComponent>();
  tc.reset(static_cast<TestComponent*>(nullptr));
  std::cout << e.has<TestComponent>() << std::endl;
  manager.destroy(e.id());

  uecs::SystemManager system(manager);

  system.add<TestSystem>();

  system.update<TestSystem>(manager, 10.0);
  system.update(12);

  system.remove<TestSystem>();
  system.update(432);

  return 0;
}