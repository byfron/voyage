add_library(scriptSystem
  ScriptSystem.hpp
  ScriptSystem.cpp
  )


target_link_libraries(scriptSystem
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

target_include_directories(scriptSystem PUBLIC
  ${PYTHON_INCLUDE_PATH})
