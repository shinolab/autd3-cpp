#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

class DcSysTime final {
 public:
  static DcSysTime now() {
    return DcSysTime{
        0  // TODO
    };
  }

  [[nodiscard]] constexpr uint64_t sys_time() const { return _dc_sys_time; }

 private:
  explicit constexpr DcSysTime(const uint64_t sys_time) : _dc_sys_time(sys_time) {}

  uint64_t _dc_sys_time;
};

}  // namespace autd3::driver
