#pragma once

#include <chrono>
#include <string>
#include <variant>

#include "autd3/controller/builder.hpp"
#include "autd3/driver/firmware/fpga/emit_intensity.hpp"
#include "autd3/driver/firmware/fpga/phase.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/exception.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/autd3capi-link-visualizer.h"
#include "autd3/native_methods/utils.hpp"

namespace autd3::link {

using native_methods::CMap;

struct PlottersBackend {
  AUTD3_API static constexpr native_methods::Backend backend = native_methods::Backend::Plotters;
};
struct PythonBackend {
  AUTD3_API static constexpr native_methods::Backend backend = native_methods::Backend::Python;
};
struct NullBackend {
  AUTD3_API static constexpr native_methods::Backend backend = native_methods::Backend::Null;
};

struct Sphere {
  AUTD3_API static constexpr native_methods::Directivity directivity = native_methods::Directivity::Sphere;
};
struct T4010A1 {
  AUTD3_API static constexpr native_methods::Directivity directivity = native_methods::Directivity::T4010A1;
};

class Visualizer;

struct PlotRange {
  friend class Visualizer;

  float x_start;
  float x_end;
  float y_start;
  float y_end;
  float z_start;
  float z_end;
  float resolution;

  AUTD3_API PlotRange() = default;  // LCOV_EXCL_LINE
  AUTD3_API explicit PlotRange(const float x_start, const float x_end, const float y_start, const float y_end, const float z_start, const float z_end,
                               const float resolution = 1)
      : x_start(x_start), x_end(x_end), y_start(y_start), y_end(y_end), z_start(z_start), z_end(z_end), resolution(resolution) {}

  AUTD3_API [[nodiscard]] std::vector<driver::Vector3> observe_points() const {
    const auto range = ptr();
    const auto len = AUTDLinkVisualizerPlotRangeObservePointsLen(range);
    std::vector<driver::Vector3> points;
    points.resize(len);
    AUTDLinkVisualizerPlotRangeObservePoints(range, reinterpret_cast<float*>(points.data()));
    return points;
  }

 private:
  AUTD3_API [[nodiscard]] native_methods::PlotRangePtr ptr() const {
    return native_methods::AUTDLinkVisualizerPlotRange(x_start, x_end, y_start, y_end, z_start, z_end, resolution);
  }
};

struct PlotConfig {
  std::pair<uint32_t, uint32_t> figsize = std::make_pair(960, 640);
  float cbar_size = 0.15f;
  uint32_t font_size = 24;
  uint32_t label_area_size = 80;
  uint32_t margin = 10;
  float ticks_step = 10;
  CMap cmap = CMap::Jet;
  std::string fname;

  AUTD3_API [[nodiscard]] native_methods::PlotConfigPtr _raw_ptr() const {
    return validate(
        AUTDLinkVisualizerPlotConfig(figsize.first, figsize.second, cbar_size, font_size, label_area_size, margin, ticks_step, cmap, fname.c_str()));
  }
};

struct PyPlotConfig {
  std::pair<int32_t, int32_t> figsize = std::make_pair(8, 6);
  int32_t dpi = 72;
  std::string cbar_position = "right";
  std::string cbar_size = "5%";
  std::string cbar_pad = "3%";
  int32_t fontsize = 12;
  float ticks_step = 10;
  std::string cmap = "jet";
  bool show = false;
  std::string fname = "fig.png";

  AUTD3_API [[nodiscard]] native_methods::PyPlotConfigPtr _raw_ptr() const {
    return validate(native_methods::AUTDLinkVisualizerPyPlotConfig(figsize.first, figsize.second, dpi, cbar_position.c_str(), cbar_size.c_str(),
                                                                   cbar_pad.c_str(), fontsize, ticks_step, cmap.c_str(), show, fname.c_str()));
  }
};

struct NullPlotConfig {};

using Config = std::variant<PlotConfig, PyPlotConfig, NullPlotConfig>;

class Visualizer final {
  native_methods::LinkPtr _ptr;
  native_methods::Backend _backend;
  native_methods::Directivity _directivity;

