add_library(visibility STATIC
  Floats.hpp
  Vector.hpp
  Primitives.hpp
  Visibility.hpp
  Visibility.cpp
)
set_target_properties(visibility PROPERTIES LINKER_LANGUAGE CXX)