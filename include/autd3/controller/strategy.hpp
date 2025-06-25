#pragma once

#include <variant>

#include "autd3/native_methods.hpp"
#include "sleeper.hpp"

namespace autd3::controller {

struct FixedSchedule {
  std::variant<StdSleeper, SpinSleeper, SpinWaitSleeper> sleeper = SpinSleeper{};

  operator native_methods::TimerStrategyWrap() const {
    return native_methods::TimerStrategyWrap{
        .tag = native_methods::TimerStrategyTag::FixedSchedule,
        .sleep = std::visit([](const auto& s) { return native_methods::SleeperWrap(s); }, sleeper),
    };
  }
};

struct FixedDelay {
  std::variant<StdSleeper, SpinSleeper, SpinWaitSleeper> sleeper;

  operator native_methods::TimerStrategyWrap() const {
    return native_methods::TimerStrategyWrap{
        .tag = native_methods::TimerStrategyTag::FixedDelay,
        .sleep = std::visit([](const auto& s) { return native_methods::SleeperWrap(s); }, sleeper),
    };
  }
};

}  // namespace autd3::controller
