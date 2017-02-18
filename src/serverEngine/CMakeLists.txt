add_library(serverEngine
  GameServer.hpp
  GameServer.cpp
  PlayerSession.hpp
  PlayerSession.cpp
  ServerEngine.hpp
  ServerEngine.cpp
  handlers/ServerLoginHandler.hpp
  handlers/ServerLoginHandler.cpp
  handlers/ServerDataHandler.hpp
  handlers/ServerDataHandler.cpp
  handlers/ServerMapHandler.hpp
  handlers/ServerMapHandler.cpp
  )

target_link_libraries(serverEngine
  networking
  proto)

target_include_directories(serverEngine PUBLIC ${PYTHON_INCLUDE_PATH} ${CMAKE_BINARY_DIR}/src/protobuf)
