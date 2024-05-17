#pragma once

#include "autd3/native_methods.hpp"

namespace autd3::controller {
class ControllerBuilder;
}

namespace autd3::link {

class Nop {
  AUTD3_API explicit Nop(native_methods::LinkPtr) {}

 public:
  class Builder final {
    friend class Nop;
    friend class controller::ControllerBuilder;

    AUTD3_API Builder() = default;

    AUTD3_API [[nodiscard]] static Nop resolve_link(const native_methods::LinkPtr link) { return Nop{link}; }

   public:
    using Link = Nop;

    AUTD3_API [[nodiscard]] static native_methods::LinkBuilderPtr ptr() { return native_methods::AUTDLinkNop(); }
  };

  static Builder builder() { return {}; }
};
}  // namespace autd3::link
