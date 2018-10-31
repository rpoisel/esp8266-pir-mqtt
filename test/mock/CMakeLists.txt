add_library(mock STATIC
  Arduino.cpp
  mock.cpp
)

target_include_directories(mock PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)
