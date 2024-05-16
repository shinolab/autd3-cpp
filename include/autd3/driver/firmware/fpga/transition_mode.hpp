#pragma once

#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class TransitionMode {
 public:
  static constexpr native_methods::TransitionModeWrap SyncIdx{0x00, 0};
  static constexpr native_methods::TransitionModeWrap SysTime(const DcSysTime sys_time) {
    return native_methods::TransitionModeWrap{0x01, sys_time.sys_time()};
  };
  static constexpr native_methods::TransitionModeWrap GPIO(const native_methods::GPIOIn gpio) {
    return native_methods::TransitionModeWrap{0x02, static_cast<uint64_t>(gpio)};
  };
  static constexpr native_methods::TransitionModeWrap Ext{0xF0, 0};
  static constexpr native_methods::TransitionModeWrap Immediate{0xFF, 0};
};

}  // namespace autd3::driver
