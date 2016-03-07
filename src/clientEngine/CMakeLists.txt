add_library(clientEngine
  GameClient.hpp
  GameClient.cpp
  ClientTileMap.hpp
  ClientTileMap.cpp
  )

target_link_libraries(clientEngine
  networking
  graphics)