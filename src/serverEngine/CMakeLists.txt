add_library(serverEngine
  GameServer.hpp
  GameServer.cpp
  handlers/ServerLoginHandler.hpp
  handlers/ServerLoginHandler.cpp
  handlers/ServerDataHandler.hpp
  handlers/ServerDataHandler.cpp  
  )

target_link_libraries(serverEngine networking)