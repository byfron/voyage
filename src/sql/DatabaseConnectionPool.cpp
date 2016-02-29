#include "DatabaseConnectionPool.hpp"
#include <spdlog/spdlog.h>

namespace spd = spdlog;

DatabaseConnectionPool::DatabaseConnectionPool(unsigned int pool_size) {
	_size = pool_size;

	for (int i = 0; i < _size; i++)
		_pool.push_back(DatabaseConnection::Ptr(new DatabaseConnection()));
}

void DatabaseConnectionPool::drainPool() {

	for (auto c : _pool) {
		c->disconnect();
	}
}

void DatabaseConnectionPool::fillPool(const std::string dataset_name) {

	for (auto c : _pool) {
		c->connect(dataset_name);
	}	
}


DatabaseConnection::Ptr DatabaseConnectionPool::requestConnection() {

	_mutex.lock();
	DatabaseConnection::Ptr con;
	if (_pool.size() > 0) {
		con = _pool.back();
		_pool.pop_back();
	}
	else {
		spd::get("Server")->error("Trying to request a connection from empty pool");
	}
	_mutex.unlock();

	return con;
}


void DatabaseConnectionPool::returnConnection(DatabaseConnection::Ptr c) {
	_mutex.lock();
	_pool.push_back(c);
	_mutex.unlock();
}
