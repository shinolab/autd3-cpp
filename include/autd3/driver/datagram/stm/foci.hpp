#pragma once

#include <optional>
#include <ranges>

#include "autd3/driver/datagram/stm/stm.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R, uint8_t N>
concept foci_range_c = std::ranges::viewable_range<R> && std::is_constructible_v<ControlPoints<N>, std::ranges::range_value_t<R>>;

template <uint8_t N>
class FociSTM final : public IntoDatagramTuple<FociSTM<N>>,
                      public DatagramST<native_methods::FociSTMPtr>,
                      public IntoDatagramWithTimeout<FociSTM<N>>,
                      public IntoDatagramWithParallelThreshold<FociSTM<N>> {
 public:
  FociSTM() = delete;
  FociSTM(const FociSTM& obj) = default;             // LCOV_EXCL_LINE
  FociSTM& operator=(const FociSTM& obj) = default;  // LCOV_EXCL_LINE
  FociSTM(FociSTM&& obj) = default;                  // LCOV_EXCL_LINE
  FociSTM& operator=(FociSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~FociSTM() override = default;                     // LCOV_EXCL_LINE

  template <stm_sampling_config T, foci_range_c<N> R>
  AUTD3_API explicit FociSTM(const T sampling, const R& iter) : _loop_behavior(LoopBehavior::Infinite) {
    for (auto e : iter) _points.emplace_back(ControlPoints<N>{std::move(e)});
    _sampling = STMSamplingConfig(sampling, static_cast<uint32_t>(_points.size()));
  }

  template <foci_range_c<N> R>
  AUTD3_API [[nodiscard]] static FociSTM nearest(const Freq<float> freq, const R& iter) {
    std::vector<ControlPoints<N>> points;
    for (auto e : iter) points.emplace_back(ControlPoints<N>{std::move(e)});
    const auto n = static_cast<uint32_t>(points.size());
    return FociSTM(STMSamplingConfig::nearest(freq, n), std::move(points));
  }

  template <typename Rep, typename P, foci_range_c<N> R>
  AUTD3_API [[nodiscard]] static FociSTM nearest(const std::chrono::duration<Rep, P> period, const R& iter) {
    std::vector<ControlPoints<N>> points;
    for (auto e : iter) points.emplace_back(ControlPoints<N>{std::move(e)});
    const auto n = static_cast<uint32_t>(points.size());
    return FociSTM(STMSamplingConfig::nearest(period, n), std::move(points));
  }

  AUTD3_API [[nodiscard]] native_methods::FociSTMPtr raw_ptr(const geometry::Geometry&) const override {
    native_methods::FociSTMPtr ptr =
        validate(native_methods::AUTDSTMFoci(_sampling, reinterpret_cast<const void*>(_points.data()), static_cast<uint16_t>(_points.size()), N));
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

  AUTD3_API [[nodiscard]] Freq<float> freq() const { return _sampling.freq(); }
  AUTD3_API [[nodiscard]] std::chrono::nanoseconds period() const { return _sampling.period(); }
  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const { return _sampling.sampling_config(); }

  AUTD3_API [[nodiscard]] std::optional<SamplingConfig> sampling_config_intensity() const { return sampling_config(); }
  AUTD3_API [[nodiscard]] std::optional<SamplingConfig> sampling_config_phase() const { return sampling_config(); }

 private:
  AUTD3_API explicit FociSTM(const STMSamplingConfig sampling, std::vector<ControlPoints<N>> points)
      : _loop_behavior(LoopBehavior::Infinite), _points(std::move(points)), _sampling(sampling) {}

  std::vector<ControlPoints<N>> _points;
  STMSamplingConfig _sampling;
};

}  // namespace autd3::driver
