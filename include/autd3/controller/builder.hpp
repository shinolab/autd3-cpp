#pragma once

#include <optional>

#include "autd3/controller/controller.hpp"
#include "autd3/driver/autd3_device.hpp"
#include "autd3/driver/defined/freq.hpp"
#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/driver/link.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {

class ControllerBuilder {
 public:
  ControllerBuilder add_device(const driver::AUTD3& device) {
    const auto pos = device.position();
    const auto rot = device.rotation();
    _ptr = AUTDControllerBuilderAddDevice(_ptr, pos.x(), pos.y(), pos.z(), rot.w(), rot.x(), rot.y(), rot.z());
    return *this;
  }

  ControllerBuilder with_ultrasound_freq(const driver::Freq<uint32_t> ultrasound_freq) {
    _ptr = AUTDControllerBuilderWithUltrasoundFreq(_ptr, ultrasound_freq.hz());
    return *this;
  }

  template <driver::link_builder B, typename Rep, typename Period>
  [[nodiscard]] Controller<typename B::Link> open_with_timeout(B&& link_builder, const std::chrono::duration<Rep, Period> timeout) {
    const int64_t timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout.value()).count();
    auto ptr = validate(AUTDControllerOpen(_ptr, link_builder.ptr(), timeout_ns));
    driver::geometry::Geometry geometry(AUTDGeometry(ptr));
    return Controller<typename B::Link>{std::move(geometry), ptr, link_builder.resolve_link(native_methods::AUTDLinkGet(ptr))};
  }

  template <driver::link_builder B>
  [[nodiscard]] Controller<typename B::Link> open(B&& link_builder) {
    auto ptr = validate(AUTDControllerOpen(_ptr, link_builder.ptr(), -1));
    driver::geometry::Geometry geometry(AUTDGeometry(ptr));
    return Controller<typename B::Link>{std::move(geometry), ptr, link_builder.resolve_link(native_methods::AUTDLinkGet(ptr))};
  }

#ifdef AUTD3_ASYNC_API
  template <driver::link_builder B, typename Rep, typename Period>
  [[nodiscard]] coro::task<Controller<typename B::Link>> open(B&& link_builder, const std::chrono::duration<Rep, Period> timeout) {
    co_return open_with_timeout(std::forward<B>(link_builder), timeout);
  }

  template <driver::link_builder B>
  [[nodiscard]] coro::task<Controller<typename B::Link>> open_async(B&& link_builder) {
    co_return open(std::forward<B>(link_builder));
  }
#endif

  ControllerBuilder() : _ptr(native_methods::AUTDControllerBuilder()) {}

 private:
  native_methods::ControllerBuilderPtr _ptr;
};

}  // namespace autd3::controller
