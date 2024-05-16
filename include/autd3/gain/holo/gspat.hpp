#pragma once

#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::gain::holo {

template <backend B>
class GSPAT final : public Holo<GSPAT<B>> {
 public:
  explicit GSPAT(std::shared_ptr<B> holo_backend) : Holo<GSPAT>(EmissionConstraint::DontCare), _repeat(100), _backend(std::move(holo_backend)) {}

  AUTD3_DEF_PARAM(GSPAT, uint32_t, repeat)

  [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->gspat(reinterpret_cast<const double*>(this->_foci.data()), reinterpret_cast<const double*>(this->_amps.data()),
                                 this->_amps.size(), _repeat, this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
};

}  // namespace autd3::gain::holo
