// File: simulator.hpp
// Project: link
// Created Date: 27/09/2023
// Author: Shun Suzuki
// -----
// Last Modified: 05/01/2024
// Modified By: Shun Suzuki (suzuki@hapis.k.u-tokyo.ac.jp)
// -----
// Copyright (c) 2023 Shun Suzuki. All rights reserved.
//

#pragma once

#include <chrono>
#include <future>
#include <string>

#include "autd3/internal/geometry/geometry.hpp"
#include "autd3/internal/native_methods.hpp"
#include "autd3/internal/utils.hpp"

namespace autd3::internal {
class ControllerBuilder;
}

namespace autd3::link {

/**
 * @brief Link for AUTD Simulator
 *
 */
class Simulator final {
  internal::native_methods::LinkPtr _ptr;

  explicit Simulator(const internal::native_methods::LinkPtr ptr) : _ptr(ptr) {}

 public:
  class Builder final {
    friend class Simulator;
    friend class internal::ControllerBuilder;

    internal::native_methods::LinkSimulatorBuilderPtr _ptr;

    explicit Builder(const uint16_t port) : _ptr(internal::native_methods::AUTDLinkSimulator(port)) {}

    [[nodiscard]] static Simulator resolve_link(const internal::native_methods::LinkPtr link) { return Simulator{link}; }

   public:
    using Link = Simulator;

    [[nodiscard]] internal::native_methods::LinkBuilderPtr ptr() const { return AUTDLinkSimulatorIntoBuilder(_ptr); }

    /**
     * @brief Set server IP address
     *
     * @param ip Server IP address
     * @return Simulator
     */
    Builder with_server_ip(const std::string& ip) {
      _ptr = validate(AUTDLinkSimulatorWithAddr(_ptr, ip.c_str()));
      return *this;
    }

    template <typename Rep, typename Period>
    Builder with_timeout(const std::chrono::duration<Rep, Period> timeout) {
      const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
      _ptr = AUTDLinkSimulatorWithTimeout(_ptr, static_cast<uint64_t>(ns));
      return *this;
    }
  };

  static Builder builder(const uint16_t port) { return Builder(port); }

  bool update_geometry(const internal::geometry::Geometry& geometry) const {
    return validate(AUTDLinkSimulatorUpdateGeometry(_ptr, geometry.ptr())) == internal::native_methods::AUTD3_TRUE;
  }

#ifdef AUTD3_ASYNC_API
  [[nodiscard]] coro::task<bool> update_geometry_async(const internal::geometry::Geometry& geometry) const { co_return update_geometry(geometry); }
#endif
};

}  // namespace autd3::link
