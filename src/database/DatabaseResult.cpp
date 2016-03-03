#include "DatabaseResult.hpp"
#include <assert.h>

DatabaseResult::DatabaseResult() {
	_numRows = 0;
	_numCols = 0;
}

std::vector<std::string> DatabaseResult::getRow(int nRow) {
	assert(nRow < _numRows);
	return _data[nRow];
}

std::string DatabaseResult::getData(int nRow, int nCol) {
	assert(nRow < _numRows);
	assert(nCol < _numCols);
	return _data[nRow][nCol];
	
}

void DatabaseResult::push_row(std::vector<std::string> row) {
	if (_data.size() == 0) _numCols = row.size();
	assert(row.size() == _numCols);
	_data.push_back(row);
	_numRows++;
}
