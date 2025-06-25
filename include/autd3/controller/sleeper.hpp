#pragma once

#include "autd3/native_methods.hpp"
#include "sleeper.hpp"

namespace autd3::controller {

struct StdSleeper {
  operator native_methods::SleeperWrap() const {
    return native_methods::SleeperWrap{
        .tag = native_methods::SleeperTag::Std,
        .value = 0,
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

struct SpinWaitSleeper {
  operator native_methods::SleeperWrap() const {
    return native_methods::SleeperWrap{
        .tag = native_methods::SleeperTag::SpinWait,
        .value = 0,
        .spin_strategy = native_methods::SpinStrategyTag::SpinLoopHint,
    };
  }
};

}  // namespace autd3::controller
