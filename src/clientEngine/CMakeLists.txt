add_library(clientEngine
  GameClient.hpp
  GameClient.cpp
  )

target_link_libraries(clientEngine networking)