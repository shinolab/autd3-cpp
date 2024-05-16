#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class DcSysTime final {
 public:
  static DcSysTime now() { return DcSysTime{native_methods::AUTDDcSysTimeNow()}; }

  [[nodiscard]] constexpr uint64_t sys_time() const { return _dc_sys_time; }

  template <typename Rep, typename Period>
  DcSysTime operator+(const std::chrono::duration<Rep, Period> &rhs) {
    return DcSysTime{_dc_sys_time + std::chrono::duration_cast<std::chrono::nanoseconds>(rhs).count()};
  }

  template <typename Rep, typename Period>
  DcSysTime operator-(const std::chrono::duration<Rep, Period> &rhs) {
    return DcSysTime{_dc_sys_time - std::chrono::duration_cast<std::chrono::nanoseconds>(rhs).count()};
  }

 private:
  explicit constexpr DcSysTime(const uint64_t sys_time) : _dc_sys_time(sys_time) {}

  uint64_t _dc_sys_time;
};

}  // namespace autd3::driver
