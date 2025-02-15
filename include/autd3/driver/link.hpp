#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::driver {

template <typename T>
concept link = requires(T t) {
  { t.resolve() } -> std::same_as<native_methods::LinkPtr>;
};

}  // namespace autd3::driver
