#pragma once

#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct LoopBehavior final {
  AUTD3_API [[nodiscard]] static native_methods::LoopBehavior infinite() noexcept { return native_methods::AUTDLoopBehaviorInfinite(); }

  AUTD3_API [[nodiscard]] static native_methods::LoopBehavior finite(const uint32_t v) {
    if (v == 0) throw AUTDException("Value must be greater than 0");
    return native_methods::AUTDLoopBehaviorFinite(v);
  }

  AUTD3_API [[nodiscard]] static native_methods::LoopBehavior once() noexcept { return native_methods::AUTDLoopBehaviorOnce(); }
};

}  // namespace autd3::driver
