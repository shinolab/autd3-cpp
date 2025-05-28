target_sources(test_autd3 PRIVATE
  firmware_version.cpp
  autd3_device.cpp
)

add_subdirectory(firmware)
add_subdirectory(common)
add_subdirectory(datagram)
add_subdirectory(geometry)
add_subdirectory(ethercat)
