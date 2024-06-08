#pragma once

#include <variant>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class R>
concept mixer_sine_range = std::ranges::viewable_range<R> && std::same_as<std::remove_reference_t<std::ranges::range_value_t<R>>, Sine>;

class Mixer final : public driver::ModulationBase<Mixer>,
                    public driver::IntoModulationCache<Mixer>,
                    public driver::IntoRadiationPressure<Mixer>,
                    public driver::IntoModulationTransform<Mixer> {
 public:
  template <mixer_sine_range R>
  AUTD3_API explicit Mixer(R&& iter) {
    for (Sine e : iter) _components.emplace_back(std::move(e));
  }
  AUTD3_API explicit Mixer(const std::initializer_list<Sine> components) : _components(components) {}

  AUTD3_API [[nodiscard]] native_methods::ModulationPtr modulation_ptr(const driver::geometry::Geometry& geometry) const override {
    const auto idx = _components[0]._freq.index();
    if (std::ranges::any_of(_components, [&](const auto& m) { return m._freq.index() != idx; }))
      throw AUTDException("All components must have the same sampling mode");

    std::vector<native_methods::ModulationPtr> components;
    components.reserve(_components.size());
    std::ranges::transform(_components, std::back_inserter(components), [&](const auto& m) { return m.modulation_ptr(geometry); });
    return std::visit([&](const auto& x) { return x.mixer_ptr(std::move(components), this->_loop_behavior); }, _components[0]._freq);
  }

 private:
  std::vector<Sine> _components;
};

}  // namespace autd3::modulation
