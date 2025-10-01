target_sources(test_autd3 PRIVATE
  nop.cpp
  audit.cpp
  simulator.cpp
  remote.cpp
  twincat.cpp
)

if(BUILD_LINK_ETHERCRAB_TEST)
  target_sources(test_autd3 PRIVATE
    ethercrab.cpp
  )
endif()
