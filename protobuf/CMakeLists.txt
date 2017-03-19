SET(PROTO_FILES
  texture_atlas.proto
  mesh.proto
  scene.proto
  shader.proto
  animation.proto
  graphics.proto
  database.proto
  tilemap.proto
  voyage.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTO_FILES})
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})
target_include_directories(proto PUBLIC ${PROTOBUF_INCLUDE_DIR})
