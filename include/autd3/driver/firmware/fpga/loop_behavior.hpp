#pragma once

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct LoopBehavior final {
  static const LoopBehavior Infinite;
  AUTD3_API [[nodiscard]] static LoopBehavior Finite(const uint16_t v) {
    if (v == 0) throw AUTDException("Value must be greater than 0");
    return LoopBehavior(native_methods::AUTDLoopBehaviorFinite(v));
  }

  AUTD3_API static const LoopBehavior ONCE;

  operator native_methods::LoopBehavior() const { return _inner; }

 private:
  explicit LoopBehavior(const native_methods::LoopBehavior rep) : _inner(rep) {}

  native_methods::LoopBehavior _inner;
};

const LoopBehavior LoopBehavior::Infinite = LoopBehavior(native_methods::AUTDLoopBehaviorInfinite());
const LoopBehavior LoopBehavior::ONCE = Finite(1);

}  // namespace autd3::driver
