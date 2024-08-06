#pragma once

#include <chrono>
#include <string>

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

    AUTD3_API explicit Builder(const std::string& ip) : _ptr(validate(native_methods::AUTDLinkSimulator(ip.c_str()))) {}

    [[nodiscard]] static Simulator resolve_link(const native_methods::RuntimePtr runtime, const native_methods::LinkPtr link) {
      return Simulator{runtime, link};
    }

   public:
    using Link = Simulator;

    [[nodiscard]] native_methods::LinkBuilderPtr ptr() const { return AUTDLinkSimulatorIntoBuilder(_ptr); }

    template <typename Rep, typename Period>
    AUTD3_API [[nodiscard]] Builder with_timeout(const std::chrono::duration<Rep, Period> timeout) {
      const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
      _ptr = AUTDLinkSimulatorWithTimeout(_ptr, static_cast<uint64_t>(ns));
      return *this;
    }
  };

  AUTD3_API [[nodiscard]] static Builder builder(const std::string& ip) { return Builder(ip); }
};

}  // namespace autd3::link
