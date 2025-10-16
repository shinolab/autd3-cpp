#pragma once

#include <string>

#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::link {

struct TwinCAT final {
  TwinCAT() = default;  // LCOV_EXCL_LINE

  [[nodiscard]] native_methods::LinkPtr resolve() const { return validate(native_methods::AUTDLinkTwinCAT()); }
};

}  // namespace autd3::link
