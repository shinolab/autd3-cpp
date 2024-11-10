#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

enum class GPIOIn : uint8_t {
  I0 = 0,
  I1 = 1,
  I2 = 2,
  I3 = 3,
};

enum class GPIOOut : uint8_t {
  O0 = 0,
  O1 = 1,
  O2 = 2,
  O3 = 3,
};

enum class GainSTMMode : uint8_t {
  PhaseIntensityFull = 0,
  PhaseFull = 1,
  PhaseHalf = 2,
};

enum class Segment : uint8_t {
  S0 = 0,
  S1 = 1,
};

enum class SilencerTarget : uint8_t {
  Intensity = 0,
  PulseWidth = 1,
};

enum class SyncMode : uint8_t {
  DC = 0,
  FreeRun = 1,
};

struct Phase {
  uint8_t value;
};
constexpr const Phase Phase_ZERO = Phase{ /* .value = */ 0 };
constexpr const Phase Phase_PI = Phase{ /* .value = */ 128 };

struct EmitIntensity {
  uint8_t value;
};
constexpr const EmitIntensity EmitIntensity_MAX = EmitIntensity{ /* .value = */ 255 };
constexpr const EmitIntensity EmitIntensity_MIN = EmitIntensity{ /* .value = */ 0 };

struct Drive {
  Phase phase;
  EmitIntensity intensity;
};
constexpr const Drive Drive_NULL = Drive{ /* .phase = */ Phase_ZERO, /* .intensity = */ EmitIntensity_MIN };

struct LoopBehavior {
  uint16_t rep;
};

struct SamplingConfig {
  uint16_t division;
};
constexpr const SamplingConfig SamplingConfig_FREQ_40K = SamplingConfig{ /* .division = */ 1 };
constexpr const SamplingConfig SamplingConfig_FREQ_4K = SamplingConfig{ /* .division = */ 10 };
constexpr const SamplingConfig SamplingConfig_FREQ_MIN = SamplingConfig{ /* .division = */ UINT16_MAX };

struct DcSysTime {
  uint64_t dc_sys_time;
};
constexpr const DcSysTime DcSysTime_ZERO = DcSysTime{ /* .dc_sys_time = */ 0 };

} // namespace autd3::native_methods
