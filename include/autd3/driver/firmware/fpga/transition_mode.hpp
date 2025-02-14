#pragma once

#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct TransitionMode {
  AUTD3_API static TransitionMode SyncIdx() noexcept { return TransitionMode(native_methods::AUTDTransitionModeSyncIdx()); }
  AUTD3_API [[nodiscard]] static TransitionMode SysTime(const DcSysTime sys_time) {
    return TransitionMode(native_methods::AUTDTransitionModeSysTime(sys_time));
  }
  AUTD3_API [[nodiscard]] static TransitionMode GPIO(const native_methods::GPIOIn gpio) { return TransitionMode(AUTDTransitionModeGPIO(gpio)); }
  AUTD3_API static TransitionMode Ext() noexcept { return TransitionMode(native_methods::AUTDTransitionModeExt()); }
  AUTD3_API static TransitionMode Immediate() noexcept { return TransitionMode(native_methods::AUTDTransitionModeImmediate()); }

  operator native_methods::TransitionModeWrap() const { return _inner; }

 private:
  explicit TransitionMode(const native_methods::TransitionModeWrap inner) : _inner(inner) {}

  native_methods::TransitionModeWrap _inner;
};

}  // namespace autd3::driver
