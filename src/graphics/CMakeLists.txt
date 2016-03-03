add_library(graphics SHARED
  SDL/SDLGraphicsEngine.hpp
  SDL/SDLGraphicsEngine.cpp
  SDL/SDLInputEngine.hpp
  SDL/SDLInputEngine.cpp
  SDL/ImguiInterface.hpp
  SDL/ImguiInterface.cpp
  SDL/TileMapLayer.hpp
  SDL/TileMapLayer.cpp
  SDL/TextureAtlas.hpp
  SDL/TextureAtlas.cpp
  Gui/Gui.hpp
  Gui/Gui.cpp)

MESSAGE(STATUS ${SDL2_INCLUDE_DIR})

target_link_libraries(graphics
  gui
  ${SDL2_LIBRARY}
  ${SDL2IMAGE_LIBRARY}
  ${OPENGL_LIBRARIES}
  GLEW)

target_include_directories(graphics PUBLIC
  ${OPENGL_INCLUDE_DIR}
  ${SDL2_INCLUDE_DIR}
  ${SDL2IMAGE_INCLUDE_DIR})

add_subdirectory(Gui)