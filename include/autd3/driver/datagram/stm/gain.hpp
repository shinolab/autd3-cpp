#pragma once

#include <memory>
#include <ranges>

#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/tuple.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/stm_sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R>
concept gain_range = std::ranges::viewable_range<R> && gain<std::ranges::range_value_t<R>>;

class GainSTM final : public IntoDatagramTuple<GainSTM>,
                      public DatagramST<native_methods::GainSTMPtr>,
                      public IntoDatagramWithTimeout<GainSTM>,
                      public IntoDatagramWithParallelThreshold<GainSTM> {
 public:
  GainSTM() = delete;
  GainSTM(const GainSTM& obj) = default;             // LCOV_EXCL_LINE
  GainSTM& operator=(const GainSTM& obj) = default;  // LCOV_EXCL_LINE
  GainSTM(GainSTM&& obj) = default;                  // LCOV_EXCL_LINE
  GainSTM& operator=(GainSTM&& obj) = default;       // LCOV_EXCL_LINE
  ~GainSTM() override = default;                     // LCOV_EXCL_LINE

  template <gain_range R>
  AUTD3_API [[nodiscard]] static GainSTM from_freq(const Freq<float> freq, const R& iter) {
    return GainSTM(STMSamplingConfig::Freq(freq), iter);
  }
  template <gain G>
  AUTD3_API [[nodiscard]] static GainSTM from_freq(const Freq<float> freq, std::initializer_list<G> iter) {
    return GainSTM::from_freq(freq, std::vector(iter));
  }
  template <gain_range R>
  AUTD3_API [[nodiscard]] static GainSTM from_freq_nearest(const Freq<float> freq, const R& iter) {
    return GainSTM(STMSamplingConfig::FreqNearest(freq), iter);
  }
  template <gain G>
  AUTD3_API [[nodiscard]] static GainSTM from_freq_nearest(const Freq<float> freq, std::initializer_list<G> iter) {
    return GainSTM::from_freq_nearest(freq, std::vector(iter));
  }
  template <gain_range R>
  AUTD3_API [[nodiscard]] static GainSTM from_sampling_config(const SamplingConfig config, const R& iter) {
    return GainSTM(STMSamplingConfig::SamplingConfig(config), iter);
  }
  template <gain G>
  AUTD3_API [[nodiscard]] static GainSTM from_sampling_config(const SamplingConfig config, std::initializer_list<G> iter) {
    return GainSTM::from_sampling_config(config, std::vector(iter));
  }

  AUTD3_API [[nodiscard]] native_methods::GainSTMPtr raw_ptr(const geometry::Geometry& geometry) const override {
    std::vector<native_methods::GainPtr> gains;
    gains.reserve(_gains.size());
    std::ranges::transform(_gains, std::back_inserter(gains), [&](const auto& gain) { return gain->gain_ptr(geometry); });
    native_methods::GainSTMPtr ptr = validate(AUTDSTMGain(_sampling, gains.data(), static_cast<uint16_t>(gains.size())));
    ptr = AUTDSTMGainWithMode(ptr, _mode);
    ptr = AUTDSTMGainWithLoopBehavior(ptr, _loop_behavior);
    return ptr;
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::GainSTMPtr p,
                                                                   const native_methods::Segment segment) const override {
    return AUTDSTMGainIntoDatagramWithSegment(p, segment);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr into_segment_transition(
      const native_methods::GainSTMPtr p, const native_methods::Segment segment,
      const native_methods::TransitionModeWrap transition_mode) const override {
    return AUTDSTMGainIntoDatagramWithSegmentTransition(p, segment, transition_mode);
  }

  AUTD3_API [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const {
    return AUTDSTMGainIntoDatagram(raw_ptr(geometry));
  }

  AUTD3_API
  [[nodiscard]] DatagramWithSegmentTransition<native_methods::GainSTMPtr> with_segment(
      const native_methods::Segment segment, const std::optional<native_methods::TransitionModeWrap>& transition_mode) {
    return DatagramWithSegmentTransition<native_methods::GainSTMPtr>(std::make_unique<GainSTM>(std::move(*this)), segment, transition_mode);
  }

  AUTD3_DEF_PARAM(GainSTM, native_methods::LoopBehavior, loop_behavior)
  AUTD3_DEF_PARAM(GainSTM, native_methods::GainSTMMode, mode)

  AUTD3_API [[nodiscard]] Freq<float> freq() const { return _sampling.freq(_gains.size()); }
  AUTD3_API [[nodiscard]] std::chrono::nanoseconds period() const { return _sampling.period(_gains.size()); }
  AUTD3_API [[nodiscard]] SamplingConfig sampling_config() const { return _sampling.sampling_config(_gains.size()); }

 private:
  template <gain_range R>
  AUTD3_API explicit GainSTM(const STMSamplingConfig sampling, const R& iter)
      : _loop_behavior(LoopBehavior::Infinite), _mode(native_methods::GainSTMMode::PhaseIntensityFull), _sampling(sampling) {
    for (auto e : iter)
      _gains.emplace_back(std::make_shared<std::remove_reference_t<std::ranges::range_value_t<R>>>(std::forward<std::ranges::range_value_t<R>>(e)));
  }

  std::vector<std::shared_ptr<GainBase>> _gains;
  STMSamplingConfig _sampling;
};

}  // namespace autd3::driver
