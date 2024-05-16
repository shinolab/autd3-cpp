#pragma once

#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class TransitionMode {
 public:
  static inline const native_methods::TransitionModeWrap SyncIdx = native_methods::AUTDTransitionModeSyncIdx();
  static inline native_methods::TransitionModeWrap SysTime(const DcSysTime sys_time) {
    return native_methods::AUTDTransitionModeSysTime(sys_time.sys_time());
  };
  static inline native_methods::TransitionModeWrap GPIO(const native_methods::GPIOIn gpio) { return native_methods::AUTDTransitionModeGPIO(gpio); };
  static inline const native_methods::TransitionModeWrap Ext = native_methods::AUTDTransitionModeExt();
  static inline const native_methods::TransitionModeWrap Immediate = native_methods::AUTDTransitionModeImmediate();
};

}  // namespace autd3::driver
