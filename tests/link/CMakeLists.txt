target_sources(test_autd3 PRIVATE
  nop.cpp
  audit.cpp
  simulator.cpp
  soem.cpp
  twincat.cpp
)

if(${AUTD3_LINK_VISUALIZER_BUILD})
  target_sources(test_autd3 PRIVATE
    visualizer.cpp
  )
endif()

