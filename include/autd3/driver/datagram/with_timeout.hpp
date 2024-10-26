#pragma once

#include <chrono>
#include <optional>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class D>
class DatagramWithTimeout final {
 public:
  DatagramWithTimeout(D datagram, const std::optional<std::chrono::nanoseconds> timeout)
      : _datagram(std::move(datagram)),
        _timeout_ns(timeout.has_value() ? static_cast<int64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout.value()).count()) : -1) {}
  ~DatagramWithTimeout() = default;
  DatagramWithTimeout(const DatagramWithTimeout& v) noexcept = default;
  DatagramWithTimeout& operator=(const DatagramWithTimeout& obj) = default;
  DatagramWithTimeout(DatagramWithTimeout&& obj) = default;
  DatagramWithTimeout& operator=(DatagramWithTimeout&& obj) = default;
  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& g) const {
    const auto ptr = _datagram.ptr(g);
    return native_methods::AUTDDatagramWithTimeout(ptr, _timeout_ns);
  }

 private:
  D _datagram;
  int64_t _timeout_ns;
};

template <class D>
class IntoDatagramWithTimeout {
 public:
  IntoDatagramWithTimeout() = default;
  virtual ~IntoDatagramWithTimeout() = default;
  IntoDatagramWithTimeout(const IntoDatagramWithTimeout& v) noexcept = default;
  IntoDatagramWithTimeout& operator=(const IntoDatagramWithTimeout& obj) = default;
  IntoDatagramWithTimeout(IntoDatagramWithTimeout&& obj) = default;
  IntoDatagramWithTimeout& operator=(IntoDatagramWithTimeout&& obj) = default;

  AUTD3_API [[nodiscard]] DatagramWithTimeout<D> with_timeout(const std::optional<std::chrono::nanoseconds> timeout) & {
    return DatagramWithTimeout<D>(*static_cast<D*>(this), timeout);
  }
  AUTD3_API [[nodiscard]] DatagramWithTimeout<D> with_timeout(const std::optional<std::chrono::nanoseconds> timeout) && {
    return DatagramWithTimeout<D>(std::move(*static_cast<D*>(this)), timeout);
  }
};

}  // namespace autd3::driver
