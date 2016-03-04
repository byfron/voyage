SET(PROTO_FILES
  database.proto
  voyage.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})
target_include_directories(proto PUBLIC ${PROTOBUF_INCLUDE_DIR})

#include the created files in the build sub-directory as a include
#include_directories(${CMAKE_BINARY_DIR}/protobuf)