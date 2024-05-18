#pragma once

#include <variant>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class R>
concept fourier_sine_range = std::ranges::viewable_range<R> && std::same_as<std::remove_reference_t<std::ranges::range_value_t<R>>, Sine>;

class Fourier final : public driver::ModulationBase<Fourier>,
                      public driver::IntoModulationCache<Fourier>,
                      public driver::IntoRadiationPressure<Fourier>,
                      public driver::IntoModulationTransform<Fourier> {
 public:
  AUTD3_API explicit Fourier(Sine component) { _components.emplace_back(std::move(component)); }

  AUTD3_API void add_component(Sine component) & { _components.emplace_back(std::move(component)); }
  AUTD3_API [[nodiscard]] Fourier&& add_component(Sine component) && {
    _components.emplace_back(std::move(component));
    return std::move(*this);
  }

  template <fourier_sine_range R>
  AUTD3_API void add_components_from_iter(R&& iter) & {
    for (Sine e : iter) _components.emplace_back(std::move(e));
  }

  template <fourier_sine_range R>
  AUTD3_API [[nodiscard]] Fourier add_components_from_iter(R&& iter) && {
    for (Sine e : iter) _components.emplace_back(std::move(e));
    return std::move(*this);
  }

  AUTD3_API Fourier& operator+=(const Sine& rhs) {
    _components.emplace_back(rhs);
    return *this;
  }

  AUTD3_API [[nodiscard]] friend Fourier&& operator+(Fourier&& lhs, const Sine& rhs) {
    lhs._components.emplace_back(rhs);
    return std::move(lhs);
  }

  AUTD3_API [[nodiscard]] friend Fourier operator+(Sine&& lhs, const Sine& rhs) {
    Fourier m(lhs);
    m._components.emplace_back(rhs);
    return m;
  }

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry& geometry) const override {
    const auto idx = _components[0]._freq.index();
    if (std::any_of(_components.begin(), _components.end(), [&](const auto& m) { return m._freq.index() != idx; }))
      throw AUTDException("All components must have the same sampling mode");

    std::vector<native_methods::ModulationPtr> components;
    components.reserve(_components.size());
    std::ranges::transform(_components, std::back_inserter(components), [&](const auto& m) { return m.modulation_ptr(geometry); });
    return std::visit([&](const auto& x) { return x.fourier_ptr(std::move(components), this->_loop_behavior); }, _components[0]._freq);
  }

 private:
  std::vector<Sine> _components;
};

}  // namespace autd3::modulation
