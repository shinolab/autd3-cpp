#pragma once

#include <algorithm>
#include <array>
#include <autd3/driver/firmware/fpga/drive.hpp>
#include <chrono>
#include <utility>

#include "autd3/native_methods.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class Audit final {
  native_methods::LinkPtr _ptr;

  explicit Audit(const native_methods::LinkPtr ptr) : _ptr(ptr) {}

 public:
  class Builder final {
    friend class Audit;
    friend class controller::ControllerBuilder;

    native_methods::LinkAuditBuilderPtr _ptr;

    Builder() : _ptr(native_methods::AUTDLinkAudit()) {}

    [[nodiscard]] static Audit resolve_link(native_methods::HandlePtr, const native_methods::LinkPtr link) { return Audit{link}; }

   public:
    using Link = Audit;

    [[nodiscard]] native_methods::LinkBuilderPtr ptr() const { return AUTDLinkAuditIntoBuilder(_ptr); }

    template <typename Rep, typename Period>
    Builder with_timeout(const std::chrono::duration<Rep, Period> timeout) {
      const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
      _ptr = AUTDLinkAuditWithTimeout(_ptr, static_cast<uint64_t>(ns));
      return std::move(*this);
    }
  };

  static Builder builder() { return {}; }

  void down() const { AUTDLinkAuditDown(_ptr); }
  void up() const { AUTDLinkAuditUp(_ptr); }

  [[nodiscard]] bool is_open() const { return AUTDLinkAuditIsOpen(_ptr); }

  [[nodiscard]] std::chrono::nanoseconds timeout() const { return std::chrono::nanoseconds(AUTDLinkAuditTimeoutNs(_ptr)); }

  [[nodiscard]] std::optional<std::chrono::nanoseconds> last_timeout() const {
    const auto ns = AUTDLinkAuditLastTimeoutNs(_ptr);
    return ns < 0 ? std::nullopt : std::optional(std::chrono::nanoseconds(ns));
  }

  [[nodiscard]] uint64_t last_parallel_threshold() const { return AUTDLinkAuditLastParallelThreshold(_ptr); }

  [[nodiscard]] bool is_force_fan(const size_t idx) const { return AUTDLinkAuditFpgaIsForceFan(_ptr, static_cast<uint16_t>(idx)); }

  void break_down() const { AUTDLinkAuditBreakDown(_ptr); }
  void repair() const { AUTDLinkAuditRepair(_ptr); }

  [[nodiscard]] uint16_t silencer_update_rate_intensity(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerUpdateRateIntensity(_ptr, static_cast<uint16_t>(idx));
  }
  [[nodiscard]] uint16_t silencer_update_rate_phase(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerUpdateRatePhase(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint16_t silencer_completion_steps_intensity(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerCompletionStepsIntensity(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint16_t silencer_completion_steps_phase(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerCompletionStepsPhase(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] bool silencer_fixed_completion_steps_mode(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerFixedCompletionStepsMode(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] bool silencer_strict_mode(const size_t idx) const { return AUTDLinkAuditCpuSilencerStrictMode(_ptr, static_cast<uint16_t>(idx)); }

  [[nodiscard]] native_methods::SilencerTarget silencer_target(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerTarget(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] std::array<uint8_t, 4> debug_types(const size_t idx) const {
    std::array<uint8_t, 4> ty{};
    AUTDLinkAuditFpgaDebugTypes(_ptr, static_cast<uint16_t>(idx), ty.data());
    return ty;
  }

  [[nodiscard]] std::array<uint64_t, 4> debug_values(const size_t idx) const {
    std::array<uint64_t, 4> value{};
    AUTDLinkAuditFpgaDebugValues(_ptr, static_cast<uint16_t>(idx), value.data());
    return value;
  }

  void assert_thermal_sensor(const size_t idx) const { AUTDLinkAuditFpgaAssertThermalSensor(_ptr, static_cast<uint16_t>(idx)); }

  void deassert_thermal_sensor(const size_t idx) const { AUTDLinkAuditFpgaDeassertThermalSensor(_ptr, static_cast<uint16_t>(idx)); }

  [[nodiscard]] std::vector<uint8_t> modulation(const size_t idx, const native_methods::Segment segment) const {
    const auto n = AUTDLinkAuditFpgaModulationCycle(_ptr, segment, static_cast<uint16_t>(idx));
    std::vector<uint8_t> buf(n);
    AUTDLinkAuditFpgaModulationBuffer(_ptr, segment, static_cast<uint16_t>(idx), buf.data(), n);
    return buf;
  }

  [[nodiscard]] uint32_t modulation_freq_division(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaModulationFreqDivision(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] native_methods::LoopBehavior modulation_loop_behavior(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaModulationLoopBehavior(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] std::vector<driver::Drive> drives(const size_t idx, const native_methods::Segment segment, const int stm_idx) const {
    const auto n = AUTDLinkAuditCpuNumTransducers(_ptr, static_cast<uint16_t>(idx));
    std::vector<driver::Drive> drives(n);
    AUTDLinkAuditFpgaDrivesAt(_ptr, segment, static_cast<uint16_t>(idx), static_cast<uint16_t>(stm_idx),
                              reinterpret_cast<native_methods::Drive*>(drives.data()));
    return drives;
  }

  [[nodiscard]] uint32_t stm_cycle(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmCycle(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] bool is_stm_gain_mode(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaIsStmGainMode(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint32_t stm_freq_division(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmFreqDivision(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] native_methods::LoopBehavior stm_loop_behavior(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmLoopBehavior(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] native_methods::Segment current_stm_segment(const size_t idx) const {
    return AUTDLinkAuditFpgaCurrentStmSegment(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] uint32_t stm_sound_speed(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaSoundSpeed(_ptr, segment, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] native_methods::Segment current_mod_segment(const size_t idx) const {
    return AUTDLinkAuditFpgaCurrentModSegment(_ptr, static_cast<uint16_t>(idx));
  }

  [[nodiscard]] std::vector<uint8_t> pulse_width_encoder_table(const size_t idx) const {
    std::vector<uint8_t> buf(256);
    AUTDLinkAuditFpgaPulseWidthEncoderTable(_ptr, static_cast<uint16_t>(idx), buf.data());
    return buf;
  }
};

}  // namespace autd3::link
