#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class M>
class Modulation : public driver::Modulation<M> {
 public:
  using driver::Modulation<M>::Modulation;

  AUTD3_API [[nodiscard]] virtual std::vector<driver::EmitIntensity> calc(
      const driver::geometry::Geometry &) const = 0;

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(
      const driver::geometry::Geometry &geometry) const override {
    const auto buffer = calc(geometry);
    const auto size = buffer.size();
    return native_methods::AUTDModulationRaw(
        this->_sampling_config, this->_loop_behavior,
        reinterpret_cast<const uint8_t *>(buffer.data()),
        static_cast<uint32_t>(size));
  }
};

}  // namespace autd3::modulation
