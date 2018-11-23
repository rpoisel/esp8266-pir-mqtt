add_subdirectory(lib)
add_subdirectory(mock)

add_library(main STATIC
  ${CMAKE_SOURCE_DIR}/src/components.cpp
)

target_include_directories(main PUBLIC
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main PUBLIC
  mock
)

add_executable(TimingApp
  timing.cpp
)

target_link_libraries(TimingApp PUBLIC
  mock
  Catch2
  main
)

add_test(NAME Timing COMMAND TimingApp)
