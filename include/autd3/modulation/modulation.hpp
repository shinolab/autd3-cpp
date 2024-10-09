#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

using ModulationCalcResult = std::vector<uint8_t>;

template <class M>
class Modulation : public driver::Modulation<M> {
 public:
  using driver::Modulation<M>::Modulation;

  AUTD3_API [[nodiscard]] virtual ModulationCalcResult calc() const = 0;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
    const auto buffer = calc();
    const auto size = buffer.size();
    return native_methods::AUTDModulationCustom(this->_sampling_config, this->_loop_behavior, buffer.data(), static_cast<uint16_t>(size));
  }
};

}  // namespace autd3::modulation
