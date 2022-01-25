#pragma once

#include <windows.h>

class Timer {
public:
	Timer() {
	}
	~Timer() {};

	void Start() {
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);
	}

	long long End() {
		QueryPerformanceCounter(&end);
		return ((end.QuadPart - start.QuadPart) * 1000000) / freq.QuadPart;
	}



private:

	LARGE_INTEGER			freq;
	LARGE_INTEGER			start;
	LARGE_INTEGER			end;


};