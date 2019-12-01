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

class TestComponent1 : public uecs::Component {
 public:
  TestComponent1(int i) : _i(i){};
  ~TestComponent1() { std::cout << _i << " destroyed 1" << std::endl; }
  int _i;
};

class TestSystem : public uecs::System {
 public:
  TestSystem() = default;
  virtual void update(double dt) override { std::cout << dt << std::endl; }
};

class TestEvent : public uecs::Event, public uecs::NonCopyable {
 public:
  TestEvent(std::string test) : _test(test){};
  std::string _test;
};

class TestEvent1 : public uecs::Event, public uecs::NonCopyable {
 public:
  TestEvent1(std::string test) : _test(test){};
  std::string _test;
};

class TestReceiver : public uecs::EventListener, public uecs::NonCopyable {
 public:
  void receive(const TestEvent& e) { std::cout << e._test << std::endl; }
  void receive(const TestEvent1& e) { std::cout << e._test << std::endl; }
};

int main() {
  std::cout << std::boolalpha;

  uecs::ComponentManager cm;
  uecs::EntityManager em(cm);
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

  cm.remove<TestComponent>(e2);
  cm.create<TestComponent1>(e2, i++);
  std::cout << em.component_mask(e2).to_string() << std::endl;
  cm.remove<TestComponent1>(e2);

  for (uecs::Entity& e : em) {
    std::cout << e.id() << std::endl;
  }

  uecs::EventManager evm;
  TestReceiver tr;
  evm.subscribe<TestEvent>(tr);
  evm.subscribe<TestEvent1>(tr);
  evm.emit<TestEvent>("test a");
  evm.emit<TestEvent1>("test b");
  evm.unsubscribe<TestEvent>(tr);
  evm.emit<TestEvent>("test c");
  evm.emit<TestEvent1>("test d");
  std::shared_ptr<TestEvent> t = std::make_shared<TestEvent>("test e");
  std::shared_ptr<TestEvent1> t1 = std::make_shared<TestEvent1>("test f");
  evm.emit<TestEvent>(*t);
  evm.emit<TestEvent1>(*t1);
  evm.emit<TestEvent>(t);
  evm.emit<TestEvent1>(t1);

  return 0;
}