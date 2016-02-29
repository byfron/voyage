#include <serverEngine/GameServer.hpp>
#include <spdlog/spdlog.h>

namespace spd = spdlog;

int main()
{
	auto console = spd::stdout_logger_mt("Server");
	GameServer server(1234, 100);
	server.start();   
}
