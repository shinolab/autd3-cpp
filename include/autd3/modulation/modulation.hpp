#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
class Modulation : public driver::Modulation<M> {
 public:
  using driver::Modulation<M>::Modulation;

  [[nodiscard]] virtual std::vector<uint8_t> calc() const = 0;

  [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry&) const override {
    const auto buffer = calc();
    const auto size = buffer.size();
    return native_methods::AUTDModulationRaw(this->_config, this->_loop_behavior, buffer.data(), static_cast<uint32_t>(size));
  }
};

}  // namespace autd3::modulation
