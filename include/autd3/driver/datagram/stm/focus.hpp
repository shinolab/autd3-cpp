#pragma once

#include <ranges>
#include <variant>

#include "autd3/driver/datagram/stm/stm.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R, uint8_t N>
concept foci_range_c = std::ranges::viewable_range<R> && std::is_constructible_v<ControlPoints<N>, std::ranges::range_value_t<R>>;

template <uint8_t N>
class FociSTM final : public DatagramST<native_methods::FociSTMPtr>,
                      public IntoDatagramWithTimeout<FociSTM<N>>,
                      public IntoDatagramWithParallelThreshold<FociSTM<N>> {
 public:
  FociSTM() = delete;
  FociSTM(const FociSTM& obj) = default;             // LCOV_EXCL_LINE
  FociSTM& operator=(const FociSTM& obj) = default;  // LCOV_EXCL_LINE
  FociSTM(FociSTM&& obj) = default;                  // LCOV_EXCL_LINE
  FociSTM& operator=(FociSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~FociSTM() override = default;                     // LCOV_EXCL_LINE

  template <foci_range_c<N> R>
  AUTD3_API [[nodiscard]] static FociSTM from_freq(const Freq<float> freq, R&& iter) {
    return FociSTM(STMSamplingModeFreq{freq}, std::forward<R>(iter));
  }

  template <foci_range_c<N> R>
  AUTD3_API [[nodiscard]] static FociSTM from_freq_nearest(const Freq<float> freq, R&& iter) {
    return FociSTM(STMSamplingModeFreqNearest{freq}, std::forward<R>(iter));
  }

  template <foci_range_c<N> R>
  AUTD3_API [[nodiscard]] static FociSTM from_sampling_config(const native_methods::SamplingConfigWrap config, R&& iter) {
    return FociSTM(STMSamplingModeSamplingConfig{config}, std::forward<R>(iter));
  }

  AUTD3_API [[nodiscard]] native_methods::FociSTMPtr raw_ptr(const geometry::Geometry&) const override {
    std::vector<float> points;
    std::vector<uint8_t> offsets;
    std::vector<uint8_t> intensities;
    for (const auto& cp : _points) {
      for (const auto& point : cp.points) {
        points.push_back(point.point.x());
        points.push_back(point.point.y());
        points.push_back(point.point.z());
        offsets.push_back(point.offset.value());
      }
      intensities.push_back(cp.intensity.value());
    }
    native_methods::FociSTMPtr ptr = std::visit([&](const auto s) { return s.focus_ptr(points, offsets, intensities, N); }, _sampling);
    ptr = AUTDSTMFociWithLoopBehavior(ptr, N, _loop_behavior);
    return ptr;
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::FociSTMPtr p,
                                                                   const native_methods::Segment segment) const override {
    return AUTDSTMFociIntoDatagramWithSegment(p, N, segment);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::FociSTMPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMFociIntoDatagramWithSegmentTransition(p, N, segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDSTMFociIntoDatagram(raw_ptr(geometry), N);
  }

  AUTD3_API
  [[nodiscard]] DatagramWithSegmentTransition<native_methods::FociSTMPtr> with_segment(
      const native_methods::Segment segment, std::optional<native_methods::TransitionModeWrap> transition_mode) {
    return DatagramWithSegmentTransition<native_methods::FociSTMPtr>(std::make_unique<FociSTM>(std::move(*this)), segment,
                                                                     std::move(transition_mode));
  }

  AUTD3_DEF_PARAM(FociSTM, native_methods::LoopBehavior, loop_behavior)

 private:
  template <foci_range_c<N> R>
  AUTD3_API explicit FociSTM(const std::variant<STMSamplingModeFreq, STMSamplingModeFreqNearest, STMSamplingModeSamplingConfig> sampling,
                             const R& iter)
      : _loop_behavior(LoopBehavior::Infinite), _sampling(sampling) {
    for (auto e : iter) _points.push_back(ControlPoints<N>{std::move(e)});
  }

  std::vector<ControlPoints<N>> _points;
  std::variant<STMSamplingModeFreq, STMSamplingModeFreqNearest, STMSamplingModeSamplingConfig> _sampling;
};

}  // namespace autd3::driver
