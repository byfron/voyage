add_library(gameEngine
  GameEngine.hpp
  GameEngine.cpp
  CollisionManager.hpp
  CollisionManager.cpp
  World.hpp
  World.cpp
  WorldProperties.hpp
  GameLevel.hpp
  GameLevel.cpp
  )

target_link_libraries(gameEngine
  components
  utils)
