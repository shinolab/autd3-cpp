#pragma once

#include <memory>
#include <vector>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

template <backend B>
class LM final : public Holo<LM<B>> {
 public:
  AUTD3_API explicit LM(std::shared_ptr<B> holo_backend)
      : Holo<LM>(EmissionConstraint::DontCare), _eps1(1e-8), _eps2(1e-8), _tau(1e-3), _k_max(5), _backend(std::move(holo_backend)) {}

  AUTD3_DEF_PARAM(LM, double, eps1)
  AUTD3_DEF_PARAM(LM, double, eps2)
  AUTD3_DEF_PARAM(LM, double, tau)
  AUTD3_DEF_PARAM(LM, uint32_t, k_max)

  AUTD3_API void with_initial(std::vector<double> value) & { _initial = std::move(value); }
  AUTD3_API [[nodiscard]] LM&& with_initial(std::vector<double> value) && {
    _initial = std::move(value);
    return std::move(*this);
  }

  AUTD3_API [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->lm(reinterpret_cast<const double*>(this->_foci.data()), reinterpret_cast<const double*>(this->_amps.data()),
                              this->_amps.size(), _eps1, _eps2, _tau, _k_max, _initial.data(), _initial.size(), this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
  std::vector<double> _initial;
};

}  // namespace autd3::gain::holo
