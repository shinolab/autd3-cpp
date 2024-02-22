#pragma once

#include "autd3/driver/common/loop_behavior.hpp"
#include "autd3/driver/common/sampling_config.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::driver {

template <class M>
class Modulation {
 public:
  Modulation() : _loop_behavior(LoopBehavior::infinite()) {}
  Modulation(const Modulation& obj) = default;
  Modulation& operator=(const Modulation& obj) = default;
  Modulation(Modulation&& obj) = default;
  Modulation& operator=(Modulation&& obj) = default;
  virtual ~Modulation() = default;  // LCOV_EXCL_LINE

  /**
   * @brief Get sampling frequency division
   * @details The sampling frequency is
   * [autd3::native_methods::FPGA_CLK_FREQ] / (sampling frequency
   * division).
   */
  [[nodiscard]] SamplingConfiguration sampling_config() const { return SamplingConfiguration(AUTDModulationSamplingConfig(modulation_ptr())); }

  [[nodiscard]] native_methods::DatagramPtr ptr(const geometry::Geometry&) const { return AUTDModulationIntoDatagram(modulation_ptr()); }

  [[nodiscard]] virtual native_methods::ModulationPtr modulation_ptr() const = 0;

  [[nodiscard]] size_t size() const { return native_methods::validate<size_t>(AUTDModulationSize(modulation_ptr())); }

  [[nodiscard]] LoopBehavior loop_behavior() const noexcept { return _loop_behavior; }

  void with_loop_behavior(const LoopBehavior loop_behavior) & { _loop_behavior = loop_behavior; }
  [[nodiscard]] M&& with_loop_behavior(const LoopBehavior loop_behavior) && {
    _loop_behavior = loop_behavior;
    return std::move(*static_cast<M*>(this));
  }

 protected:
  LoopBehavior _loop_behavior;
};

template <class M>
class ModulationWithSamplingConfig : public Modulation<M> {
 protected:
  SamplingConfiguration _config;

  explicit ModulationWithSamplingConfig(const SamplingConfiguration config) : _config(config) {}

 public:
  void with_sampling_config(const SamplingConfiguration config) & { _config = config; }
  [[nodiscard]] M&& with_sampling_config(const SamplingConfiguration config) && {
    _config = config;
    return std::move(*static_cast<M*>(this));
  }
};

}  // namespace autd3::driver
