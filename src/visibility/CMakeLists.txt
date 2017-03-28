add_library(visibility STATIC
  Floats.hpp
  Vector.hpp
  Primitives.hpp
  Visibility.hpp
  Visibility.cpp
        VisibilityManager.cpp VisibilityManager.hpp)
set_target_properties(visibility PROPERTIES LINKER_LANGUAGE CXX)