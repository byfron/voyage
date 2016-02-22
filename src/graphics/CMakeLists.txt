add_library(graphicsSystem SHARED
  GraphicsSystem.h
  GraphicsSystem.cpp
  )

target_link_libraries(graphicsSystem ${OGRE_LIBRARIES})

target_include_directories(graphicsSystem PUBLIC ../common)