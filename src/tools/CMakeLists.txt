add_executable(fbx_viewer fbx_view_app.cpp)
target_link_libraries(fbx_viewer
  fbxsdk
  dl
  X11
  GL
  ${BGFX_LIBS}
)