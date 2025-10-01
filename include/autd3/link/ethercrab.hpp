#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "autd3/controller/sleeper.hpp"
#include "autd3/native_methods.hpp"
#include "autd3/native_methods/utils.hpp"

namespace autd3::link {

static void tracing_init() { native_methods::AUTDLinkEtherCrabTracingInit(); }

class Status {
  native_methods::Status _inner;
  std::string _msg;

  explicit Status(const native_methods::Status inner, std::string msg) : _inner(inner), _msg(std::move(msg)) {}

 public:
  friend struct EtherCrab;

  static Status Lost() { return Status(native_methods::Status::Lost, ""); }
  static Status Error() { return Status(native_methods::Status::Error, ""); }
  static Status StateChanged() { return Status(native_methods::Status::StateChanged, ""); }
  static Status Resumed() { return Status(native_methods::Status::Resumed, ""); }

  bool operator==(const Status& that) const { return _inner == that._inner && _msg == that._msg; }

  friend std::ostream& operator<<(std::ostream& os, const Status& s);
};

inline std::ostream& operator<<(std::ostream& os, const Status& s) {
  os << s._msg;
  return os;
}

struct ThreadPriority {
  AUTD3_API static ThreadPriority Min() { return ThreadPriority(native_methods::AUTDLinkEtherCrabThreadPriorityMin()); }
  AUTD3_API static ThreadPriority Max() { return ThreadPriority(native_methods::AUTDLinkEtherCrabThreadPriorityMax()); }
  AUTD3_API [[nodiscard]] static ThreadPriority Crossplarform(const uint8_t value) {
    if (value > 99) throw std::invalid_argument("value must be between 0 and 99");
    return ThreadPriority(native_methods::AUTDLinkEtherCrabThreadPriorityCrossplatform(value));
  }

  operator native_methods::ThreadPriorityPtr() const { return _ptr; }

 private:
  explicit ThreadPriority(const native_methods::ThreadPriorityPtr ptr) : _ptr(ptr) {}

  native_methods::ThreadPriorityPtr _ptr;
};

struct CoreId {
  uint32_t id;
};

template <class F>
concept ethercrab_err_handler_f = requires(F f, const uint16_t slave, const Status status) {
  { f(slave, status) } -> std::same_as<void>;
};

struct EtherCrabOption {
  std::optional<std::string> ifname = std::nullopt;
  size_t buf_size = 16;
  std::chrono::nanoseconds state_check_period = std::chrono::milliseconds(100);
  std::chrono::nanoseconds sync0_period = std::chrono::milliseconds(2);
  std::chrono::nanoseconds sync_tolerance = std::chrono::microseconds(1);
  std::chrono::nanoseconds sync_timeout = std::chrono::seconds(10);
  std::optional<ThreadPriority> tx_rx_thread_builder = std::nullopt;
  std::optional<CoreId> tx_rx_thread_affinity = std::nullopt;
  std::optional<ThreadPriority> main_thread_builder = std::nullopt;
  std::optional<CoreId> main_thread_affinity = std::nullopt;

  operator native_methods::EtherCrabOption() const {
    return native_methods::EtherCrabOption{
        .ifname = ifname.has_value() ? ifname.value().c_str() : nullptr,
        .buf_size = static_cast<uint32_t>(buf_size),
        .timeouts_state_transition = native_methods::to_duration(std::chrono::seconds(10)),
        .timeouts_pdu = native_methods::to_duration(std::chrono::milliseconds(100)),
        .timeouts_eeprom = native_methods::to_duration(std::chrono::milliseconds(10)),
        .timeouts_wait_loop_delay = native_methods::to_duration(std::chrono::milliseconds(0)),
        .timeouts_mailbox_echo = native_methods::to_duration(std::chrono::milliseconds(100)),
        .timeouts_mailbox_response = native_methods::to_duration(std::chrono::milliseconds(1000)),
        .main_device_config_dc_static_sync_iterations = 10000,
        .main_device_config_retry_behaviour = 0,
        .dc_configuration_start_delay = native_methods::to_duration(std::chrono::milliseconds(100)),
        .dc_configuration_sync0_period = native_methods::to_duration(sync0_period),
        .dc_configuration_sync0_shift = native_methods::to_duration(std::chrono::microseconds(0)),
        .state_check_period = native_methods::to_duration(state_check_period),
        .sync_tolerance = native_methods::to_duration(sync_tolerance),
        .sync_timeout = native_methods::to_duration(sync_timeout),
        .tx_rx_thread_builder = tx_rx_thread_builder.has_value() ? static_cast<native_methods::ThreadPriorityPtr>(tx_rx_thread_builder.value())
                                                                 : native_methods::ThreadPriorityPtr{nullptr},
        .tx_rx_thread_affinity = tx_rx_thread_affinity.has_value() ? static_cast<int32_t>(tx_rx_thread_affinity->id) : -1,
        .main_thread_builder = main_thread_builder.has_value() ? static_cast<native_methods::ThreadPriorityPtr>(main_thread_builder.value())
                                                               : native_methods::ThreadPriorityPtr{nullptr},
        .main_thread_affinity = main_thread_affinity.has_value() ? static_cast<int32_t>(main_thread_affinity->id) : -1};
  }
};

struct EtherCrab final {
  using err_handler_t = void (*)(uint16_t, Status);

  explicit EtherCrab(const err_handler_t err_handler, const EtherCrabOption option) : err_handler(err_handler), option(option) {
    _native_err_handler = +[](const void* context, const uint32_t slave, const native_methods::Status status) {           // LCOV_EXCL_LINE
      const std::string msg(128, ' ');                                                                                    // LCOV_EXCL_LINE
      (void)AUTDLinkEtherCrabStatusGetMsg(status, const_cast<char*>(msg.c_str()));                                        // LCOV_EXCL_LINE
      (*reinterpret_cast<err_handler_t>(const_cast<void*>(context)))(static_cast<uint16_t>(slave), Status(status, msg));  // LCOV_EXCL_LINE
    };  // LCOV_EXCL_LINE
  }

  [[nodiscard]] native_methods::LinkPtr resolve() const {
    return validate(native_methods::AUTDLinkEtherCrab(reinterpret_cast<void*>(_native_err_handler), reinterpret_cast<void*>(err_handler), option));
  }

  err_handler_t err_handler;
  EtherCrabOption option;

 private:
  using native_err_handler_t = void (*)(const void*, uint32_t, native_methods::Status);
  native_err_handler_t _native_err_handler;
};

}  // namespace autd3::link
