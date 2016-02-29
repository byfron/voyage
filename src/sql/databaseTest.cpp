#include <gtest/gtest.h>
#include "DatabaseConnection.hpp"
#include "DatabaseConnectionPool.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;
auto console = spd::stdout_logger_mt("Server");

TEST(databaseTest, DatabaseConnection) {

	DatabaseConnection con;
	con.connect(DB_DIR "/test.db");
	DatabaseResult res = con.runSelectQuery("SELECT * FROM ENTITIES");
	
	std::cout << res << std::endl;
	
}

TEST(databasePoolTest, DatabaseConnectionPool) {

	DatabaseConnectionPool pool(10);
	pool.fillPool(DB_DIR "/test.db");

	//TODO:run threads requesting connections		
	DatabaseConnection::Ptr con = pool.requestConnection();
	DatabaseResult res = con->runSelectQuery("SELECT * FROM ENTITIES");
	pool.returnConnection(con);	
	
}

