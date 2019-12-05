#pragma once

#include <iostream>
#include <type_traits>
#include <typeinfo>

#include <uecs/config.hpp>

namespace uecs {
namespace tests {
namespace detail {
extern const char* __test_name;
bool __tassert(bool condition, const char* c_str, size_t line);
bool __tassert(bool condition, const char* c_str, size_t line) {
  if (!condition) {
    std::cout << "failed" << std::endl;
    std::cerr << "  Line " << line << ": " << c_str << std::endl;
  }
  return condition;
}

template <typename LHS, typename RHS>
typename std::enable_if<std::is_class<LHS>::value && std::is_class<RHS>::value,
                        bool>::type
__tassertd(LHS l, RHS r, const char* l_str, const char* r_str, size_t line) {
  if (!(l == r)) {
    std::cout << "failed" << std::endl;
    std::cerr << "  Line " << line << ": " << l_str << " == " << r_str << " ("
              << typeid(l).name() << " == " << typeid(r).name() << ")"
              << std::endl;
    return false;
  }
  return true;
}

template <typename LHS, typename RHS>
typename std::enable_if<std::is_class<LHS>::value && !std::is_class<RHS>::value,
                        bool>::type
__tassertd(LHS l, RHS r, const char* l_str, const char* r_str, size_t line) {
  if (!(l == r)) {
    std::cout << "failed" << std::endl;
    std::cerr << "  Line " << line << ": " << l_str << " == " << r_str << " ("
              << l_str << " == " << r << ")" << std::endl;
    return false;
  }
  return true;
}

template <typename LHS, typename RHS>
typename std::enable_if<!std::is_class<LHS>::value && std::is_class<RHS>::value,
                        bool>::type
__tassertd(LHS l, RHS r, const char* l_str, const char* r_str, size_t line) {
  if (!(l == r)) {
    std::cout << "failed" << std::endl;
    std::cerr << "  Line " << line << ": " << l_str << " == " << r_str << " ("
              << l << " == " << r_str << ")" << std::endl;
    return false;
  }
  return true;
}

template <typename LHS, typename RHS>
typename std::enable_if<
    !std::is_class<LHS>::value && !std::is_class<RHS>::value, bool>::type
__tassertd(LHS l, RHS r, const char* l_str, const char* r_str, size_t line) {
  if (!(l == r)) {
    std::cout << "failed" << std::endl;
    std::cerr << "  Line " << line << ": " << l_str << " == " << r_str << " ("
              << l << " == " << r << ")" << std::endl;
    return false;
  }
  return true;
}
}  // namespace detail
}  // namespace tests
}  // namespace uecs

#define REGISTER_TEST(name)                                  \
  const char* uecs::tests::detail::__test_name = #name;      \
  int name(int argc, char* argvp[]);                         \
  int main(int argc, char* argv[]) {                         \
    std::cout << "uecs v" << uecs::VERSION_STR << std::endl; \
    std::cout << #name << "... ";                            \
    if (name(argc, argv) == 0) {                             \
      std::cout << "passed" << std::endl;                    \
      return 0;                                              \
    } else {                                                 \
      return -1;                                             \
    }                                                        \
  }                                                          \
  int name(int argc, char* argvp[])

#define TASSERT(condition)                                                \
  if (!uecs::tests::detail::__tassert(condition, #condition, __LINE__)) { \
    return -1;                                                            \
  }

#define TASSERTD(lhs, rhs)                                            \
  if (!uecs::tests::detail::__tassertd<decltype(lhs), decltype(rhs)>( \
          lhs, rhs, #lhs, #rhs, __LINE__)) {                          \
    return -1;                                                        \
  }

#define TPASS() return 0;