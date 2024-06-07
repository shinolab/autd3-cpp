
#pragma once

#include <chrono>
#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class D>
class DatagramWithParallelThreshold {
 public:
  DatagramWithParallelThreshold(D datagram, const size_t threshold) : _datagram(std::move(datagram)), _threshold(threshold) {}
  AUTD3_API virtual ~DatagramWithParallelThreshold() = default;                                  // LCOV_EXCL_LINE
  DatagramWithParallelThreshold(const DatagramWithParallelThreshold& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramWithParallelThreshold& operator=(const DatagramWithParallelThreshold& obj) = default;  // LCOV_EXCL_LINE
  DatagramWithParallelThreshold(DatagramWithParallelThreshold&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramWithParallelThreshold& operator=(DatagramWithParallelThreshold&& obj) = default;       // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr ptr(const geometry::Geometry& g) const {
    const auto ptr = _datagram.ptr(g);
    return native_methods::AUTDDatagramWithParallelThreshold(ptr, static_cast<uint16_t>(_threshold));
  }

 private:
  D _datagram;
  size_t _threshold;
};

template <class D>
class IntoDatagramWithParallelThreshold {
 public:
  IntoDatagramWithParallelThreshold() = default;                                                         // LCOV_EXCL_LINE
  virtual ~IntoDatagramWithParallelThreshold() = default;                                                // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold(const IntoDatagramWithParallelThreshold& v) noexcept = default;      // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold& operator=(const IntoDatagramWithParallelThreshold& obj) = default;  // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold(IntoDatagramWithParallelThreshold&& obj) = default;                  // LCOV_EXCL_LINE
  IntoDatagramWithParallelThreshold& operator=(IntoDatagramWithParallelThreshold&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] DatagramWithParallelThreshold<D> with_ParallelThreshold(const size_t threshold) & {
    return DatagramWithParallelThreshold<D>(*static_cast<D*>(this), threshold);
  }
  AUTD3_API [[nodiscard]] DatagramWithParallelThreshold<D> with_ParallelThreshold(const size_t threshold) && {
    return DatagramWithParallelThreshold<D>(std::move(*static_cast<D*>(this)), threshold);
  }
};

}  // namespace autd3::driver
