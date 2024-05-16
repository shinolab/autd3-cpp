#pragma once

#include <memory>

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/gain/holo/holo.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::gain::holo {

template <backend B>
class Naive final : public Holo<Naive<B>> {
 public:
  explicit Naive(std::shared_ptr<B> holo_backend) : Holo<Naive>(EmissionConstraint::DontCare), _backend(std::move(holo_backend)) {}

  [[nodiscard]] native_methods::GainPtr gain_ptr(const driver::geometry::Geometry&) const override {
    return this->_backend->naive(reinterpret_cast<const double*>(this->_foci.data()), reinterpret_cast<const double*>(this->_amps.data()),
                                 this->_amps.size(), this->_constraint);
  }

 private:
  std::shared_ptr<B> _backend;
};

}  // namespace autd3::gain::holo
