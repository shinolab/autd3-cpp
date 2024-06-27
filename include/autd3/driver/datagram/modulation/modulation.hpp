#pragma once

#include "autd3/driver/datagram/modulation/base.hpp"
#include "autd3/driver/datagram/modulation/cache.hpp"
#include "autd3/driver/datagram/modulation/radiation_pressure.hpp"
#include "autd3/driver/datagram/modulation/transform.hpp"
#include "autd3/driver/firmware/fpga/sampling_config.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class M>
class Modulation : public ModulationBase<M>, public IntoModulationCache<M>, public IntoRadiationPressure<M>, public IntoModulationTransform<M> {
 public:
  void with_sampling_config(SamplingConfig value) & { this->_sampling_config = std::move(value); } /* LCOV_EXCL_LINE */
  [[nodiscard]] M&& with_sampling_config(SamplingConfig value) && {                                /* LCOV_EXCL_LINE */
    this->_sampling_config = std::move(value);                                                     /* LCOV_EXCL_LINE */
    return std::move(*static_cast<M*>(this));                                                      /* LCOV_EXCL_LINE */
  } /* LCOV_EXCL_LINE */

 protected:
  AUTD3_API explicit Modulation(const SamplingConfig sampling_config) : _sampling_config(sampling_config) {}

  SamplingConfig _sampling_config;
};

}  // namespace autd3::driver
