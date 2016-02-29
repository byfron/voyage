
# add_executable(voyage_net voyage_net.cpp)
# target_link_libraries(voyage_net networking)
# target_include_directories(voyage_net PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/networking)

add_executable(server main_server.cpp)
target_link_libraries(server serverEngine)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(client main_client.cpp)
target_link_libraries(client clientEngine)
target_include_directories(client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(voyage voyage.cpp)

include_directories(.)
include_directories(utils)

add_subdirectory(networking)
add_subdirectory(clientEngine)
add_subdirectory(serverEngine)
add_subdirectory(sql)
add_subdirectory(scripts)
  


if (BUILD_VOYAGE3D)
  ogre_add_executable(voyage3d voyage3d.cpp)
  target_link_libraries(voyage3d graphicsSystem common ${OGRE_LIBRARIES} ${SDL2_LIBRARY})
  target_include_directories(voyage3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  add_subdirectory(graphics)
endif()


