#pragma once

#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

struct ControlPoint {
  template <uint8_t N>
  friend struct ControlPoints;

  AUTD3_DEF_PROP(Vector3, point)
  AUTD3_DEF_PARAM_PHASE(ControlPoint, offset)

  explicit ControlPoint(Vector3 point) : ControlPoint(std::move(point), Phase(0)) {}

 private:
  ControlPoint() : ControlPoint(Vector3::Zero()) {}
  explicit ControlPoint(Vector3 point, Phase offset) : _point(std::move(point)), _offset(offset) {}
};

template <uint8_t N>
struct ControlPoints {
 private:
  std::array<ControlPoint, N> _points = {};

 public:
  const std::array<ControlPoint, N>& points() const { return _points; }

  AUTD3_DEF_PARAM_INTENSITY(ControlPoints<N>, intensity)

  explicit ControlPoints(std::array<ControlPoint, N> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(std::array<Vector3, N> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}

 private:
  explicit ControlPoints(std::array<ControlPoint, N> points, EmitIntensity intensity) : _points(std::move(points)), _intensity(intensity) {}
  explicit ControlPoints(std::array<Vector3, N> points, EmitIntensity intensity) : _intensity(intensity) {
    for (size_t i = 0; i < N; i++) this->_points[i] = ControlPoint(points[i]);
  }
};

template <>
struct ControlPoints<1> {
 private:
  std::array<ControlPoint, 1> _points = {};

 public:
  const std::array<ControlPoint, 1>& points() const { return _points; }

  AUTD3_DEF_PARAM_INTENSITY(ControlPoints<1>, intensity)

  explicit ControlPoints(std::array<ControlPoint, 1> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(std::array<Vector3, 1> points) : ControlPoints(std::move(points), std::numeric_limits<EmitIntensity>::max()) {}
  explicit ControlPoints(Vector3 point) : ControlPoints({std::move(point)}, std::numeric_limits<EmitIntensity>::max()) {}

 private:
  explicit ControlPoints(std::array<ControlPoint, 1> points, EmitIntensity intensity) : _points(std::move(points)), _intensity(intensity) {}
  explicit ControlPoints(std::array<Vector3, 1> points, EmitIntensity intensity) : _intensity(intensity) {
    for (size_t i = 0; i < 1; i++) this->_points[i] = ControlPoint(points[i]);
  }
};

struct STMSamplingModeFreq {
  Freq<float> freq;

  template <uint8_t N>
  [[nodiscard]] native_methods::FociSTMPtr focus_ptr(const std::vector<ControlPoints<N>>& points) const {
    return validate(
        native_methods::AUTDSTMFociFromFreq(freq.hz(), reinterpret_cast<const void*>(points.data()), static_cast<uint16_t>(points.size()), N));
  }

  [[nodiscard]] native_methods::GainSTMPtr gain_ptr(const std::vector<native_methods::GainPtr>& gains) const {
    return validate(AUTDSTMGainFromFreq(freq.hz(), gains.data(), static_cast<uint16_t>(gains.size())));
  }
};

struct STMSamplingModeFreqNearest {
  Freq<float> freq;

  template <uint8_t N>
  [[nodiscard]] native_methods::FociSTMPtr focus_ptr(const std::vector<ControlPoints<N>>& points) const {
    return validate(
        native_methods::AUTDSTMFociFromFreqNearest(freq.hz(), reinterpret_cast<const void*>(points.data()), static_cast<uint16_t>(points.size()), N));
  }

  [[nodiscard]] native_methods::GainSTMPtr gain_ptr(const std::vector<native_methods::GainPtr>& gains) const {
    return validate(AUTDSTMGainFromFreqNearest(freq.hz(), gains.data(), static_cast<uint16_t>(gains.size())));
  }
};

struct STMSamplingModeSamplingConfig {
  native_methods::SamplingConfigWrap config;

  template <uint8_t N>
  [[nodiscard]] native_methods::FociSTMPtr focus_ptr(const std::vector<ControlPoints<N>>& points) const {
    return AUTDSTMFociFromSamplingConfig(config, reinterpret_cast<const void*>(points.data()), static_cast<uint16_t>(points.size()), N);
  }

  [[nodiscard]] native_methods::GainSTMPtr gain_ptr(const std::vector<native_methods::GainPtr>& gains) const {
    return AUTDSTMGainFromSamplingConfig(config, gains.data(), static_cast<uint16_t>(gains.size()));
  }
};

}  // namespace autd3::driver