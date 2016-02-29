#pragma once

#include <vector>
#include <string>
#include <iostream>

class DatabaseResult {

public:
	DatabaseResult();
	std::vector<std::string> getRow(int num);
	std::string getData(int row, int column);
	void push_row(std::vector<std::string> row);
	friend std::ostream& operator<< (std::ostream& stream, const DatabaseResult dr);
private:
	int _numRows;
	int _numCols;
	std::vector< std::vector<std::string> > _data;
	
};

inline std::ostream& operator<< (std::ostream& stream, const DatabaseResult dr) {

	for (auto i : dr._data) {
		for (auto j : i) {
			stream << j << std::string(" ");
		}
		stream << std::endl;
	}	

	return stream;
}

