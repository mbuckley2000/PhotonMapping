#pragma once
#include "Vectors.h"

/**
 * Returns the value of the BRDF for the given arguments
 * @param incomingVec The vector of the incoming ray (From light)
 * @param outgoingVec Vector of the outgoing ray (To View)
 * @param surfaceNormal Vector of the surface normal at the point
 * @param brdf Pointer to the BRDF, loaded by read_brdf
 * @return The colour of the BRDF at the given arguments, as a 3D float vector from 0 to 1
 */
Vec3 our_getBDRF(Vec3 incomingVec, Vec3 outgoingVec, Vec3 surfaceNormal, double* brdf);

/**
 * Loads a BDRF from the given file into a double*
 * Copied from https://people.csail.mit.edu/wojciech/BRDFDatabase/code/BRDFRead.cpp
 * @param filename BRDF binary file from MERL database
 * @param brdf Double* to load into
 * @return True if success
 */
bool read_brdf(const char *filename, double* &brdf);


void lookup_brdf_val(double* brdf, double theta_in, double fi_in,
	double theta_out, double fi_out,
	double& red_val, double& green_val, double& blue_val);