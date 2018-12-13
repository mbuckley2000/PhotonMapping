#pragma once
#include "pch.h"
#include <Eigen/Dense>

/*
	Typedefs
*/
typedef Eigen::Vector3f Vec3; //3D float vector
typedef Eigen::Vector2i Vec2; //2D int vector
typedef Eigen::Matrix3f Mat3; //3x3 float matrix


Vec3 reflectVector(Vec3 vec, Vec3 normal);
Vec3 refractVector(Vec3 incomingVector, Vec3 surfaceNormal, float indexOfRefraction);
float calculateCosAzimuth(Vec3 vecA, Vec3 vecB, Vec3 planeNormal);
float cosBetweenVectors(Vec3 a, Vec3 b);