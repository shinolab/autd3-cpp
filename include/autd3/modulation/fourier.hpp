#pragma once

#include "autd3/driver/datagram/modulation.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct FourierOption {
  std::optional<float> scale_factor;
  bool clamp;
  uint8_t offset;

  operator native_methods::FourierOption() const {
    return native_methods::FourierOption{.has_scale_factor = scale_factor.has_value(),
                                         .scale_factor = scale_factor.value_or(std::numeric_limits<float>::quiet_NaN()),
                                         .clamp = clamp,
                                         .offset = offset};
  }
};

template <typename>
struct Fourier {};

template <>
struct Fourier<driver::Freq<uint32_t>> final : driver::Modulation, driver::IntoDatagramTuple<Fourier<driver::Freq<uint32_t>>> {
  AUTD3_API explicit Fourier(std::vector<Sine<driver::Freq<uint32_t>>> components, const FourierOption option)
      : components(std::move(components)), option(option) {}

  std::vector<Sine<driver::Freq<uint32_t>>> components;
  FourierOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    std::vector<uint32_t> sine_freq;
    sine_freq.reserve(components.size());
    std::ranges::transform(components, std::back_inserter(sine_freq), [&](const auto& m) { return m.freq.hz(); });
    std::vector<native_methods::SineOption> sine_option;
    sine_option.reserve(components.size());
    std::ranges::transform(components, std::back_inserter(sine_option), [&](const auto& m) { return m.option; });

    return AUTDModulationFourierExact(sine_freq.data(), sine_option.data(), static_cast<uint32_t>(components.size()), option);
  }
};

template <>
struct Fourier<driver::Freq<float>> final : driver::Modulation, driver::IntoDatagramTuple<Fourier<driver::Freq<float>>> {
  AUTD3_API explicit Fourier(std::vector<Sine<driver::Freq<float>>> components, const FourierOption option)
      : components(std::move(components)), option(option) {}

  std::vector<Sine<driver::Freq<float>>> components;
  FourierOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    std::vector<float> sine_freq;
    sine_freq.reserve(components.size());
    std::ranges::transform(components, std::back_inserter(sine_freq), [&](const auto& m) { return m.freq.hz(); });
    std::vector<native_methods::SineOption> sine_option;
    sine_option.reserve(components.size());
    std::ranges::transform(components, std::back_inserter(sine_option), [&](const auto& m) { return m.option; });

    return AUTDModulationFourierExactFloat(sine_freq.data(), sine_option.data(), static_cast<uint32_t>(components.size()), option);
  }
};

template <>
struct Fourier<Nearest> final : driver::Modulation, driver::IntoDatagramTuple<Fourier<Nearest>> {
  AUTD3_API explicit Fourier(std::vector<Sine<Nearest>> components, const FourierOption option) : components(std::move(components)), option(option) {}

  std::vector<Sine<Nearest>> components;
  FourierOption option;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    std::vector<float> sine_freq;
    sine_freq.reserve(components.size());
    std::ranges::transform(components, std::back_inserter(sine_freq), [&](const Sine<Nearest>& m) { return m.freq.hz(); });
    std::vector<native_methods::SineOption> sine_option;
    sine_option.reserve(components.size());
    std::ranges::transform(components, std::back_inserter(sine_option), [&](const auto& m) { return m.option; });

    return AUTDModulationFourierNearest(sine_freq.data(), sine_option.data(), static_cast<uint32_t>(components.size()), option);
  }
};

Fourier(std::vector<Sine<driver::Freq<uint32_t>>> components, FourierOption option) -> Fourier<driver::Freq<uint32_t>>;
Fourier(std::vector<Sine<driver::Freq<float>>> components, FourierOption option) -> Fourier<driver::Freq<float>>;
Fourier(std::vector<Sine<Nearest>> components, FourierOption option) -> Fourier<Nearest>;

}  // namespace autd3::modulation
