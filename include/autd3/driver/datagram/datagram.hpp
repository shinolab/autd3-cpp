#pragma once

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class D>
concept datagram = requires(D d, const geometry::Geometry& g) {
  { d.ptr(g) } -> std::same_as<native_methods::DatagramPtr>;
};

class NullDatagram final {
 public:
  NullDatagram() = default;                                    // LCOV_EXCL_LINE
  ~NullDatagram() = default;                                   // LCOV_EXCL_LINE
  NullDatagram(const NullDatagram& v) noexcept = default;      // LCOV_EXCL_LINE
  NullDatagram& operator=(const NullDatagram& obj) = default;  // LCOV_EXCL_LINE
  NullDatagram(NullDatagram&& obj) = default;                  // LCOV_EXCL_LINE
  NullDatagram& operator=(NullDatagram&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] static native_methods::DatagramPtr ptr(const geometry::Geometry&) { return native_methods::DatagramPtr{nullptr}; }
};

}  // namespace autd3::driver
