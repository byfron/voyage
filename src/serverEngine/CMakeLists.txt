add_library(serverEngine
  GameServer.hpp
  GameServer.cpp
  PlayerSession.hpp
  PlayerSession.cpp
  handlers/ServerLoginHandler.hpp
  handlers/ServerLoginHandler.cpp
  handlers/ServerDataHandler.hpp
  handlers/ServerDataHandler.cpp  
  )

target_link_libraries(serverEngine
  networking
  database
  scriptEngine
  ${PYTHON_LIBRARIES})

target_include_directories(serverEngine PUBLIC ${PYTHON_INCLUDE_PATH})