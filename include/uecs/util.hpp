#pragma once

namespace uecs {
class NonCopyable {
 protected:
  NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
};
template <typename T>
struct identity {
  using type = T;
};
}  // namespace uecs