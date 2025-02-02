#include "autd3/link/twincat.hpp"

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

int main() try {
  auto autd = autd3::ControllerBuilder(std::vector{autd3::AUTD3(autd3::Point3::origin())}).open(autd3::link::TwinCAT::builder());
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
