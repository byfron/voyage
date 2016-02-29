#pragma once

#include <sqlite3.h>
#include <deque>
#include <mutex>
#include "DatabaseConnection.hpp"

class DatabaseConnectionPool {

public:

	DatabaseConnectionPool(unsigned int pool_size);
	void fillPool(const std::string dataset_name);
	void drainPool();
	DatabaseConnection::Ptr requestConnection();
	void returnConnection(DatabaseConnection::Ptr);

	typedef std::shared_ptr<DatabaseConnectionPool> Ptr;

private:

	std::deque<DatabaseConnection::Ptr> _pool;
	unsigned int _size;
	std::mutex _mutex;
	
};
