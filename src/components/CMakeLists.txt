add_library(components
  VisibilityComponent.hpp
  CollisionComponent.hpp
  CollisionComponent.cpp
  PlayerComponent.hpp
  PlayerComponent.cpp
  WeaponSystem.hpp
  WeaponSystem.cpp
  BodyComponent.hpp
  BodyComponent.cpp)

add_library(scriptSystem
  ScriptSystem.hpp
  ScriptSystem.cpp
  )

target_link_libraries(scriptSystem
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

target_include_directories(scriptSystem PUBLIC
  ${PYTHON_INCLUDE_PATH})
