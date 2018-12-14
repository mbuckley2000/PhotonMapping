#include "pch.h"
#include "Common.h"
#include <iostream>

float max(float a, float b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

float random(float min, float max) {
	//Not uniformly distributed
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}