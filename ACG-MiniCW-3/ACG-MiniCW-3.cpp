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
#include "Sphere.h"
#include "ObjectInstance.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include <opencv2/imgproc/imgproc.hpp>


using namespace Eigen;

int main()
{
	Vector2i resolution(10, 15);

	Camera camera;
	camera.focus << 0, 0, 0;
	camera.backward << 0, 0, -1;
	camera.up << 0, 1, 0;
	camera.right = Vector3d(1, 0, 0);
	camera.focalLength = 1;
	camera.imagePlane.worldSize = Vector2d(2, 2);
	camera.imagePlane.resolution = Vector2i(resolution(0), resolution(1));

	Sphere sphere(1);

	ObjectInstance sphereInstance(sphere);
	sphereInstance.position << 0, -1, 2;
	sphereInstance.colour = (0, 0, 255);

	Mesh mesh;
	mesh.loadFromFile("C:/Users/mbuck/Downloads/cube.obj");
	ObjectInstance meshInst(mesh);
	meshInst.position << 0, 1, 2;
	meshInst.colour = (255, 0, 255);
	
	Scene scene;
	scene.objectInstances.push_back(&sphereInstance);
	scene.objectInstances.push_back(&meshInst);
	scene.camera = &camera;

	cv::Mat_<cv::Vec3b> image(resolution(0), resolution(1), cv::Vec3b(0, 0, 0));
	scene.render(image);

	cv::Mat out;
	cv::resize(image, out, cv::Size(), 20, 20);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", out); // Show our image inside it.


	cv::waitKey(0);
}