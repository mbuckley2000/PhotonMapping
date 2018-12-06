#include "pch.h"
#include "Vectors.h"

Vec3 reflectVector(Vec3 incomingVector, Vec3 surfaceNormal) {
	return incomingVector - (2 * incomingVector.dot(surfaceNormal) * surfaceNormal);
}


//Returns cos of angle between two vectors
float cosBetweenVectors(Vec3 a, Vec3 b) {
	return (a.dot(b) / (a.norm() * b.norm()));
}


#define IOR_AIR 1.00029 //Index of refraction of air
/*
Refracts a vector upon hitting a surface based on the surface's normal and the surface material's index of refraction.
Entering / exiting a solid is determined by the surface normal direction
*/
Vec3 refractVector(Vec3 incomingVector, Vec3 surfaceNormal, float indexOfRefraction) {
	const float refractionRatio = IOR_AIR / indexOfRefraction;
	const float t1 = cosBetweenVectors(incomingVector, surfaceNormal);
	const float t2 = sqrt(1 - (pow(refractionRatio, 2) * (1 - pow(t1, 2))));

	return ((refractionRatio * incomingVector) + (((refractionRatio) * t1) - t2) * surfaceNormal);
}