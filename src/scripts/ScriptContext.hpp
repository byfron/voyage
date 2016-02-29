#pragma once

/**
 * Object exposed to the python scripts with all the necessary handlers
 */
struct ScriptContext {

//public:

	ScriptContext() {}	
	ScriptContext(DatabaseConnection::Ptr con, PlayerSession::Ptr ps) {
		_dbCon = con;
		_playerSession = ps;
	}

	int execute(std::string query) {
//		return _dbCon->runQuery(query.c_str());
		std::cout << "query runs!" << std::endl;
		return 0;
	}
	
	DatabaseResult executeSelect(std::string query) {
		return _dbCon->runSelectQuery(query.c_str());
	}
	
//private:
	
	DatabaseConnection::Ptr _dbCon;
	PlayerSession::Ptr _playerSession;	
	
};
