#include "autd3/link/ethercrab.hpp"

#include <stdlib.h>

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

int main() try {
#ifdef WIN32
  _putenv_s("RUST_LOG", "autd3=INFO");
#else
  setenv("RUST_LOG", "autd3=INFO", false);
#endif

  autd3::link::tracing_init();

  auto autd = autd3::Controller::open(
      std::vector{autd3::AUTD3{.pos = autd3::Point3::origin(), .rot = autd3::Quaternion::Identity()},
                  autd3::AUTD3{.pos = autd3::Point3(autd3::AUTD3::DEVICE_WIDTH, 0.0, 0.0), .rot = autd3::Quaternion::Identity()}},
      autd3::link::EtherCrab{
          [](const uint16_t idx, const autd3::link::Status status) { std::cout << "Device[" << idx << "]: " << status << std::endl; },
          autd3::link::EtherCrabOption{}});
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
