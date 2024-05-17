#pragma once

#include <vector>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/constraint.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

class Greedy final : public Holo<Greedy> {
 public:
  AUTD3_API Greedy() : Holo(EmissionConstraint::Uniform(std::numeric_limits<driver::EmitIntensity>::max())), _phase_div(16) {}

  AUTD3_DEF_PARAM(Greedy, uint8_t, phase_div)

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return native_methods::AUTDGainHoloGreedySphere(reinterpret_cast<const double*>(this->_foci.data()),
                                                    reinterpret_cast<const double*>(this->_amps.data()), this->_amps.size(), _phase_div, _constraint);
  }
};

}  // namespace autd3::gain::holo
