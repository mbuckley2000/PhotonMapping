#pragma once
#include "Vectors.h"
Vec3 our_getBDRF(Vec3 incomingAngle, Vec3 outgoingAngle, Vec3 surfaceNormal, double* brdf);

void lookup_brdf_val(double* brdf, double theta_in, double fi_in,
	double theta_out, double fi_out,
	double& red_val, double& green_val, double& blue_val);

bool read_brdf(const char *filename, double* &brdf);