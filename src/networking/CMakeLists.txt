add_library(networking
  ClientNetworkManager.hpp
  ClientNetworkManager.cpp
  ServerNetworkManager.hpp
  ServerNetworkManager.cpp
  )

target_link_libraries(networking pthread
  common
  proto
  ${PROTOBUF_LIBRARIES}
  ${RAKNET_LIB})

target_include_directories(networking PUBLIC ${CMAKE_BINARY_DIR}/src/protobuf)