add_library(components
  CollisionComponent.hpp
  CollisionComponent.cpp
  PlayerSystem.hpp
  PlayerSystem.cpp
  BodyCmp.hpp
  BodyCmp.cpp)
  
add_library(scriptSystem
  ScriptSystem.hpp
  ScriptSystem.cpp
  )
  
target_link_libraries(scriptSystem
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

target_include_directories(scriptSystem PUBLIC
  ${PYTHON_INCLUDE_PATH})
