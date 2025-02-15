#pragma once

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct Datagram {
  Datagram() = default;
  virtual ~Datagram() = default;                       // LCOV_EXCL_LINE
  Datagram(const Datagram& v) noexcept = default;      // LCOV_EXCL_LINE
  Datagram& operator=(const Datagram& obj) = default;  // LCOV_EXCL_LINE
  Datagram(Datagram&& obj) = default;                  // LCOV_EXCL_LINE
  Datagram& operator=(Datagram&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const = 0;
};

template <class D>
concept datagram = std::is_base_of_v<Datagram, std::remove_reference_t<D>>;

}  // namespace autd3::driver
