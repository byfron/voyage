
# add_executable(voyage_net voyage_net.cpp)
# target_link_libraries(voyage_net networking)
# target_include_directories(voyage_net PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/networking)

# add_executable(server main_server.cpp)
# target_link_libraries(server serverEngine)
# target_include_directories(server PUBLIC
#   ${CMAKE_CURRENT_SOURCE_DIR})

SET(BGFX_LIBS
  "${CMAKE_SOURCE_DIR}/pumpkin/bgfx/.build/linux64_gcc/bin/libexample-commonRelease.a"
  "${CMAKE_SOURCE_DIR}/pumpkin/bgfx/.build/linux64_gcc/bin/libbxRelease.a"
  "${CMAKE_SOURCE_DIR}/pumpkin/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a")

add_executable(client clients/main_client.cpp)
target_link_libraries(client
  clientEngine
  EntitiesCore
  proto
  pumpkin_engine
  dl
  scriptSystem
  gameEngine
  ${BGFX_LIBS}
  )

add_executable(server main_server.cpp)
target_link_libraries(server
  serverEngine
  EntitiesCore
  proto
  gameEngine
  components
  utils
  ${PYTHON_LIBRARIES}
  ${Boost_LIBRARIES}
  )

target_include_directories(client PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SDL2_INCLUDE_DIR})

add_executable(voyage clients/voyage.cpp)

include_directories(.)
include_directories(utils)

add_subdirectory(common)
add_subdirectory(components)
add_subdirectory(entities)
add_subdirectory(networking)
add_subdirectory(clientEngine)
add_subdirectory(serverEngine)
add_subdirectory(visibility)
add_subdirectory(game)
add_subdirectory(database)
add_subdirectory(utils)
add_subdirectory(tools)

#add_subdirectory(scripts)
