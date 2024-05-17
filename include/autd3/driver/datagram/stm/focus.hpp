#pragma once

#include <ranges>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

struct ControlPoint {
  Vector3 point;
  EmitIntensity intensity;
};

template <class R>
concept focus_range_v = std::ranges::viewable_range<R> && std::same_as<std::ranges::range_value_t<R>, Vector3>;

template <class R>
concept focus_range_c = std::ranges::viewable_range<R> && std::same_as<std::ranges::range_value_t<R>, ControlPoint>;

class FocusSTM final : public DatagramST<native_methods::FocusSTMPtr> {
 public:
  FocusSTM() = delete;
  FocusSTM(const FocusSTM& obj) = default;
  FocusSTM& operator=(const FocusSTM& obj) = default;
  FocusSTM(FocusSTM&& obj) = default;
  FocusSTM& operator=(FocusSTM&& obj) = default;
  ~FocusSTM() override = default;

  AUTD3_API [[nodiscard]] static FocusSTM from_freq(const Freq<double> freq) { return FocusSTM(freq, std::nullopt, std::nullopt); }
  AUTD3_API [[nodiscard]] static FocusSTM from_freq_nearest(const Freq<double> freq) { return FocusSTM(std::nullopt, freq, std::nullopt); }
  AUTD3_API [[nodiscard]] static FocusSTM from_sampling_config(const native_methods::SamplingConfigWrap config) {
    return FocusSTM(std::nullopt, std::nullopt, config);
  }

  AUTD3_API [[nodiscard]] native_methods::FocusSTMPtr raw_ptr(const geometry::Geometry&) const override {
    native_methods::FocusSTMPtr ptr;
    if (_freq.has_value())
      ptr = native_methods::AUTDSTMFocusFromFreq(_freq.value().hz());
    else if (_freq_nearest.has_value())
      ptr = native_methods::AUTDSTMFocusFromFreqNearest(_freq_nearest.value().hz());
    else if (_config.has_value())
      ptr = AUTDSTMFocusFromSamplingConfig(_config.value());
    else
      throw std::runtime_error("unreachable!");
    ptr = AUTDSTMFocusWithLoopBehavior(ptr, _loop_behavior);
    return AUTDSTMFocusAddFoci(ptr, reinterpret_cast<const double*>(_points.data()), reinterpret_cast<const uint8_t*>(_intensities.data()),
                               _intensities.size());
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::FocusSTMPtr p,
                                                                   const native_methods::Segment segment) const override {
    return AUTDSTMFocusIntoDatagramWithSegment(p, segment);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::FocusSTMPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMFocusIntoDatagramWithSegmentTransition(p, segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDSTMFocusIntoDatagram(raw_ptr(geometry));
  }

  AUTD3_API [[nodiscard]] DatagramWithSegmentTransition<native_methods::FocusSTMPtr> with_segment(
      const native_methods::Segment segment, std::optional<native_methods::TransitionModeWrap> transition_mode) {
    return DatagramWithSegmentTransition<native_methods::FocusSTMPtr>(std::make_unique<FocusSTM>(std::move(*this)), segment,
                                                                      std::move(transition_mode));
  }

  AUTD3_API void add_focus(Vector3 point, const EmitIntensity intensity = std::numeric_limits<EmitIntensity>::max()) & {
    _points.emplace_back(std::move(point));
    _intensities.emplace_back(intensity);
  }

  AUTD3_API [[nodiscard]] FocusSTM&& add_focus(Vector3 point, const EmitIntensity intensity = std::numeric_limits<EmitIntensity>::max()) && {
    _points.emplace_back(std::move(point));
    _intensities.emplace_back(intensity);
    return std::move(*this);
  }

  AUTD3_API void add_focus(ControlPoint p) & {
    _points.emplace_back(std::move(p.point));
    _intensities.emplace_back(p.intensity);
  }

  AUTD3_API [[nodiscard]] FocusSTM&& add_focus(ControlPoint p) && {
    _points.emplace_back(std::move(p.point));
    _intensities.emplace_back(p.intensity);
    return std::move(*this);
  }

  template <focus_range_v R>
  AUTD3_API void add_foci_from_iter(R&& iter) & {
    for (Vector3 e : iter) {
      _points.emplace_back(std::move(e));
      _intensities.emplace_back(std::numeric_limits<EmitIntensity>::max());
    }
  }

  template <focus_range_v R>
  AUTD3_API [[nodiscard]] FocusSTM add_foci_from_iter(R&& iter) && {
    for (Vector3 e : iter) {
      _points.emplace_back(std::move(e));
      _intensities.emplace_back(std::numeric_limits<EmitIntensity>::max());
    }
    return std::move(*this);
  }

  template <focus_range_c R>
  AUTD3_API void add_foci_from_iter(R&& iter) & {
    for (ControlPoint e : iter) {
      _points.emplace_back(std::move(e.point));
      _intensities.emplace_back(e.intensity);
    }
  }

  template <focus_range_c R>
  AUTD3_API [[nodiscard]] FocusSTM add_foci_from_iter(R&& iter) && {
    for (ControlPoint e : iter) {
      _points.emplace_back(std::move(e.point));
      _intensities.emplace_back(e.intensity);
    }
    return std::move(*this);
  }

  AUTD3_DEF_PARAM(FocusSTM, native_methods::LoopBehavior, loop_behavior)

 private:
  AUTD3_API explicit FocusSTM(const std::optional<Freq<double>> freq, const std::optional<Freq<double>> freq_nearest,
                              std::optional<native_methods::SamplingConfigWrap> config)
      : _loop_behavior(LoopBehavior::infinite()), _freq(freq), _freq_nearest(freq_nearest), _config(std::move(config)) {}

  std::vector<Vector3> _points;
  std::vector<EmitIntensity> _intensities;
  std::optional<Freq<double>> _freq;
  std::optional<Freq<double>> _freq_nearest;
  std::optional<native_methods::SamplingConfigWrap> _config;
};

}  // namespace autd3::driver
