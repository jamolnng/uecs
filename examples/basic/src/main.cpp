#include <bitset>
#include <iomanip>
#include <iostream>

#include <uecs/uecs.hpp>

class TestComponent : public uecs::Component {
 public:
  TestComponent(int i) : _i(i){};
  ~TestComponent() { std::cout << "deleted" << std::endl; }
  int _i;
};

int main() {
  std::cout << std::boolalpha;

  uecs::Entity e;
  std::cout << e.has<TestComponent>() << std::endl;
  auto tc = e.add<TestComponent>(99);
  std::cout << e.has<TestComponent>() << std::endl;
  std::cout << tc->_i << " " << e.get<TestComponent>()->_i << std::endl;
  e.remove<TestComponent>();
  tc.reset(static_cast<TestComponent*>(nullptr));
  std::cout << e.has<TestComponent>() << std::endl;

  return 0;
}