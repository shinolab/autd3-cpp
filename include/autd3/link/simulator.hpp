#pragma once

#include <chrono>
#include <string>

#ifdef AUTD3_ASYNC_API
#include <coro/coro.hpp>
#endif

#include "autd3/driver/geometry/geometry.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class Simulator final {
  native_methods::RuntimePtr _runtime;
  native_methods::LinkPtr _ptr;

  explicit Simulator(const native_methods::RuntimePtr runtime, const native_methods::LinkPtr ptr) : _runtime(runtime), _ptr(ptr) {}

 public:
  class Builder final {
    friend class Simulator;
    friend class controller::ControllerBuilder;

    native_methods::LinkSimulatorBuilderPtr _ptr;

    AUTD3_API explicit Builder(const uint16_t port) : _ptr(native_methods::AUTDLinkSimulator(port)) {}

    [[nodiscard]] static Simulator resolve_link(const native_methods::RuntimePtr runtime, const native_methods::LinkPtr link) {
      return Simulator{runtime, link};
    }

   public:
    using Link = Simulator;

    [[nodiscard]] native_methods::LinkBuilderPtr ptr() const { return AUTDLinkSimulatorIntoBuilder(_ptr); }

    AUTD3_API [[nodiscard]] Builder with_server_ip(const std::string& ip) {
      _ptr = validate(AUTDLinkSimulatorWithAddr(_ptr, ip.c_str()));
      return *this;
    }

    template <typename Rep, typename Period>
    AUTD3_API [[nodiscard]] Builder with_timeout(const std::chrono::duration<Rep, Period> timeout) {
      const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
      _ptr = AUTDLinkSimulatorWithTimeout(_ptr, static_cast<uint64_t>(ns));
      return *this;
    }
  };

  AUTD3_API [[nodiscard]] static Builder builder(const uint16_t port) { return Builder(port); }

  [[nodiscard]] bool update_geometry(const driver::geometry::Geometry& geometry) const {
    return validate(AUTDWaitResultI32(_runtime, AUTDLinkSimulatorUpdateGeometry(_ptr, geometry.ptr()))) == native_methods::AUTD3_TRUE;
  }

#ifdef AUTD3_ASYNC_API
  [[nodiscard]] coro::task<bool> update_geometry_async(const driver::geometry::Geometry& geometry) const {
    const auto future = AUTDLinkSimulatorUpdateGeometry(_ptr, geometry.ptr());
    const auto result = co_await wait_future(_runtime, future);
    co_return validate(result) == native_methods::AUTD3_TRUE;
  }
#endif
};

}  // namespace autd3::link
