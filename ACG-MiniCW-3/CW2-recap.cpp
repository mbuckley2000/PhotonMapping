// ACGCW2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <igl/combine.h>

void setTranslation(Eigen::Matrix4d& matrix, int x, int y, int z) {
	matrix <<
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1;
}

void toHomo(Eigen::MatrixXd& V) {
	//Convert V into a vertical matrix of homogenous coordinates
	V.transposeInPlace();
	V.conservativeResize(V.rows() + 1, V.cols());
	Eigen::VectorXd ones = Eigen::VectorXd::Ones(V.cols());
	V.bottomRows(1) = ones.transpose();
}

void fromHomo(Eigen::MatrixXd& V) {
	//Convert back to obj format vectors
	V.conservativeResize(V.rows() - 1, V.cols());
	V.transposeInPlace();
}

int main()
{
	//Load Cube
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
	igl::readOBJ("C:/Users/mbuck/Downloads/cube.obj", V, F);
	Eigen::MatrixXd V2 = V;
	Eigen::MatrixXi F2 = F;

	//Make homogeneous matrix for transform
	//(Convert the matrix of OBJ format horizontal vectors into a matrix of homogeneous coordinates (vertical vectors with projection ordinate))
	toHomo(V);
	toHomo(V2);

	//Make transformation matrix
	Eigen::Matrix4d translation;
	setTranslation(translation, 5, 5, 5);
	Eigen::Matrix4d translation2;
	setTranslation(translation2, -3, -2, -1);

	//Apply transform
	V = translation * V;
	V2 = translation2 * V2;

	//Convert matrix back into OBJ format matrix of vectors (horizontal)
	fromHomo(V);
	fromHomo(V2);

	//List vertices
	std::cout << V << std::endl;
	std::cout << V2 << std::endl;

	Eigen::MatrixXd outV;
	Eigen::MatrixXi outF;

	std::vector<Eigen::MatrixXd> VV;
	VV.push_back(V);
	VV.push_back(V2);

	std::vector<Eigen::MatrixXi> FF;
	FF.push_back(F);
	FF.push_back(F2);

	igl::combine(VV, FF, outV, outF);
	//Write to output obj
	igl::writeOBJ("C:/Users/mbuck/cube_out.obj", outV, outF);
	return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file