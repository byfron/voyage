ogre_add_executable(voyage voyage.cpp)
target_link_libraries(voyage graphicsSystem common ${OGRE_LIBRARIES} ${SDL2_LIBRARY})
target_include_directories(voyage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(graphics)