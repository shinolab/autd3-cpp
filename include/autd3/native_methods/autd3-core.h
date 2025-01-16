#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

/// GPIO input pin.
enum class GPIOIn : uint8_t {
  /// Input 0
  I0 = 0,
  /// Input 1
  I1 = 1,
  /// Input 2
  I2 = 2,
  /// Input 3
  I3 = 3,
};

/// GPIO output pin.
enum class GPIOOut : uint8_t {
  /// Output 0
  O0 = 0,
  /// Output 1
  O1 = 1,
  /// Output 2
  O2 = 2,
  /// Output 3
  O3 = 3,
};

/// Segment of the FPGA memory
enum class Segment : uint8_t {
  /// Segment 0
  S0 = 0,
  /// Segment 1
  S1 = 1,
};

/// The phase of the ultrasound.
struct Phase {
  /// The value of the phase.
  uint8_t value;
};
/// A phase of zero.
constexpr const Phase Phase_ZERO = Phase{ /* .value = */ 0 };
/// A phase of π.
constexpr const Phase Phase_PI = Phase{ /* .value = */ 128 };

/// The intensity of the ultrasound.
struct EmitIntensity {
  /// The value of the intensity.
  uint8_t value;
};
/// Maximum intensity.
constexpr const EmitIntensity EmitIntensity_MAX = EmitIntensity{ /* .value = */ 255 };
/// Minimum intensity.
constexpr const EmitIntensity EmitIntensity_MIN = EmitIntensity{ /* .value = */ 0 };

/// A container for the phase and intensity of the ultrasound.
struct Drive {
  /// The phase of the ultrasound.
  Phase phase;
  /// The intensity of the ultrasound.
  EmitIntensity intensity;
};
/// A [`Drive`] with a phase of [`Phase::ZERO`] and an intensity of [`EmitIntensity::MIN`].
constexpr const Drive Drive_NULL = Drive{ /* .phase = */ Phase_ZERO, /* .intensity = */ EmitIntensity_MIN };

/// The behavior of the loop.
struct LoopBehavior {
  uint16_t rep;
};

/// The configuration for sampling.
struct SamplingConfig {
  /// The division number of the sampling frequency.
  ///
  /// The sampling frequency is [`ultrasound_freq`] / `division`.
  uint16_t division;
};
/// A [`SamplingConfig`] of 40kHz.
constexpr const SamplingConfig SamplingConfig_FREQ_40K = SamplingConfig{ /* .division = */ 1 };
/// A [`SamplingConfig`] of 4kHz.
constexpr const SamplingConfig SamplingConfig_FREQ_4K = SamplingConfig{ /* .division = */ 10 };
/// A [`SamplingConfig`] of the minimum frequency.
constexpr const SamplingConfig SamplingConfig_FREQ_MIN = SamplingConfig{ /* .division = */ UINT16_MAX };

/// The system time of the Distributed Clock
///
/// The system time is the time expressed in 1ns units with 2000-01-01 0:00:00 UTC as the reference.
/// It is expressed as a 64-bit unsigned integer and can represent about 584 years of time.
/// See [EtherCAT Distributed Clock](https://infosys.beckhoff.com/english.php?content=../content/1033/ethercatsystem/2469118347.html) for more information.
struct DcSysTime {
  uint64_t dc_sys_time;
};
/// The zero point of the DcSysTime (2000-01-01 0:00:00 UTC)
constexpr const DcSysTime DcSysTime_ZERO = DcSysTime{ /* .dc_sys_time = */ 0 };

} // namespace autd3::native_methods
