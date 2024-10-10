#pragma once

#include <variant>

#include "autd3/native_methods.hpp"

namespace autd3::modulation {

struct BlackMan {
  uint32_t window_size;

  explicit BlackMan(const uint32_t window_size) : window_size(window_size) {}

  [[nodiscard]] static native_methods::DynWindow window() { return native_methods::DynWindow::Blackman; }
};

struct Rectangular {
  uint32_t window_size;

  explicit Rectangular(const uint32_t window_size) : window_size(window_size) {}

  [[nodiscard]] static native_methods::DynWindow window() { return native_methods::DynWindow::Rectangular; }
};

class SincInterpolation {
 public:
  SincInterpolation() : SincInterpolation(BlackMan(32)) {}
  explicit SincInterpolation(std::variant<BlackMan, Rectangular> window) : _window(std::move(window)) {}

  [[nodiscard]] native_methods::DynSincInterpolator dyn_resampler() const {
    const auto window = std::visit([](const auto& w) { return w.window(); }, _window);
    const auto window_size = std::visit([](const auto& w) { return w.window_size; }, _window);
    return native_methods::DynSincInterpolator{.window{static_cast<uint32_t>(window)}, .window_size = window_size};
  }

 private:
  std::variant<BlackMan, Rectangular> _window;
};

}  // namespace autd3::modulation