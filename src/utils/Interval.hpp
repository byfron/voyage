#pragma once

#include <vector>

template <typename T>
class Interval {

public:
	Interval() : _lower(T(0)), _upper(T(1)) {}
	Interval(T l, T u) : _lower(l), _upper(u) {}

	T getLower() { return _lower; }
	T getUpper() { return _upper; }
	T getWidth() { return _upper - _lower; }
	
	bool contains(T x) const {
		return _lower <= x && x < _upper;
	}

private:

	T _lower;
	T _upper;
	
};

template <typename T>
class MultiInterval {

public:

	MultiInterval(Interval<T> range, int num) {
		T width = range.getWidth()/num;
		T range_start = range.getLower();
		for (int i = 0; i < num; i++) {
			_intervals.push_back(Interval<T>(range_start, range_start + width));
			range_start += width;
		}
	}

	int indexContains(T x) const {
		for (int idx = 0; idx < _intervals.size(); idx++) {
			if (_intervals[idx].contains(x)) return idx;
		}
	}

private:

	std::vector<Interval<T>> _intervals;
};
