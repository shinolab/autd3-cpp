#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct ControlPoint {
  Vector3 point;
  Phase offset;

  explicit ControlPoint(Vector3 point, Phase offset) : point(std::move(point)), offset(offset) {}
  explicit ControlPoint(Vector3 point) : ControlPoint(std::move(point), Phase(0)) {}

  ControlPoint() : ControlPoint(Vector3::Zero()) {}
};

template <uint8_t N>
struct ControlPoints {
  std::array<ControlPoint, N> points;
  EmitIntensity intensity;

  explicit ControlPoints(std::array<ControlPoint, N> points, EmitIntensity intensity) : points(std::move(points)), intensity(intensity) {}
  explicit ControlPoints(std::array<ControlPoint, N> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(std::array<Vector3, N> points, EmitIntensity intensity) : intensity(intensity) {
    for (size_t i = 0; i < N; i++) this->points[i] = ControlPoint(points[i]);
  }
  explicit ControlPoints(std::array<Vector3, N> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
};

struct STMSamplingModeFreq {
  Freq<float> freq;

  [[nodiscard]] native_methods::FociSTMPtr focus_ptr(const std::vector<float>& points, const std::vector<uint8_t>& offsets,
                                                     const std::vector<uint8_t>& intensities, const uint8_t n) const {
    return validate(native_methods::AUTDSTMFociFromFreq(freq.hz(), points.data(), offsets.data(), intensities.data(),
                                                        static_cast<uint16_t>(intensities.size()), n));
  }
  [[nodiscard]] native_methods::GainSTMPtr gain_ptr(const std::vector<native_methods::GainPtr>& gains) const {
    return validate(AUTDSTMGainFromFreq(freq.hz(), gains.data(), static_cast<uint16_t>(gains.size())));
  }
};

struct STMSamplingModeFreqNearest {
  Freq<float> freq;

  [[nodiscard]] native_methods::FociSTMPtr focus_ptr(const std::vector<float>& points, const std::vector<uint8_t>& offsets,
                                                     const std::vector<uint8_t>& intensities, const uint8_t n) const {
    return validate(native_methods::AUTDSTMFociFromFreqNearest(freq.hz(), points.data(), offsets.data(), intensities.data(),
                                                               static_cast<uint16_t>(intensities.size()), n));
  }
  [[nodiscard]] native_methods::GainSTMPtr gain_ptr(const std::vector<native_methods::GainPtr>& gains) const {
    return validate(AUTDSTMGainFromFreqNearest(freq.hz(), gains.data(), static_cast<uint16_t>(gains.size())));
  }
};

struct STMSamplingModeSamplingConfig {
  native_methods::SamplingConfigWrap config;

  [[nodiscard]] native_methods::FociSTMPtr focus_ptr(const std::vector<float>& points, const std::vector<uint8_t>& offsets,
                                                     const std::vector<uint8_t>& intensities, const uint8_t n) const {
    return AUTDSTMFociFromSamplingConfig(config, points.data(), offsets.data(), intensities.data(), static_cast<uint16_t>(intensities.size()), n);
  }
  [[nodiscard]] native_methods::GainSTMPtr gain_ptr(const std::vector<native_methods::GainPtr>& gains) const {
    return AUTDSTMGainFromSamplingConfig(config, gains.data(), static_cast<uint16_t>(gains.size()));
  }
};

}  // namespace autd3::driver