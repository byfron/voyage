#include "ScriptEngine.hpp"
#include "ScriptContext.hpp"
#include <sql/DatabaseConnection.hpp>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace boost::python;

typedef boost::shared_ptr<ScriptContext> context_ptr;

BOOST_PYTHON_MODULE(scontext)
{
        class_<ScriptContext, context_ptr >("ScriptContext")
		.def("execute", &ScriptContext::execute)
		.def("executeSelect", &ScriptContext::executeSelect);
}

ScriptEngine::ScriptEngine(DatabaseConnectionPool::Ptr pool) : _dbPool(pool) {
	init();
}

ScriptEngine::~ScriptEngine() {
	 Py_Finalize();
}

void ScriptEngine::init() {
	// If you're going to use threads: PyEval_InitThreads();
        Py_Initialize();
	setupContext();
}

void ScriptEngine::setupContext() {

	try {
		PyRun_SimpleString(
 			"script_context = None\n"
			"\n"
			"def setup(script_context_cxx):\n"
			"    global script_context\n"
			"    script_context = script_context_cxx\n"
			"    script_context.execute('a')\n"
			"\n"
			);
		
	// Retrieve the main module.
		_main = boost::python::object(
			boost::python::handle<>(
				boost::python::borrowed(
					PyImport_AddModule("__main__")
					)));

		_context = _main.attr("setup");
		initscontext();
	}
	catch(error_already_set) {
		PyErr_Print();
	}
	
}

void ScriptEngine::runScript(PlayerSession::Ptr ps, const std::string & script) {

	DatabaseConnection::Ptr con = _dbPool->requestConnection();
	context_ptr cptr = boost::make_shared<ScriptContext>(con, ps);	
	boost::python::object global(_main.attr("__dict__"));
	
	_context(cptr);
	boost::python::object result = boost::python::exec_file(script.c_str(), global, global);
	
	
	_dbPool->returnConnection(con);
	
}
