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

}  // namespace autd3::native_methods
