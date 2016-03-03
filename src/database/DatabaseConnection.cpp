#include "DatabaseConnection.hpp"
#include "DatabaseResult.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;

DatabaseConnection::DatabaseConnection() {

}

void DatabaseConnection::disconnect() {
	sqlite3_close(_db);
}

int DatabaseConnection::connect(const std::string & db_name) {

	int rc = sqlite3_open(db_name.c_str(), &_db);
	
	if(rc) {
		spd::get("Server")->info() << "Can't open database: " << 
			sqlite3_errmsg(_db);
		return -1;
	}else{
		spd::get("Server")->info() << "Opened database successfully";
		return 0;
	}
}

int DatabaseConnection::runQuery(const char* query) {
	//mark as dirty when modified	
}

DatabaseResult DatabaseConnection::runSelectQuery(const char* query) {

	 DatabaseResult  db_result;
	 sqlite3_stmt *statement;
	 if(sqlite3_prepare_v2(_db, query, -1, &statement, 0) == SQLITE_OK)
	 {
		 int cols = sqlite3_column_count(statement);
		 int result = 0;
		 while(true)
		 {
			 result = sqlite3_step(statement);
			 if(result == SQLITE_ROW)
			 {
				 std::vector<std::string> values;
				 for(int col = 0; col < cols; col++)
				 {
					 std::string  val;
					 char * ptr = (char*)sqlite3_column_text(statement, col);
					 if(ptr)
					 {
						 val = ptr;
					 }
					 else val = "";
					 values.push_back(val);
				 }
				 db_result.push_row(values);
			 }
			 else
			 {
				 break;
			 }
		 }
		 sqlite3_finalize(statement);
	 }
	 return db_result; 
}
