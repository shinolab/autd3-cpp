#pragma once

#include <unordered_map>
#include <vector>

#include "autd3.hpp"

class BurstModulation final : public autd3::Modulation<BurstModulation> {
 public:
  std::vector<uint8_t> calc() const override {
    std::vector<uint8_t> buffer(_buf_size, std::numeric_limits<uint8_t>::min());
    buffer[_buf_size - 1] = std::numeric_limits<uint8_t>::max();
    return buffer;
  }

  explicit BurstModulation(const size_t buf_size = 4000,
                           const autd3::native_methods::SamplingConfigWrap config = autd3::SamplingConfig::from_freq(4000 * autd3::Hz)) noexcept
      : Modulation(config), _buf_size(buf_size) {}

 private:
  size_t _buf_size;
};

class MyUniformGain final : public autd3::Gain<MyUniformGain> {
 public:
  MyUniformGain() = default;

  [[nodiscard]] std::unordered_map<size_t, std::vector<autd3::Drive>> calc(const autd3::Geometry& geometry) const override {
    return autd3::Gain<MyUniformGain>::transform(geometry, [](const autd3::Device&, const autd3::Transducer&) {
      return autd3::Drive{autd3::Phase(0), std::numeric_limits<autd3::EmitIntensity>::max()};
    });
  }
};

template <typename L>
inline void advanced_test(autd3::Controller<L>& autd) {
  auto config = autd3::Silencer::disable();
  autd.send(config);

  MyUniformGain g;
  BurstModulation m;

  autd.send(m, g);
}
