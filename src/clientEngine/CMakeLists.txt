add_library(clientEngine
  ClientEngine.hpp
  ClientEngine.cpp
  )

target_link_libraries(clientEngine
  networking
  pumpkin_engine)
