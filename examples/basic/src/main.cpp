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
  std::cout << e.has_component<TestComponent>() << std::endl;
  auto* tc = e.add_component<TestComponent>(99);
  std::cout << e.has_component<TestComponent>() << std::endl;
  std::cout << tc->_i << " " << e.get_component<TestComponent>()->_i
            << std::endl;
  e.remove_component<TestComponent>();
  std::cout << e.has_component<TestComponent>() << std::endl;

  uecs::EntityManager manager;
  std::cout << manager._next_id << std::endl;
  uecs::id_type id1 = manager.create_id(), id2 = manager.create_id();
  std::cout << std::bitset<sizeof(uecs::id_type) * 8>{id1}.to_string();
  std::cout << " " << manager._next_id << std::endl;
  std::cout << std::bitset<sizeof(uecs::id_type) * 8>{id2}.to_string() << " ";
  std::cout << manager._next_id << std::endl;
  std::cout
      << std::bitset<sizeof(uecs::id_type) * 8>{manager.create_id()}.to_string()
      << " ";
  std::cout << manager._next_id << std::endl;
  std::cout
      << std::bitset<sizeof(uecs::id_type) * 8>{manager.create_id()}.to_string()
      << " ";
  std::cout << manager._next_id << std::endl;
  uecs::id_type id3 = manager.create_id();
  std::cout << std::bitset<sizeof(uecs::id_type) * 8>{id3}.to_string() << "* ";
  std::cout << manager._next_id << std::endl;
  std::cout
      << std::bitset<sizeof(uecs::id_type) * 8>{manager.create_id()}.to_string()
      << " ";
  std::cout << manager._next_id << std::endl;
  std::cout << std::dec << manager._next_id << std::endl;
  manager.release_id(id1);
  std::cout << manager._unused_ids.size() << std::endl;
  std::cout
      << std::bitset<sizeof(uecs::id_type) * 8>{manager.create_id()}.to_string()
      << std::endl;
  manager.release_id(id3);
  std::cout << manager._unused_ids.size() << std::endl;
  std::cout
      << std::bitset<sizeof(uecs::id_type) * 8>{manager.create_id()}.to_string()
      << std::endl;

  return 0;
}