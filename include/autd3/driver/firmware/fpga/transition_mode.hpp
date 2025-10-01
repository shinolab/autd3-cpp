#pragma once

#include "autd3/driver/ethercat/dc_sys_time.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

class TransitionMode {
 public:
  virtual ~TransitionMode() = default;
  virtual native_methods::TransitionModeWrap inner() const = 0;
};

class InfiniteTransitionMode : TransitionMode {
 public:
  virtual ~InfiniteTransitionMode() = default;
};

class FiniteTransitionMode : TransitionMode {
 public:
  virtual ~FiniteTransitionMode() = default;
};

template <class T>
concept transition_mode_v = std::is_base_of_v<TransitionMode, std::remove_reference_t<T>>;

template <class T>
concept infinite_transition_mode_v = std::is_base_of_v<InfiniteTransitionMode, std::remove_reference_t<T>>;

template <class T>
concept finite_transition_mode_v = std::is_base_of_v<FiniteTransitionMode, std::remove_reference_t<T>>;

namespace transition_mode {

struct Immediate final : InfiniteTransitionMode {
  AUTD3_API Immediate() = default;
  ~Immediate() override = default;                       // LCOV_EXCL_LINE
  Immediate(const Immediate& v) noexcept = default;      // LCOV_EXCL_LINE
  Immediate& operator=(const Immediate& obj) = default;  // LCOV_EXCL_LINE
  Immediate(Immediate&& obj) = default;                  // LCOV_EXCL_LINE
  Immediate& operator=(Immediate&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API native_methods::TransitionModeWrap inner() const override { return native_methods::AUTDTransitionModeImmediate(); }
};

struct Ext final : InfiniteTransitionMode {
  AUTD3_API Ext() = default;
  ~Ext() override = default;                 // LCOV_EXCL_LINE
  Ext(const Ext& v) noexcept = default;      // LCOV_EXCL_LINE
  Ext& operator=(const Ext& obj) = default;  // LCOV_EXCL_LINE
  Ext(Ext&& obj) = default;                  // LCOV_EXCL_LINE
  Ext& operator=(Ext&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API native_methods::TransitionModeWrap inner() const override { return native_methods::AUTDTransitionModeExt(); }
};

struct SyncIdx final : FiniteTransitionMode {
  AUTD3_API SyncIdx() = default;
  ~SyncIdx() override = default;                     // LCOV_EXCL_LINE
  SyncIdx(const SyncIdx& v) noexcept = default;      // LCOV_EXCL_LINE
  SyncIdx& operator=(const SyncIdx& obj) = default;  // LCOV_EXCL_LINE
  SyncIdx(SyncIdx&& obj) = default;                  // LCOV_EXCL_LINE
  SyncIdx& operator=(SyncIdx&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API native_methods::TransitionModeWrap inner() const override { return native_methods::AUTDTransitionModeSyncIdx(); }
};

struct SysTime final : FiniteTransitionMode {
  AUTD3_API SysTime(const DcSysTime sys_time) : _sys_time(sys_time) {}
  ~SysTime() override = default;                     // LCOV_EXCL_LINE
  SysTime(const SysTime& v) noexcept = default;      // LCOV_EXCL_LINE
  SysTime& operator=(const SysTime& obj) = default;  // LCOV_EXCL_LINE
  SysTime(SysTime&& obj) = default;                  // LCOV_EXCL_LINE
  SysTime& operator=(SysTime&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API native_methods::TransitionModeWrap inner() const override { return native_methods::AUTDTransitionModeSysTime(_sys_time); }

 private:
  DcSysTime _sys_time;
};

struct GPIO final : FiniteTransitionMode {
  AUTD3_API explicit GPIO(const native_methods::GPIOIn gpio) : _gpio(gpio) {}
  ~GPIO() override = default;                  // LCOV_EXCL_LINE
  GPIO(const GPIO& v) noexcept = default;      // LCOV_EXCL_LINE
  GPIO& operator=(const GPIO& obj) = default;  // LCOV_EXCL_LINE
  GPIO(GPIO&& obj) = default;                  // LCOV_EXCL_LINE
  GPIO& operator=(GPIO&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API native_methods::TransitionModeWrap inner() const override { return native_methods::AUTDTransitionModeGPIO(_gpio); }

 private:
  native_methods::GPIOIn _gpio;
};

struct Later final : InfiniteTransitionMode, FiniteTransitionMode {
  AUTD3_API Later() = default;
  ~Later() override = default;                   // LCOV_EXCL_LINE
  Later(const Later& v) noexcept = default;      // LCOV_EXCL_LINE
  Later& operator=(const Later& obj) = default;  // LCOV_EXCL_LINE
  Later(Later&& obj) = default;                  // LCOV_EXCL_LINE
  Later& operator=(Later&& obj) = default;       // LCOV_EXCL_LINE

  AUTD3_API native_methods::TransitionModeWrap inner() const override { return native_methods::AUTDTransitionModeLater(); }
};
}  // namespace transition_mode

}  // namespace autd3::driver
