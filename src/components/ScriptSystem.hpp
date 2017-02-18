#include <entities/EventManager.hpp>
#include <entities/System.hpp>
#include <boost/python.hpp>
#include "BodyCmp.hpp"

// Class Exposure
using namespace boost::python;


			
BOOST_PYTHON_MODULE(Context)
{
    class_<EntityManager>("EntityManager")
	    .def("create", &EntityManager::createLocal);

    class_<EventManager>("EventManager");
    
    
};

class ScriptCmp {
public:

	
	ScriptCmp(const std::string & script) : m_script(script) {}

	template <typename ... Components>
	void run(const EntityManager & em, const EventManager &evm, const float & delta, Components && ... comps) {

		try {

		std::string full_path = std::string(SCRIPT_FILE_PATH) + m_script;
		
		boost::python::object main = boost::python::import("__main__");
		boost::python::object global(main.attr("__dict__"));
		boost::python::exec_file(full_path.c_str(), global, global);

		object update_func = main.attr("update");
		update_func(boost::python::ptr(&em),
			    boost::python::ptr(&evm),
			    delta,
			    boost::python::ptr(&comps) ...); 

		}catch(error_already_set) {
			PyErr_Print();
		}
	}

private:
	
	std::string m_script;
	boost::python::object m_update_fn;

};

template <typename ... Components>
class ScriptSystem : public System<ScriptSystem<Components...> > {

public:

	ScriptSystem() {
		// If you're going to use threads: PyEval_InitThreads();
		Py_Initialize();

		m_main = boost::python::object(
			boost::python::handle<>(
				boost::python::borrowed(
					PyImport_AddModule("__main__")
					)));
		initContext();
	};

	~ScriptSystem() {
		Py_Finalize();
	}

	void update(EntityManager & em, EventManager &evm, float delta ) {
	       
		// Update all player states
		em.each<ScriptCmp, Components...>([em, evm, delta, this](Entity entity,
									   ScriptCmp &script,
									   Components & ... comps) {

		  //send list of components
		  script.run<Components...>(em, evm, delta, std::forward<Components>(comps)...);
		  
		  });	
	}

private:

	boost::python::object m_main;

};
