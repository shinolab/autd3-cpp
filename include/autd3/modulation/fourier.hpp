#pragma once

#include <numeric>

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/modulation/sine.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class T, class R>
concept fourier_sine_range = std::ranges::viewable_range<R> && std::same_as<std::ranges::range_value_t<R>, T>;

template <class T>
class Fourier final : public driver::ModulationBase<Fourier<T>>,
                      public driver::IntoModulationCache<Fourier<T>>,
                      public driver::IntoRadiationPressure<Fourier<T>>,
                      public driver::IntoModulationTransform<Fourier<T>> {
 public:
  explicit Fourier(T component) : driver::ModulationBase() { _components.emplace_back(std::move(component)); }

  void add_component(T component) & { _components.emplace_back(std::move(component)); }

  [[nodiscard]] Fourier&& add_component(T component) && {
    _components.emplace_back(std::move(component));
    return std::move(*this);
  }

  template <fourier_sine_range<T> R>
  void add_components_from_iter(R&& iter) & {
    for (T e : iter) _components.emplace_back(std::move(e));
  }

  template <fourier_sine_range<T> R>
  [[nodiscard]] Fourier add_components_from_iter(R&& iter) && {
    for (T e : iter) _components.emplace_back(std::move(e));
    return std::move(*this);
  }

  [[nodiscard]] Fourier& operator+=(const T& rhs) {
    _components.emplace_back(rhs);
    return *this;
  }

  [[nodiscard]] friend Fourier&& operator+(Fourier&& lhs, const T& rhs) {
    lhs._components.emplace_back(rhs);
    return std::move(lhs);
  }

  [[nodiscard]] friend Fourier operator+(T&& lhs, const T& rhs) {
    Fourier m(lhs);
    m._components.emplace_back(rhs);
    return m;
  }  // LCOV_EXCL_LINE

  //[[nodiscard]] native_methods::ModulationPtr modulation_ptr() const override {
  //  std::vector<native_methods::ModulationPtr> components;
  //  components.reserve(_components.size());
  //  std::ranges::transform(_components, std::back_inserter(components), [&](const auto& m) { return m.modulation_ptr(); });
  //  return AUTDModulationFourier(components.data(), static_cast<uint32_t>(components.size()),
  //                               static_cast<native_methods::LoopBehavior>(_loop_behavior));
  //}

 private:
  std::vector<T> _components;
};

}  // namespace autd3::modulation
