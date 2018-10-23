// ACG-MiniCW-3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <igl/readOBJ.h>
#include <igl/writeOBJ.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <igl/readOBJ.h>

using namespace Eigen;

struct ImagePlane {
	Vector2d worldSize;
	Vector2i resolution;
};

struct Camera {
	Vector3d focus; //Focal position
	//Direction vectors
	Vector3d up;
	Vector3d right;
	Vector3d backward;

	double focalLength;

	ImagePlane imagePlane;
};

struct Mesh {
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
};

struct Sphere {
	Vector3d centre;
	double radius;
	Vector3d colour;
};

/*
	Calculates the world position of a pixel on the image plane of the camera
	Lecture 4
*/
Vector3d calculateWorldPos(Vector2i& pixelPos, Camera& camera) {
	int i = pixelPos(0);
	int j = pixelPos(1);
	double W = camera.imagePlane.worldSize(0);
	double H = camera.imagePlane.worldSize(1);
	int nW = camera.imagePlane.resolution(0);
	int nH = camera.imagePlane.resolution(1);

	double r = W * (((j-0.5)/nW)-0.5);
	double b = H * (((i - 0.5) / nH) - 0.5);

	Vector3d e = camera.focus;
	double d = camera.focalLength;
	Vector3d w = camera.backward;
	Vector3d u = camera.up;
	Vector3d v = camera.right;

	Vector3d q = e + (d*-w) + (r*u) - (b*v);

	//std::cout << std::endl << "  -  " << i << "  -  " << j << "  -  " << W << "  -  " << H << "  -  " << nW << "  -  " << nH << "  -  " << r << "  -  " << b << std::endl;

	return q;
}

struct Ray {
	Vector3d position;
	Vector3d direction;
};

//From: https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
bool raySphereIntersection(Ray& ray, Sphere& sphere) {
	Vector3d l = ray.direction.normalized();
	Vector3d o = ray.position;
	Vector3d c = sphere.centre;
	double r = sphere.radius;

	double value = (pow(l.dot(o - c), 2) - (o-c).squaredNorm() + pow(r, 2));

	return (value >= 0);
}


int main()
{
	
	Vector2i resolution(300, 300);

	cv::Mat_<cv::Vec3b> image(resolution(0), resolution(1), cv::Vec3b(0, 0, 0));

	//MatrixXd image(resolution(0), resolution(1));
	//image.setZero();
	
	Sphere sphere = {Vector3d(0, 0, 5), 2, Vector3d(1, 0, 0)};
	Camera camera;
	camera.focus = Vector3d(0, 0, 0);
	camera.backward = Vector3d(0, 0, -1);
	camera.up = Vector3d(0, 1, 0);
	camera.right = Vector3d(1, 0, 0);
	camera.focalLength = 1;
	camera.imagePlane.worldSize = Vector2d(2, 2);
	camera.imagePlane.resolution = Vector2i(resolution(0), resolution(1));
	
	for (int x = 0; x < resolution(0); x++) {
		for (int y = 0; y < resolution(1); y++) {
			
			Vector2i pixel(x, y);
			Vector3d pixelWorldPos = calculateWorldPos(pixel, camera);
			
			
			//STEP 5
			Ray ray{ pixelWorldPos, pixelWorldPos - camera.focus }; //Pos, dir

			//std::cout << "X: " << x << "Y: " << y << "PixelPos: " << pixelWorldPos(0) << ", " << pixelWorldPos(1) << ", " << pixelWorldPos(2) << std::endl;

			
			if (raySphereIntersection(ray, sphere)) {
				//Colour red
				//image.block<0, 0, 3>(x, y, 0) = sphere.colour;
				//std::cout << "# ";
				image(x, y) = cv::Vec3b(0, 0, 255);

			}
			else {
				//std::cout << "- ";
				//image.at<double>(x, y) = 0;
			}


			
			
			
		}
		//std::cout << std::endl;
	}
	
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", image); // Show our image inside it.
	cv::waitKey(0);
    //std::cout << "Hello World!\n"; 
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
