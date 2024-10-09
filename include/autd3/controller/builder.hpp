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
  AUTD3_API ControllerBuilder with_parallel_threshold(const size_t parallel_threshold) {
    _ptr = AUTDControllerBuilderWithParallelThreshold(_ptr, static_cast<uint16_t>(parallel_threshold));
    return *this;
  }

  AUTD3_API ControllerBuilder with_timer_resolution(const uint32_t resolution) {
    _ptr = AUTDControllerBuilderWithTimerResolution(_ptr, resolution);
    return *this;
  }

  template <typename Rep, typename Period>
  AUTD3_API ControllerBuilder with_send_interval(const std::chrono::duration<Rep, Period> interval) {
    _ptr = AUTDControllerBuilderWithSendInterval(_ptr, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(interval).count()));
    return *this;
  }

  template <driver::link_builder B, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] Controller<typename B::Link> open_with_timeout(const B& link_builder, const std::chrono::duration<Rep, Period> timeout) {
    const int64_t timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
    auto runtime = native_methods::AUTDCreateRuntime();
    auto handle = native_methods::AUTDGetRuntimeHandle(runtime);
    auto ptr = validate(AUTDWaitResultController(handle, AUTDControllerOpen(_ptr, link_builder.ptr(), timeout_ns)));
    driver::geometry::Geometry geometry(AUTDGeometry(ptr));
    return Controller<typename B::Link>{std::move(geometry), runtime, handle, ptr,
                                        link_builder.resolve_link(handle, native_methods::AUTDLinkGet(ptr))};
  }

  template <driver::link_builder B>
  AUTD3_API [[nodiscard]] Controller<typename B::Link> open(const B& link_builder) {
    return open_with_timeout(link_builder, std::chrono::nanoseconds(-1));
  }

#ifdef AUTD3_ASYNC_API
  template <driver::link_builder B, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] coro::task<Controller<typename B::Link>> open_with_timeout_async(const B& link_builder,
                                                                                           const std::chrono::duration<Rep, Period> timeout) {
    const int64_t timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
    auto runtime = native_methods::AUTDCreateRuntime();
    auto handle = native_methods::AUTDGetRuntimeHandle(runtime);
    auto future = AUTDControllerOpen(_ptr, link_builder.ptr(), timeout_ns);
    const auto result = co_await wait_future(handle, future);
    auto ptr = validate(result);
    driver::geometry::Geometry geometry(AUTDGeometry(ptr));
    co_return Controller<typename B::Link>{std::move(geometry), runtime, handle, ptr,
                                           link_builder.resolve_link(handle, native_methods::AUTDLinkGet(ptr))};
  }

  template <driver::link_builder B>
  AUTD3_API [[nodiscard]] coro::task<Controller<typename B::Link>> open_async(const B& link_builder) {
    auto cnt = co_await open_with_timeout_async(link_builder, std::chrono::nanoseconds(-1));
    co_return cnt;
  }
#endif

  template <device_range R>
  AUTD3_API explicit ControllerBuilder(R iter) {
    std::vector<native_methods::Vector3> pos;
    std::vector<native_methods::Quaternion> rot;
    for (const auto& device : iter) {
      const auto p = device.position();
      const auto r = device.rotation();
      pos.emplace_back(native_methods::Vector3{p.x(), p.y(), p.z()});
      rot.emplace_back(native_methods::Quaternion{r.x(), r.y(), r.z(), r.w()});
    }
    _ptr = AUTDControllerBuilder(pos.data(), rot.data(), static_cast<uint16_t>(pos.size()));
  }

  AUTD3_API explicit ControllerBuilder(const std::initializer_list<driver::AUTD3> r) : ControllerBuilder(std::vector(r)) {}

 private:
  native_methods::ControllerBuilderPtr _ptr;
};

}  // namespace autd3::controller
