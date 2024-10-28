#pragma once

#include "autd3/controller/controller.hpp"
#include "autd3/controller/timer.hpp"
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
  AUTD3_DEF_PROP(std::vector<driver::AUTD3>, devices)
  AUTD3_DEF_PARAM(ControllerBuilder, uint16_t, fallback_parallel_threshold)
  AUTD3_DEF_PARAM_CHRONO(ControllerBuilder, fallback_timeout)
  AUTD3_DEF_PARAM_CHRONO(ControllerBuilder, send_interval)
  AUTD3_DEF_PARAM_CHRONO(ControllerBuilder, receive_interval)
  AUTD3_DEF_PARAM(ControllerBuilder, native_methods::TimerStrategyWrap, timer_strategy)

  template <driver::link_builder B, typename Rep, typename Period>
  AUTD3_API [[nodiscard]] Controller<typename B::Link> open_with_timeout(const B& link_builder, const std::chrono::duration<Rep, Period> timeout) {
    const int64_t timeout_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
    auto runtime = native_methods::AUTDCreateRuntime();
    auto handle = AUTDGetRuntimeHandle(runtime);

    std::vector<native_methods::Vector3> pos;
    std::vector<native_methods::Quaternion> rot;
    for (const auto& device : _devices) {
      const auto p = device.position();
      const auto r = device.rotation();
      pos.emplace_back(native_methods::Vector3{p.x(), p.y(), p.z()});
      rot.emplace_back(native_methods::Quaternion{r.x(), r.y(), r.z(), r.w()});
    }
    const auto builder = AUTDControllerBuilder(pos.data(), rot.data(), static_cast<uint16_t>(pos.size()), _fallback_parallel_threshold,
                                               static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(_fallback_timeout).count()),
                                               static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(_send_interval).count()),
                                               static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(_receive_interval).count()),
                                               _timer_strategy);
    auto ptr = validate(AUTDWaitResultController(handle, AUTDControllerOpen(builder, link_builder.ptr(), timeout_ns)));
    return Controller<typename B::Link>{AUTDGeometry(ptr), runtime, handle, ptr, link_builder.resolve_link(handle, native_methods::AUTDLinkGet(ptr))};
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

    std::vector<native_methods::Vector3> pos;
    std::vector<native_methods::Quaternion> rot;
    for (const auto& device : _devices) {
      const auto p = device.position();
      const auto r = device.rotation();
      pos.emplace_back(native_methods::Vector3{p.x(), p.y(), p.z()});
      rot.emplace_back(native_methods::Quaternion{r.x(), r.y(), r.z(), r.w()});
    }
    const auto builder = AUTDControllerBuilder(pos.data(), rot.data(), static_cast<uint16_t>(pos.size()), _fallback_parallel_threshold,
                                               static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(_fallback_timeout).count()),
                                               static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(_send_interval).count()),
                                               static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(_receive_interval).count()),
                                               _timer_strategy);

    auto future = AUTDControllerOpen(builder, link_builder.ptr(), timeout_ns);
    const auto result = co_await wait_future(handle, future);
    auto ptr = validate(result);
    co_return Controller<typename B::Link>{AUTDGeometry(ptr), runtime, handle, ptr,
                                           link_builder.resolve_link(handle, native_methods::AUTDLinkGet(ptr))};
  }

  template <driver::link_builder B>
  AUTD3_API [[nodiscard]] coro::task<Controller<typename B::Link>> open_async(const B& link_builder) {
    auto cnt = co_await open_with_timeout_async(link_builder, std::chrono::nanoseconds(-1));
    co_return cnt;
  }
#endif

  template <device_range R>
  AUTD3_API explicit ControllerBuilder(R iter)
      : _devices(iter.begin(), iter.end()),
        _fallback_parallel_threshold(4),
        _fallback_timeout(std::chrono::milliseconds(20)),
        _send_interval(std::chrono::milliseconds(1)),
        _receive_interval(std::chrono::milliseconds(1)),
        _timer_strategy(timer::TimerStrategy::Spin(timer::SpinSleeper())) {}

  AUTD3_API explicit ControllerBuilder(const std::initializer_list<driver::AUTD3> r) : ControllerBuilder(std::vector(r)) {}
};

}  // namespace autd3::controller
