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

cxx_test(databaseTest databaseTest.cpp
  database
  gtest_main)
