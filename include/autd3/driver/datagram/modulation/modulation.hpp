#pragma once

#include "autd3/driver/datagram/modulation/base.hpp"
#include "autd3/driver/datagram/modulation/cache.hpp"
#include "autd3/driver/datagram/modulation/radiation_pressure.hpp"
#include "autd3/driver/datagram/modulation/transform.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <class M>
class Modulation : public ModulationBase<M>, public IntoModulationCache<M>, public IntoRadiationPressure<M>, public IntoModulationTransform<M> {
 protected:
  AUTD3_API explicit Modulation(const native_methods::SamplingConfigWrap sampling_config) : _sampling_config(sampling_config) {}

 public:
  AUTD3_DEF_PARAM(M, native_methods::SamplingConfigWrap, sampling_config)
};

}  // namespace autd3::driver
