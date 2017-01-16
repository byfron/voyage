add_library(scriptEngine
  ScriptEngine.hpp
  ScriptEngine.cpp
  ScriptContext.hpp
  )

target_link_libraries(scriptEngine
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

target_include_directories(scriptEngine PUBLIC
  ${PYTHON_INCLUDE_PATH})

add_executable(scriptTest scriptEngineTest.cpp)
target_link_libraries(scriptTest
  scriptEngine
  serverEngine
  database
  networking
  gtest_main)
