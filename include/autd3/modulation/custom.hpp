#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

class Custom final : public driver::ModulationBase<Custom>,
                     public driver::IntoModulationCache<Custom>,
                     public driver::IntoRadiationPressure<Custom>,
                     public driver::IntoModulationTransform<Custom> {
 public:
  explicit Custom(std::vector<uint8_t> buf, const driver::SamplingConfig sampling_config) : _sampling_config(sampling_config), _buf(std::move(buf)) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    const auto size = _buf.size();
    return native_methods::AUTDModulationRaw(this->_sampling_config, this->_loop_behavior, _buf.data(), static_cast<uint16_t>(size));
  }

 private:
  driver::SamplingConfig _sampling_config;
  std::vector<uint8_t> _buf;
};

}  // namespace autd3::modulation
