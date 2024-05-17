#pragma once

#include <chrono>
#include <utility>

#include "autd3/driver/firmware/fpga/loop_behavior.hpp"
#include "autd3/native_methods.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class Audit final {
  native_methods::LinkPtr _ptr;

  AUTD3_API explicit Audit(const native_methods::LinkPtr ptr) : _ptr(ptr) {}

 public:
  class Builder final {
    friend class Audit;
    friend class controller::ControllerBuilder;

    native_methods::LinkAuditBuilderPtr _ptr;

    AUTD3_API Builder() : _ptr(native_methods::AUTDLinkAudit()) {}

    AUTD3_API [[nodiscard]] static Audit resolve_link(const native_methods::LinkPtr link) { return Audit{link}; }

   public:
    using Link = Audit;

    AUTD3_API [[nodiscard]] native_methods::LinkBuilderPtr ptr() const { return AUTDLinkAuditIntoBuilder(_ptr); }

    template <typename Rep, typename Period>
    AUTD3_API Builder with_timeout(const std::chrono::duration<Rep, Period> timeout) {
      const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count();
      _ptr = AUTDLinkAuditWithTimeout(_ptr, static_cast<uint64_t>(ns));
      return std::move(*this);
    }
  };

  AUTD3_API static Builder builder() { return {}; }

  AUTD3_API void down() const { AUTDLinkAuditDown(_ptr); }

  AUTD3_API [[nodiscard]] bool is_open() const { return AUTDLinkAuditIsOpen(_ptr); }

  AUTD3_API [[nodiscard]] std::chrono::nanoseconds timeout() const { return std::chrono::nanoseconds(AUTDLinkAuditTimeoutNs(_ptr)); }

  AUTD3_API [[nodiscard]] std::chrono::nanoseconds last_timeout() const { return std::chrono::nanoseconds(AUTDLinkAuditLastTimeoutNs(_ptr)); }

  AUTD3_API [[nodiscard]] bool is_force_fan(const size_t idx) const { return AUTDLinkAuditFpgaIsForceFan(_ptr, static_cast<uint32_t>(idx)); }

  AUTD3_API void break_down() const { AUTDLinkAuditBreakDown(_ptr); }

  AUTD3_API [[nodiscard]] uint16_t silencer_update_rate_intensity(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerUpdateRateIntensity(_ptr, static_cast<uint32_t>(idx));
  }
  AUTD3_API [[nodiscard]] uint16_t silencer_update_rate_phase(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerUpdateRatePhase(_ptr, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] uint16_t silencer_completion_steps_intensity(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerCompletionStepsIntensity(_ptr, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] uint16_t silencer_completion_steps_phase(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerCompletionStepsPhase(_ptr, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] bool silencer_fixed_completion_steps_mode(const size_t idx) const {
    return AUTDLinkAuditFpgaSilencerFixedCompletionStepsMode(_ptr, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] std::array<uint8_t, 4> debug_types(const size_t idx) const {
    std::array<uint8_t, 4> ty{};
    AUTDLinkAuditFpgaDebugTypes(_ptr, static_cast<uint32_t>(idx), ty.data());
    return ty;
  }

  AUTD3_API [[nodiscard]] std::array<uint16_t, 4> debug_values(const size_t idx) const {
    std::array<uint16_t, 4> value{};
    AUTDLinkAuditFpgaDebugValues(_ptr, static_cast<uint32_t>(idx), value.data());
    return value;
  }

  AUTD3_API void assert_thermal_sensor(const size_t idx) const { AUTDLinkAuditFpgaAssertThermalSensor(_ptr, static_cast<uint32_t>(idx)); }

  AUTD3_API void deassert_thermal_sensor(const size_t idx) const { AUTDLinkAuditFpgaDeassertThermalSensor(_ptr, static_cast<uint32_t>(idx)); }

  AUTD3_API [[nodiscard]] std::vector<autd3::driver::EmitIntensity> modulation(const size_t idx, const native_methods::Segment segment) const {
    const auto n = AUTDLinkAuditFpgaModulationCycle(_ptr, segment, static_cast<uint32_t>(idx));
    std::vector<autd3::driver::EmitIntensity> buf(n);
    AUTDLinkAuditFpgaModulation(_ptr, segment, static_cast<uint32_t>(idx), reinterpret_cast<const uint8_t *>(buf.data()));
    return buf;
  }

  AUTD3_API [[nodiscard]] uint32_t modulation_freq_division(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaModulationFreqDivision(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] native_methods::LoopBehavior modulation_loop_behavior(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaModulationLoopBehavior(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] std::pair<std::vector<autd3::driver::Drive>> drives(const size_t idx, const native_methods::Segment segment,
                                                                              const int stm_idx) const {
    const auto n = AUTDLinkAuditCpuNumTransducers(_ptr, static_cast<uint32_t>(idx));
    std::vector<std::uint8_t> duties(n);
    std::vector<std::uint8_t> phases(n);
    AUTDLinkAuditFpgaDrives(_ptr, segment, static_cast<uint32_t>(idx), static_cast<uint32_t>(stm_idx), duties.data(), phases.data());
    return std::make_pair(duties, phases);
  }

  AUTD3_API [[nodiscard]] uint32_t stm_cycle(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmCycle(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] bool is_stm_gain_mode(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaIsStmGainMode(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] uint32_t stm_freq_division(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmFreqDivision(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] native_methods::LoopBehavior stm_loop_behavior(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaStmLoopBehavior(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] native_methods::Segment current_stm_segment(const size_t idx) const {
    return AUTDLinkAuditFpgaCurrentStmSegment(_ptr, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] uint32_t stm_sound_speed(const size_t idx, const native_methods::Segment segment) const {
    return AUTDLinkAuditFpgaSoundSpeed(_ptr, segment, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] native_methods::Segment current_mod_segment(const size_t idx) const {
    return AUTDLinkAuditFpgaCurrentModSegment(_ptr, static_cast<uint32_t>(idx));
  }

  AUTD3_API [[nodiscard]] std::vector<autd3::driver::Phase> phase_filter(const size_t idx) const {
    const auto n = AUTDLinkAuditCpuNumTransducers(_ptr, static_cast<uint32_t>(idx));
    std::vector<autd3::driver::Phase> buf(n);
    AUTDLinkAuditFpgaPhaseFilter(_ptr, static_cast<uint32_t>(idx), reinterpret_cast<const uint8_t *>(buf.data()));
    return buf;
  }
};

}  // namespace autd3::link
