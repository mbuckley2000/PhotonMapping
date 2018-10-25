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
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include <opencv2/imgproc/imgproc.hpp>


using namespace Eigen;

int main()
{
	Vector2i resolution(20, 20);

	Camera camera;
	camera.focus << 0, 0, 0;
	camera.backward << 0, 0, -1;
	camera.up << 0, 1, 0;
	camera.right << 1, 0, 0;
	camera.focalLength = 1;
	camera.imagePlane.worldSize << 2, 2;
	camera.imagePlane.resolution << resolution(0), resolution(1);

	Sphere sphere(1);
	sphere.position << 0, -1, 2;
	sphere.colour << 0, 0, 255;

	
	Mesh mesh("C:/Users/mbuck/Downloads/cube.obj");
	mesh.position << 0, 1, 2;
	mesh.colour = (255, 0, 255);
	
	Scene scene;
	//scene.objects.push_back(&sphere);
	scene.objects.push_back(&mesh);
	scene.camera = &camera;

	cv::Mat_<cv::Vec3b> image(resolution(0), resolution(1), cv::Vec3b(0, 0, 0));
	scene.target = &image;
	scene.render();

	cv::Mat out = image;
	cv::resize(image, out, cv::Size(), 10, 10);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", out); // Show our image inside it.


	cv::waitKey(0);
}