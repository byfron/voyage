#pragma once

#include <string>
#include <sql/DatabaseConnectionPool.hpp>
#include <boost/python.hpp>

class PlayerSession;
typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;

class ScriptEngine {
public:

	void init(DatabaseConnectionPool::Ptr pool);
	void finalize();
	void runScript(PlayerSessionPtr ps, const std::string & script);

	typedef std::shared_ptr<ScriptEngine> Ptr;
private:
	void _setupContext();
	DatabaseConnectionPool::Ptr _dbPool;
	boost::python::object _main;
	boost::python::object _context;
};
