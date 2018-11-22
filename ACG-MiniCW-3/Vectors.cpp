#include "pch.h"
#include "Vectors.h"

Vec3 reflectVector(Vec3 vec, Vec3 normal) {
	return vec - (2 * vec.dot(normal) * normal);
}