  AUTD3_API [[nodiscard]] native_methods::ConfigPtr get_plot_config(const Config& config) const {
    if (_backend == native_methods::Backend::Plotters && std::holds_alternative<PlotConfig>(config)) {
      return native_methods::ConfigPtr{std::get<PlotConfig>(config)._raw_ptr()._0};
    }

    if (_backend == native_methods::Backend::Python && std::holds_alternative<PyPlotConfig>(config)) {
      return native_methods::ConfigPtr{std::get<PyPlotConfig>(config)._raw_ptr()._0};
    }

    if (_backend == native_methods::Backend::Null && std::holds_alternative<NullPlotConfig>(config))
      return native_methods::ConfigPtr{native_methods::AUTDLinkVisualizerNullPlotConfig()._0};

    throw AUTDException("Invalid plot config type.");
  }

 public:
  class Builder final {
    friend class Visualizer;
    friend class controller::ControllerBuilder;

    std::optional<int32_t> _gpu_idx;
    native_methods::Backend _backend;
    native_methods::Directivity _directivity;

    AUTD3_API explicit Builder(const native_methods::Backend backend = native_methods::Backend::Plotters,
                               const native_methods::Directivity directivity = native_methods::Directivity::Sphere)
        : _backend(backend), _directivity(directivity) {}

    AUTD3_API [[nodiscard]] Visualizer resolve_link(const native_methods::LinkPtr link) const { return Visualizer{link, _backend, _directivity}; }

   public:
    using Link = Visualizer;

    AUTD3_API [[nodiscard]] native_methods::LinkBuilderPtr ptr() const {
      switch (_backend) {
        case native_methods::Backend::Plotters:
          switch (_directivity) {
            case native_methods::Directivity::Sphere:
              return native_methods::AUTDLinkVisualizerSpherePlotters(_gpu_idx.has_value(), _gpu_idx.value_or(0));
            case native_methods::Directivity::T4010A1:
              return native_methods::AUTDLinkVisualizerT4010A1Plotters(_gpu_idx.has_value(), _gpu_idx.value_or(0));
          }
        case native_methods::Backend::Python:
          switch (_directivity) {
            case native_methods::Directivity::Sphere:
              return native_methods::AUTDLinkVisualizerSpherePython(_gpu_idx.has_value(), _gpu_idx.value_or(0));
            case native_methods::Directivity::T4010A1:
              return native_methods::AUTDLinkVisualizerT4010A1Python(_gpu_idx.has_value(), _gpu_idx.value_or(0));
          }
        case native_methods::Backend::Null:
          switch (_directivity) {
            case native_methods::Directivity::Sphere:
              return native_methods::AUTDLinkVisualizerSphereNull(_gpu_idx.has_value(), _gpu_idx.value_or(0));
            case native_methods::Directivity::T4010A1:
              return native_methods::AUTDLinkVisualizerT4010A1Null(_gpu_idx.has_value(), _gpu_idx.value_or(0));
          }
      }
      throw std::runtime_error("unreachable");  // LCOV_EXCL_LINE
    }

    [[nodiscard]] Builder with_gpu(const int32_t gpu_idx) {
      _gpu_idx = gpu_idx;
      return *this;
    }

    template <typename B>
    AUTD3_API [[nodiscard]] Builder with_backend() {
      _backend = B::backend;
      return *this;
    }

    template <typename D>
    AUTD3_API [[nodiscard]] Builder with_directivity() {
      _directivity = D::directivity;
      return *this;
    }
  };

  AUTD3_API [[nodiscard]] static Builder builder() { return Builder(); }
  AUTD3_API [[nodiscard]] static Builder plotters() { return Builder().with_backend<PlottersBackend>(); }
  AUTD3_API [[nodiscard]] static Builder python() { return Builder().with_backend<PythonBackend>(); }
  AUTD3_API [[nodiscard]] static Builder null() { return Builder().with_backend<NullBackend>(); }

