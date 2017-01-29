add_library(game
  GameMap.hpp
  GameMap.cpp
  World.hpp
  World.cpp
  WorldProperties.hpp
  )

target_link_libraries(game
  pumpkin_engine)
