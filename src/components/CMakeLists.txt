SET (COMPONENTS_SRC
  VisibilityComponent.hpp
  CollisionComponent.hpp
  CollisionComponent.cpp
  PlayerComponent.hpp
  PlayerComponent.cpp
  WeaponSystem.hpp
  WeaponSystem.cpp
  BodyComponent.hpp
  BodyComponent.cpp)

add_library(components_client
  GraphicsComponent.hpp
  ParticleComponent.hpp
  ${COMPONENTS_SRC})

add_library(components_server
  ${COMPONENTS_SRC})

add_library(scriptSystem
  ScriptSystem.hpp
  ScriptSystem.cpp
  )

target_compile_definitions(components_client PUBLIC CLIENT=1)

target_link_libraries(scriptSystem
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARIES})

target_include_directories(scriptSystem PUBLIC
  ${PYTHON_INCLUDE_PATH})
