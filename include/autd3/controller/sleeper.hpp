#pragma once

#include "autd3/native_methods.hpp"
#include "sleeper.hpp"

namespace autd3::controller {

struct StdSleeper {
  std::optional<uint32_t> timer_resolution;

  operator native_methods::SleeperWrap() const {
    return native_methods::SleeperWrap{
        .tag = native_methods::SleeperTag::Std,
        .value = timer_resolution.value_or(0),
        .spin_strategy = native_methods::SpinStrategyTag::SpinLoopHint,
    };
  }
};

struct SpinSleeper {
  uint32_t native_accuracy_ns = native_methods::AUTDSpinSleepDefaultAccuracy();
#ifdef WIN32
  native_methods::SpinStrategyTag spin_strategy = native_methods::SpinStrategyTag::SpinLoopHint;
#else
  native_methods::SpinStrategyTag spin_strategy = native_methods::SpinStrategyTag::YieldThread;
#endif

  operator native_methods::SleeperWrap() const {
    return native_methods::SleeperWrap{
        .tag = native_methods::SleeperTag::Spin,
        .value = native_accuracy_ns,
        .spin_strategy = spin_strategy,
    };
  }
};

struct WaitableSleeper {
  operator native_methods::SleeperWrap() const {
    return native_methods::SleeperWrap{
        .tag = native_methods::SleeperTag::Waitable,
        .value = 0,
        .spin_strategy = native_methods::SpinStrategyTag::SpinLoopHint,
    };
  }
};

}  // namespace autd3::controller
