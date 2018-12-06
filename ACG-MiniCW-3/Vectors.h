#pragma once
#include "pch.h"
#include <Eigen/Dense>

typedef Eigen::Vector3f Vec3;
typedef Eigen::Vector2i Vec2;
typedef Eigen::Matrix3f Mat3;

Vec3 reflectVector(Vec3 vec, Vec3 normal);
Vec3 refractVector(Vec3 incomingVector, Vec3 surfaceNormal, float indexOfRefraction);