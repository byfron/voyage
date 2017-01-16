add_library(clientEngine
  ClientEngine.hpp
  ClientEngine.cpp
  ClientTileMap.hpp
  ClientTileMap.cpp
  )

target_link_libraries(clientEngine
  networking
  cabbage)
