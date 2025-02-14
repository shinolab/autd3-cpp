#pragma once

#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {
struct Clear final : Datagram, IntoDatagramTuple<Clear> {
  AUTD3_API Clear() = default;  // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const override { return native_methods::AUTDDatagramClear(); }
};
}  // namespace autd3::driver
