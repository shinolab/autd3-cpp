#pragma once

#include "autd3/controller/controller.hpp"
#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/link.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {

template <class R>
concept device_range = std::ranges::viewable_range<R> && std::same_as<std::ranges::range_value_t<R>, driver::AUTD3>;

class ControllerBuilder {
 public:
  AUTD3_API ControllerBuilder with_ultrasound_freq(const driver::Freq<uint32_t> ultrasound_freq) {
    _ptr = AUTDControllerBuilderWithUltrasoundFreq(_ptr, ultrasound_freq.hz());
    return *this;
  }

  AUTD3_API ControllerBuilder with_parallel_threshold(const size_t parallel_threshold) {
    _ptr = native_methods::AUTDControllerBuilderWithParallelThreshold(_ptr, static_cast<uint16_t>(parallel_threshold));
    return *this;
  }

  template <driver::link_builder B, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] Controller<typename B::Link> open_with_timeout(const B& link_builder, const std::chrono::duration<Rep, Period> timeout) {
    const int64_t timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout.value()).count();
    auto ptr = validate(AUTDControllerOpen(_ptr, link_builder.ptr(), timeout_ns));
    driver::geometry::Geometry geometry(AUTDGeometry(ptr));
    return Controller<typename B::Link>{std::move(geometry), ptr, link_builder.resolve_link(native_methods::AUTDLinkGet(ptr))};
  }

  template <driver::link_builder B>
  AUTD3_API [[nodiscard]] Controller<typename B::Link> open(const B& link_builder) {
    auto ptr = validate(AUTDControllerOpen(_ptr, link_builder.ptr(), -1));
    driver::geometry::Geometry geometry(AUTDGeometry(ptr));
    return Controller<typename B::Link>{std::move(geometry), ptr, link_builder.resolve_link(native_methods::AUTDLinkGet(ptr))};
  }

#ifdef AUTD3_ASYNC_API
  template <driver::link_builder B, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] coro::task<Controller<typename B::Link>> open(const B& link_builder, const std::chrono::duration<Rep, Period> timeout) {
    co_return open_with_timeout(link_builder, timeout);
  }

  template <driver::link_builder B>
  AUTD3_API [[nodiscard]] coro::task<Controller<typename B::Link>> open_async(const B& link_builder) {
    co_return open(link_builder);
  }
#endif

  template <device_range R>
  AUTD3_API explicit ControllerBuilder(R r) {
    std::vector<float> params;
    for (const auto& device : r) {
      const auto pos = device.position();
      const auto rot = device.rotation();
      params.push_back(pos.x());
      params.push_back(pos.y());
      params.push_back(pos.z());
      params.push_back(rot.w());
      params.push_back(rot.x());
      params.push_back(rot.y());
      params.push_back(rot.z());
    }
    _ptr = native_methods::AUTDControllerBuilder(params.data(), static_cast<uint16_t>(params.size() / 7));
  }

  AUTD3_API explicit ControllerBuilder(const std::initializer_list<driver::AUTD3> r) : ControllerBuilder(std::vector(r)) {}

 private:
  native_methods::ControllerBuilderPtr _ptr;
};

}  // namespace autd3::controller
