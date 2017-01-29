#pragma once

#include <fstream>

class Reader {
public:
	template <typename T>
	static Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>
	loadMatrixFromTextFile(const std::string & file, int w, int h) {
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> data(h,w);		
		std::ifstream fhandle(file);		
		for (unsigned int i = 0; i < h; i++) {
			for (unsigned int j = 0; j < w; j++) {
				char a;
				fhandle >> data(i,j);
			}
		}

		return data;
	}	
};
