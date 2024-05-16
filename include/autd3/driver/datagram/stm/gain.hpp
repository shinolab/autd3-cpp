#pragma once

#include <memory>
#include <ranges>

#include "autd3/driver/datagram/datagram.hpp"
#include "autd3/driver/datagram/gain/base.hpp"
#include "autd3/driver/datagram/with_segment_transition.hpp"
#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class R>
concept gain_range = std::ranges::viewable_range<R> && gain<std::ranges::range_value_t<R>>;

class GainSTM final : public DatagramST<native_methods::GainSTMPtr> {
 public:
  GainSTM() = delete;
  GainSTM(const GainSTM& obj) = default;
  GainSTM& operator=(const GainSTM& obj) = default;
  GainSTM(GainSTM&& obj) = default;
  GainSTM& operator=(GainSTM&& obj) = default;
  ~GainSTM() override = default;  // LCOV_EXCL_LINE

  [[nodiscard]] static GainSTM from_freq(const Freq<double> freq) { return GainSTM(freq, std::nullopt, std::nullopt); }
  [[nodiscard]] static GainSTM from_freq_nearest(const Freq<double> freq) { return GainSTM(std::nullopt, freq, std::nullopt); }
  static GainSTM from_sampling_config(const native_methods::SamplingConfigWrap config) { return GainSTM(std::nullopt, std::nullopt, config); }

  [[nodiscard]] native_methods::GainSTMPtr raw_ptr(const geometry::Geometry& geometry) const override {
    native_methods::GainSTMPtr ptr;
    if (_freq.has_value())
      ptr = native_methods::AUTDSTMGainFromFreq(_freq.value().hz());
    else if (_freq_nearest.has_value())
      ptr = native_methods::AUTDSTMGainFromFreqNearest(_freq_nearest.value().hz());
    else if (_config.has_value())
      ptr = native_methods::AUTDSTMGainFromSamplingConfig(_config.value());
    else
      throw std::runtime_error("unreachable!");
    ptr = native_methods::AUTDSTMGainWithMode(ptr, _mode);
    ptr = native_methods::AUTDSTMGainWithLoopBehavior(ptr, _loop_behavior);

    std::vector<native_methods::GainPtr> gains;
    gains.reserve(_gains.size());
    std::ranges::transform(_gains, std::back_inserter(gains), [&](const auto& gain) { return gain->gain_ptr(geometry); });
    return native_methods::AUTDSTMGainAddGains(ptr, gains.data(), static_cast<uint32_t>(gains.size()));
  }

  [[nodiscard]] native_methods::DatagramPtr into_segment(const native_methods::GainSTMPtr p, const native_methods::Segment segment) const override {
    return AUTDSTMGainIntoDatagramWithSegment(p, segment);
  }

  [[nodiscard]] native_methods::DatagramPtr into_segment_transition(const native_methods::GainSTMPtr p, const native_methods::Segment segment,
                                                                    const native_methods::TransitionModeWrap transition_mode) const override {
    return native_methods::AUTDSTMGainIntoDatagramWithSegmentTransition(p, segment, transition_mode);
  }

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry& geometry) const { return AUTDSTMGainIntoDatagram(raw_ptr(geometry)); }

  [[nodiscard]] DatagramWithSegmentTransition<native_methods::GainSTMPtr> with_segment(
      const native_methods::Segment segment, const std::optional<native_methods::TransitionModeWrap> transition_mode) {
    return DatagramWithSegmentTransition<native_methods::GainSTMPtr>(std::make_unique<GainSTM>(std::move(*this)), segment, transition_mode);
  }

  template <gain G>
  void add_gain(G&& gain) & {
    _gains.emplace_back(std::make_shared<std::remove_reference_t<G>>(std::forward<G>(gain)));
  }

  template <gain G>
  [[nodiscard]] GainSTM&& add_gain(G&& gain) && {
    _gains.emplace_back(std::make_shared<std::remove_reference_t<G>>(std::forward<G>(gain)));
    return std::move(*this);
  }

  template <gain_range R>
  void add_gains_from_iter(R&& iter) & {
    for (auto e : iter)
      _gains.emplace_back(std::make_shared<std::remove_reference_t<std::ranges::range_value_t<R>>>(std::forward<std::ranges::range_value_t<R>>(e)));
  }

  template <gain_range R>
  [[nodiscard]] GainSTM add_gains_from_iter(R&& iter) && {
    for (auto e : iter)
      _gains.emplace_back(std::make_shared<std::remove_reference_t<std::ranges::range_value_t<R>>>(std::forward<std::ranges::range_value_t<R>>(e)));
    return std::move(*this);
  }

  AUTD3_DEF_PARAM(GainSTM, native_methods::LoopBehavior, loop_behavior)
  AUTD3_DEF_PARAM(GainSTM, native_methods::GainSTMMode, mode)

 private:
  explicit GainSTM(const std::optional<Freq<double>> freq, const std::optional<Freq<double>> freq_nearest,
                   const std::optional<native_methods::SamplingConfigWrap> config)
      : _freq(freq),
        _freq_nearest(freq_nearest),
        _config(config),
        _loop_behavior(LoopBehavior::infinite()),
        _mode(native_methods::GainSTMMode::PhaseIntensityFull) {}

  std::vector<std::shared_ptr<GainBase>> _gains;
  std::optional<Freq<double>> _freq;
  std::optional<Freq<double>> _freq_nearest;
  std::optional<native_methods::SamplingConfigWrap> _config;
};

}  // namespace autd3::driver
