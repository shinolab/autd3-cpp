#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

/// The parallel processing mode.
enum class ParallelMode : uint8_t {
  /// Automatically select the processing mode. If the number of devices is greater than the parallel threshold of the [`Datagram::option`], the parallel processing is used.
  Auto = 0,
  /// Force to use the parallel processing.
  On = 1,
  /// Force to use the serial processing.
  Off = 2,
};

/// The option of [`Square`].
struct SquareOption {
  /// The low value of the modulation. The default value is [`u8::MIN`].
  uint8_t low;
  /// The high value of the modulation. The default value is [`u8::MAX`].
  uint8_t high;
  /// The duty ratio of the modulation, that is the ratio of high value to the period. The default value is `0.5`.
  float duty;
  /// The sampling configuration of the modulation. The default value is [`SamplingConfig::DIV_10`].
  SamplingConfig sampling_config;
};

/// The option of [`Sine`].
struct SineOption {
  /// The intensity of the modulation. The default value is [`u8::MAX`].
  uint8_t intensity;
  /// The offset of the modulation. The default value is `0x80`.
  uint8_t offset;
  /// The phase of the modulation. The default value is `0 rad`.
  Angle phase;
  /// If `true`, the modulation value is clamped to the range of `u8`. If `false`, returns an error if the value is out of range. The default value is `false`.
  bool clamp;
  /// The sampling configuration of the modulation. The default value is [`SamplingConfig::DIV_10`].
  SamplingConfig sampling_config;
};

/// The option of [`Plane`].
struct PlaneOption {
  /// The intensity of the beam.
  EmitIntensity intensity;
  /// The phase offset of the beam.
  Phase phase_offset;
};

/// The option of [`Focus`].
struct FocusOption {
  /// The intensity of the beam.
  EmitIntensity intensity;
  /// The phase offset of the beam.
  Phase phase_offset;
};

/// The option of [`Bessel`].
struct BesselOption {
  /// The intensity of the beam.
  EmitIntensity intensity;
  /// The phase offset of the beam.
  Phase phase_offset;
};

} // namespace autd3::native_methods
