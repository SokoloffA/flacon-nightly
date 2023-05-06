list(APPEND SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/preferencesdialog.h
  ${CMAKE_CURRENT_LIST_DIR}/preferencesdialog.cpp
  ${CMAKE_CURRENT_LIST_DIR}/preferencesdialog.ui
)

include(${CMAKE_CURRENT_LIST_DIR}/profilespage/module.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/generalpage/module.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/updatepage/module.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/programspage/module.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR})
