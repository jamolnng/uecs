#pragma once

namespace uecs {
class System {
 public:
  System() = default;
  virtual ~System() = default;
  virtual void update(double dt) = 0;

 private:
};
}  // namespace uecs