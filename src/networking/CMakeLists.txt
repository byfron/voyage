add_library(networking
  ClientNetworkManager.hpp
  ClientNetworkManager.cpp
  ServerNetworkManager.hpp
  ServerNetworkManager.cpp
  )

target_link_libraries(networking pthread ${RAKNET_LIB})

