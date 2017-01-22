MESSAGE(STATUS ${GTEST_INCLUDE_DIRS})

add_library(database
  DatabaseConnectionPool.hpp
  DatabaseConnectionPool.cpp
  DatabaseConnection.hpp
  DatabaseConnection.cpp
  DatabaseResult.hpp
  DatabaseResult.cpp
  )

target_link_libraries(database
  ${SQLITE3_LIBRARY})

if (BUILD_TESTS)
cxx_test(databaseTest databaseTest.cpp database)
endif()
