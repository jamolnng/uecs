#include <bitset>
#include <iomanip>
#include <iostream>

#include <uecs/uecs.hpp>

class TestComponent : public uecs::Component {
 public:
  TestComponent(int i) : _i(i){};
  int _i;
};

class TestComponent1 : public uecs::Component {
 public:
  TestComponent1(int i) : _i(i){};
  int _i;
};

class TestComponent2 : public uecs::Component {
 public:
  TestComponent2(int i) : _i(i){};
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

  void receive(const uecs::EntityManager::EntityCreatedEvent& e) {
    std::cout << "Entity created. ID: " << e.entity.id() << std::endl;
  }
  void receive(const uecs::EntityManager::EntityDestroyedEvent& e) {
    std::cout << "Entity destroyed. ID: " << e.entity.id() << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentCreatedEvent<TestComponent>& c) {
    std::cout << "TestComponent created. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentCreatedEvent<TestComponent1>& c) {
    std::cout << "TestComponent1 created. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentRemovedEvent<TestComponent>& c) {
    std::cout << "TestComponent removed. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentRemovedEvent<TestComponent1>& c) {
    std::cout << "TestComponent1 removed. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentSwappedEvent<TestComponent>& c) {
    std::cout << "TestComponent swapped to Entity ID: " << c.entity.id()
              << " from " << c.from.id() << " ci:" << c.component->_i
              << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentSwappedEvent<TestComponent1>& c) {
    std::cout << "TestComponent1 swapped to Entity ID: " << c.entity.id()
              << " from " << c.from.id() << " ci:" << c.component->_i
              << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentBoundEvent<TestComponent>& c) {
    std::cout << "TestComponent bound. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentBoundEvent<TestComponent1>& c) {
    std::cout << "TestComponent1 bound. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentDestroyedEvent<TestComponent>& c) {
    std::cout << "TestComponent destroyed. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
  void receive(
      const uecs::ComponentManager::ComponentDestroyedEvent<TestComponent1>&
          c) {
    std::cout << "TestComponent1 destroyed. Entity ID: " << c.entity.id()
              << " ci:" << c.component->_i << std::endl;
  }
};

using ASDF = TestEvent;

int main() {
  std::cout << std::boolalpha;

  uecs::EventManager evm;
  uecs::ComponentManager cm(evm);
  uecs::EntityManager em(cm, evm);
  uecs::SystemManager sm(em, cm, evm);

  TestReceiver tr;
  evm.subscribe<uecs::EntityManager::EntityCreatedEvent>(tr);
  evm.subscribe<uecs::EntityManager::EntityDestroyedEvent>(tr);
  evm.subscribe<uecs::ComponentManager::ComponentCreatedEvent<TestComponent>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentCreatedEvent<TestComponent1>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentRemovedEvent<TestComponent>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentRemovedEvent<TestComponent1>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentSwappedEvent<TestComponent>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentSwappedEvent<TestComponent1>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentBoundEvent<TestComponent>>(tr);
  evm.subscribe<uecs::ComponentManager::ComponentBoundEvent<TestComponent1>>(
      tr);
  evm.subscribe<uecs::ComponentManager::ComponentDestroyedEvent<TestComponent>>(
      tr);
  evm.subscribe<
      uecs::ComponentManager::ComponentDestroyedEvent<TestComponent1>>(tr);

  uecs::Entity& e1 = em.create();
  uecs::Entity& e2 = em.create();
  uecs::Entity& e3 = em.create();
  e3.destroy();

  int i = 0;

  cm.create<TestComponent>(e1, i++);
  cm.swap<TestComponent>(e2, e1);
  cm.create<TestComponent>(e1, i++);
  auto p = cm.get<TestComponent>(e2);
  cm.remove<TestComponent>(e2);
  cm.bind(e2, p);
  p.reset();
  cm.get<TestComponent>(e1)->_i = 4;

  cm.create<TestComponent1>(e2, i++);

  std::cout << "Entities:" << std::endl;
  for (uecs::Entity& e : em) {
    std::cout << "id: " << e.id()
              << " mask: " << cm.component_mask(e).to_string() << std::endl;
  }
  std::cout
      << "Entities with TestComponent and TestComponent1 three different ways:"
      << std::endl;
  for (auto& e : em.contains_component_view<TestComponent, TestComponent1>()) {
    std::cout << "Entity ID: " << e.id() << " ci "
              << cm.get<TestComponent>(e)->_i << " c1i "
              << cm.get<TestComponent1>(e)->_i << std::endl;
  }

  em.contains_for_each<TestComponent, TestComponent1>(
      [&](uecs::Entity& e, TestComponent& t, TestComponent1& t1) {
        std::cout << "Entity ID: " << e.id() << " ci " << t._i << " c1i "
                  << t1._i << std::endl;
      });

  for (uecs::Entity& e :
       em.component_view<uecs::EntityManager::ContainsComponentsTest,
                         TestComponent, TestComponent1>()) {
    std::cout << "Entity ID: " << e.id() << " ci "
              << cm.get<TestComponent>(e)->_i << " c1i "
              << cm.get<TestComponent1>(e)->_i << std::endl;
  }

  e1.destroy();
  e2.destroy();

  /*evm.subscribe<TestEvent>(tr);
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
  evm.emit<TestEvent1>(t1);*/

  return 0;
}