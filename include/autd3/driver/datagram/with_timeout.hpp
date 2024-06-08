#pragma once

#include <chrono>
#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class D>
class DatagramWithTimeout {
 public:
  template <typename Rep, typename Period>
  DatagramWithTimeout(D datagram, const std::chrono::duration<Rep, Period> timeout)
      : _datagram(std::move(datagram)), _timeout_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count()) {}
  AUTD3_API virtual ~DatagramWithTimeout() = default;                        // LCOV_EXCL_LINE
  DatagramWithTimeout(const DatagramWithTimeout& v) noexcept = default;      // LCOV_EXCL_LINE
  DatagramWithTimeout& operator=(const DatagramWithTimeout& obj) = default;  // LCOV_EXCL_LINE
  DatagramWithTimeout(DatagramWithTimeout&& obj) = default;                  // LCOV_EXCL_LINE
  DatagramWithTimeout& operator=(DatagramWithTimeout&& obj) = default;       // LCOV_EXCL_LINE
  AUTD3_API [[nodiscard]] virtual native_methods::DatagramPtr ptr(const geometry::Geometry& g) const {
    const auto ptr = _datagram.ptr(g);
    return native_methods::AUTDDatagramWithTimeout(ptr, _timeout_ns);
  }

 private:
  D _datagram;
  uint64_t _timeout_ns;
};

template <class D>
class IntoDatagramWithTimeout {
 public:
  IntoDatagramWithTimeout() = default;                                               // LCOV_EXCL_LINE
  virtual ~IntoDatagramWithTimeout() = default;                                      // LCOV_EXCL_LINE
  IntoDatagramWithTimeout(const IntoDatagramWithTimeout& v) noexcept = default;      // LCOV_EXCL_LINE
  IntoDatagramWithTimeout& operator=(const IntoDatagramWithTimeout& obj) = default;  // LCOV_EXCL_LINE
  IntoDatagramWithTimeout(IntoDatagramWithTimeout&& obj) = default;                  // LCOV_EXCL_LINE
  IntoDatagramWithTimeout& operator=(IntoDatagramWithTimeout&& obj) = default;       // LCOV_EXCL_LINE

  template <typename Rep, typename Period>
  AUTD3_API [[nodiscard]] DatagramWithTimeout<D> with_timeout(const std::chrono::duration<Rep, Period> timeout) & {  // LCOV_EXCL_LINE
    return DatagramWithTimeout<D>(*static_cast<D*>(this), timeout);                                                  // LCOV_EXCL_LINE
  }  // LCOV_EXCL_LINE
  template <typename Rep, typename Period>
  AUTD3_API [[nodiscard]] DatagramWithTimeout<D> with_timeout(const std::chrono::duration<Rep, Period> timeout) && {
    return DatagramWithTimeout<D>(std::move(*static_cast<D*>(this)), timeout);
  }
};

}  // namespace autd3::driver
