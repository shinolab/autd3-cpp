#pragma once

#include <chrono>

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class DcSysTime final {
 public:
  AUTD3_API static DcSysTime now() { return DcSysTime{native_methods::AUTDDcSysTimeNow()}; }

  AUTD3_API [[nodiscard]] constexpr uint64_t sys_time() const { return _dc_sys_time; }

  template <typename Rep, typename Period>
  AUTD3_API DcSysTime operator+(const std::chrono::duration<Rep, Period> &rhs) {
    return DcSysTime{_dc_sys_time + std::chrono::duration_cast<std::chrono::nanoseconds>(rhs).count()};
  }

  template <typename Rep, typename Period>
  AUTD3_API DcSysTime operator-(const std::chrono::duration<Rep, Period> &rhs) {
    return DcSysTime{_dc_sys_time - std::chrono::duration_cast<std::chrono::nanoseconds>(rhs).count()};
  }

 private:
  AUTD3_API explicit constexpr DcSysTime(const uint64_t sys_time) : _dc_sys_time(sys_time) {}

  uint64_t _dc_sys_time;
};

}  // namespace autd3::driver
