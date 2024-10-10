#pragma once

#include "autd3/driver/datagram/modulation/modulation.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::modulation {

template <class W>
concept window = requires(W w) {
  { w.window_size } -> std::same_as<uint32_t>;
  { w.window() } -> std::same_as<native_methods::DynWindow>;
};

template <window W>
class SincInterpolation {
 public:
  explicit SincInterpolation(W window) : _window(window) {}

  [[nodiscard]] native_methods::DynSincInterpolator dyn_resampler() const {
    return native_methods::DynSincInterpolator{.window{static_cast<uint32_t>(_window.window())}, .window_size{_window.window_size}};
  }

 private:
  W _window;
};

struct BlackMan {
  uint32_t window_size;

  explicit BlackMan(uint32_t window_size) : window_size(window_size) {}

  [[nodiscard]] native_methods::DynWindow window() const { return native_methods::DynWindow::Blackman; }
};

struct Rectangular {
  uint32_t window_size;

  explicit Rectangular(uint32_t window_size) : window_size(window_size) {}

  [[nodiscard]] native_methods::DynWindow window() const { return native_methods::DynWindow::Rectangular; }
};

}  // namespace autd3::modulation
