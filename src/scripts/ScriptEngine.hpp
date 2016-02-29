#pragma once

#include <string>
#include <sql/DatabaseConnectionPool.hpp>
#include <boost/python.hpp>
#include <serverEngine/PlayerSession.hpp>

class ScriptEngine {
public:

	ScriptEngine(DatabaseConnectionPool::Ptr pool);
	~ScriptEngine();
	void init();
	void setupContext();
	void runScript(PlayerSession::Ptr ps, const std::string & script);

	typedef std::shared_ptr<ScriptEngine> Ptr;
private:

	DatabaseConnectionPool::Ptr _dbPool;
	boost::python::object _main;
	boost::python::object _context;
};
