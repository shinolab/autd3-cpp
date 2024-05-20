#pragma once

#include <ranges>
#include <variant>

#include "autd3/def.hpp"
#include "autd3/driver/datagram/stm/stm.hpp"
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
concept focus_range_v = std::ranges::viewable_range<R> &&
                        std::same_as<std::ranges::range_value_t<R>, Vector3>;

template <class R>
concept focus_range_c =
    std::ranges::viewable_range<R> &&
    std::same_as<std::ranges::range_value_t<R>, ControlPoint>;

class FocusSTM final : public DatagramST<native_methods::FocusSTMPtr> {
 public:
  FocusSTM() = delete;
  FocusSTM(const FocusSTM& obj) = default;             // LCOV_EXCL_LINE
  FocusSTM& operator=(const FocusSTM& obj) = default;  // LCOV_EXCL_LINE
  FocusSTM(FocusSTM&& obj) = default;                  // LCOV_EXCL_LINE
  FocusSTM& operator=(FocusSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~FocusSTM() override = default;                      // LCOV_EXCL_LINE

  AUTD3_API [[nodiscard]] static FocusSTM from_freq(const Freq<double> freq) {
    return FocusSTM(STMSamplingModeFreq{freq});
  }
  AUTD3_API [[nodiscard]] static FocusSTM from_freq_nearest(
      const Freq<double> freq) {
    return FocusSTM(STMSamplingModeFreqNearest{freq});
  }
  AUTD3_API [[nodiscard]] static FocusSTM from_sampling_config(
      const native_methods::SamplingConfigWrap config) {
    return FocusSTM(STMSamplingModeSamplingConfig{config});
  }

  AUTD3_API [[nodiscard]] native_methods::FocusSTMPtr raw_ptr(
      const geometry::Geometry&) const override {
    native_methods::FocusSTMPtr ptr =
        std::visit([](const auto s) { return s.focus_ptr(); }, _sampling);
    ptr = AUTDSTMFocusWithLoopBehavior(ptr, _loop_behavior);
    return AUTDSTMFocusAddFoci(
        ptr, reinterpret_cast<const double*>(_points.data()),
        reinterpret_cast<const uint8_t*>(_intensities.data()),
        _intensities.size());
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment(
      const native_methods::FocusSTMPtr p,
      const native_methods::Segment segment) const override {
    return AUTDSTMFocusIntoDatagramWithSegment(p, segment);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::FocusSTMPtr p,
      const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMFocusIntoDatagramWithSegmentTransition(p, segment,
                                                         transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(
      const geometry::Geometry& geometry) const {
    return AUTDSTMFocusIntoDatagram(raw_ptr(geometry));
  }

  AUTD3_API
      [[nodiscard]] DatagramWithSegmentTransition<native_methods::FocusSTMPtr>
      with_segment(
          const native_methods::Segment segment,
          std::optional<native_methods::TransitionModeWrap> transition_mode) {
    return DatagramWithSegmentTransition<native_methods::FocusSTMPtr>(
        std::make_unique<FocusSTM>(std::move(*this)), segment,
        std::move(transition_mode));
  }

  AUTD3_API void add_focus(Vector3 point,
                           const EmitIntensity intensity =
                               std::numeric_limits<EmitIntensity>::max()) & {
    _points.emplace_back(std::move(point));
    _intensities.emplace_back(intensity);
  }

  AUTD3_API [[nodiscard]] FocusSTM&& add_focus(
      Vector3 point, const EmitIntensity intensity =
                         std::numeric_limits<EmitIntensity>::max()) && {
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
  AUTD3_API explicit FocusSTM(
      std::variant<STMSamplingModeFreq, STMSamplingModeFreqNearest,
                   STMSamplingModeSamplingConfig>
          sampling)
      : _loop_behavior(LoopBehavior::Infinite),
        _sampling(std::move(sampling)) {}

  std::vector<Vector3> _points;
  std::vector<EmitIntensity> _intensities;
  std::variant<STMSamplingModeFreq, STMSamplingModeFreqNearest,
               STMSamplingModeSamplingConfig>
      _sampling;
};

}  // namespace autd3::driver
