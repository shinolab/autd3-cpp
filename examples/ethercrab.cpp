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
      autd3::link::EtherCrab{[](const uint16_t slave, const autd3::link::Status status) {
                               std::cout << "slave [" << slave << "]: " << status << std::endl;
                               if (status == autd3::link::Status::Lost()) {
                                 // You can also wait for the link to recover, without exiting
                                 // the process
                                 exit(-1);
                               }
                             },
                             autd3::link::EtherCrabOption{}});
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
