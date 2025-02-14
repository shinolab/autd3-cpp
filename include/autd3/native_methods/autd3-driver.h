#pragma once

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>

namespace autd3::native_methods {

/// [`GainSTM`] transmission mode.
///
/// [`GainSTM`]: crate::datagram::GainSTM
enum class GainSTMMode : uint8_t {
  /// This mode uses both phase and intensity data.
  PhaseIntensityFull = 0,
  /// This mode uses only phase data.
  PhaseFull = 1,
  /// This mode uses only half-compressed phase data.
  PhaseHalf = 2,
};

/// Silencer target.
enum class SilencerTarget : uint8_t {
  /// Apply the silencer to the intensity (before [`PulseWidthEncoder`]).
  ///
  /// [`PulseWidthEncoder`]: crate::datagram::PulseWidthEncoder
  Intensity = 0,
  /// Apply the silencer to the pulse width (after [`PulseWidthEncoder`]).
  ///
  /// [`PulseWidthEncoder`]: crate::datagram::PulseWidthEncoder
  PulseWidth = 1,
};

/// The option for the [`GainSTM`].
struct GainSTMOption {
  /// The mode of the STM. The default is [`GainSTMMode::PhaseIntensityFull`].
  GainSTMMode mode;
};

/// To configure the silencer by the completion steps.
struct FixedCompletionSteps {
  /// The completion steps of the intensity change.
  ///
  /// The larger this value, the more the noise is suppressed.
  uint16_t intensity;
  /// The completion time of the phase change.
  ///
  /// The larger this value, the more the noise is suppressed.
  uint16_t phase;
  /// Whether the strict mode is enabled. The default is `true`.
  ///
  /// If the strict mode is enabled, an error is returned if the phase/intensity change of [`Modulation`], [`FociSTM`] or [`GainSTM`] cannot be completed within the time specified by the silencer.
  ///
  /// [`Modulation`]: autd3_core::modulation::Modulation
  /// [`FociSTM`]: crate::datagram::FociSTM
  /// [`GainSTM`]: crate::datagram::GainSTM
  bool strict_mode;
};

/// To configure the silencer by the update rate.
struct FixedUpdateRate {
  /// The update rate of the intensity change.
  ///
  /// The smaller this value, the more the noise is suppressed.
  uint16_t intensity;
  /// The update rate of the phase change.
  ///
  /// The smaller this value, the more the noise is suppressed.
  uint16_t phase;
};

} // namespace autd3::native_methods
