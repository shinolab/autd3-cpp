#include "autd3/link/soem.hpp"

#include <iostream>

#include "autd3.hpp"
#include "runner.hpp"
#include "util.hpp"

int main() try {
  auto autd =
      autd3::ControllerBuilder(std::vector{autd3::AUTD3(autd3::Vector3::Zero())})
          .open(autd3::link::SOEM::builder().with_err_handler([](const uint16_t slave, const autd3::link::Status status, const std::string& msg) {
            switch (status) {
              case autd3::link::Status::Error:
                std::cout << "Error [" << slave << "]: " << msg << std::endl;
                break;
              case autd3::link::Status::Lost:
                std::cout << "Lost [" << slave << "]: " << msg << std::endl;
                // You can also wait for the link to recover,
                // without exiting the process
                exit(-1);
              case autd3::link::Status::StateChanged:
                std::cout << "StateChanged [" << slave << "]: " << msg << std::endl;
                break;
            }
          }));
  run(autd);
  return 0;
} catch (std::exception& e) {
  print_err(e);
  return -1;
}
