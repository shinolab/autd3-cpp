#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::link {

struct Nop {
  Nop() = default;

  AUTD3_API [[nodiscard]] static native_methods::LinkPtr resolve() { return native_methods::AUTDLinkNop(); }
};
}  // namespace autd3::link
