#pragma once

#include <vector>
#include <memory>
#include <sqlite3.h>
#include "DatabaseResult.hpp"

class DatabaseConnection {
public:
	DatabaseConnection();
	int connect(const std::string & db_name);
	void disconnect();
	int runQuery(const char* query);
	DatabaseResult runSelectQuery(const char* query);	
	void push_row(std::vector<std::string>);
	typedef std::shared_ptr<DatabaseConnection> Ptr;
private:
	sqlite3 *_db;
};