  Visualizer() = delete;

  AUTD3_API explicit Visualizer(const native_methods::LinkPtr ptr, const native_methods::Backend backend,
                                const native_methods::Directivity directivity)
      : _ptr(ptr), _backend(backend), _directivity(directivity) {}

  AUTD3_API [[nodiscard]] std::vector<driver::Phase> phases(const native_methods::Segment segment, const size_t idx) const {
    const auto size = AUTDLinkVisualizerPhasesOf(_ptr, _backend, _directivity, segment, static_cast<uint16_t>(idx), nullptr);
    std::vector buf(size, driver::Phase(0));
    AUTDLinkVisualizerPhasesOf(_ptr, _backend, _directivity, segment, static_cast<uint16_t>(idx), reinterpret_cast<uint8_t*>(buf.data()));
    return buf;
  }

  AUTD3_API [[nodiscard]] std::vector<driver::EmitIntensity> intensities(const native_methods::Segment segment, const size_t idx) const {
    const auto size = AUTDLinkVisualizerIntensities(_ptr, _backend, _directivity, segment, static_cast<uint16_t>(idx), nullptr);
    std::vector buf(size, driver::EmitIntensity(0));
    AUTDLinkVisualizerIntensities(_ptr, _backend, _directivity, segment, static_cast<uint16_t>(idx), reinterpret_cast<uint8_t*>(buf.data()));
    return buf;
  }

  AUTD3_API [[nodiscard]] std::vector<uint8_t> modulation(const native_methods::Segment segment) const {
    const auto size = AUTDLinkVisualizerModulation(_ptr, _backend, _directivity, segment, nullptr);
    std::vector<uint8_t> buf(size);
    AUTDLinkVisualizerModulation(_ptr, _backend, _directivity, segment, buf.data());
    return buf;
  }

  AUTD3_API [[nodiscard]] std::vector<std::complex<float>> calc_field(const std::vector<driver::Vector3>& points,
                                                                      const driver::geometry::Geometry& geometry,
                                                                      const native_methods::Segment segment, const size_t idx) const {
    const auto points_len = static_cast<uint32_t>(points.size());
    const auto points_ptr = reinterpret_cast<const native_methods::Vector3*>(points.data());
    std::vector<std::complex<float>> buf;
    buf.resize(points_len);
    validate(AUTDLinkVisualizerCalcField(_ptr, _backend, _directivity, points_ptr, points_len, geometry.ptr(), segment, static_cast<uint16_t>(idx),
                                         reinterpret_cast<float*>(buf.data())));
    return buf;
  }

  AUTD3_API void plot_field(const Config& config, const PlotRange& range, const driver::geometry::Geometry& geometry,
                            const native_methods::Segment segment, const size_t idx) const {
    const auto config_ptr = get_plot_config(config);
    validate(AUTDLinkVisualizerPlotField(_ptr, _backend, _directivity, config_ptr, range.ptr(), geometry.ptr(), segment, static_cast<uint16_t>(idx)));
  }

  AUTD3_API void plot_phase(const Config& config, const driver::geometry::Geometry& geometry, const native_methods::Segment segment,
                            const size_t idx) const {
    const auto config_ptr = get_plot_config(config);
    validate(AUTDLinkVisualizerPlotPhase(_ptr, _backend, _directivity, config_ptr, geometry.ptr(), segment, static_cast<uint16_t>(idx)));
  }

  AUTD3_API void plot_modulation(const Config& config, const native_methods::Segment segment) const {
    const auto config_ptr = get_plot_config(config);
    validate(AUTDLinkVisualizerPlotModulation(_ptr, _backend, _directivity, config_ptr, segment));
  }
};

}  // namespace autd3::link
