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
#include "DirectionalLight.h"

using namespace Eigen;

int main()
{
	Vector2i resolution(900, 900);

	Camera camera;
	camera.focus << 0, 0, 0;
	camera.backward << 0, 0, -1;
	camera.up << 0, 1, 0;
	camera.right << 1, 0, 0;
	camera.focalLength = 1;
	camera.imagePlane.worldSize << 1, 1;
	camera.imagePlane.resolution << resolution(0), resolution(1);

	Sphere sphere(0.5);
	sphere.position << 0, 0, 2;
	sphere.colour << 0.6, 0.3, 1;
	sphere.ambient = 0.2;
	sphere.specular = 0.5;

	DirectionalLight light(Eigen::Vector3f(0.7, 0.7, 0.7), Eigen::Vector3f(1, 0, 1));
	
	Mesh mesh("C:/Users/mbuck/Downloads/cube.obj");
	mesh.position << 0, 0, 4;
	mesh.colour << 1, 0, 0;
	mesh.ambient = 0.1;
	
	Scene scene;
	scene.objects.push_back(&sphere);
	//scene.objects.push_back(&mesh);
	scene.camera = &camera;
	scene.light = &light;

	cv::Mat_<cv::Vec3b> image(resolution(0), resolution(1), cv::Vec3b(0, 0, 0));
	scene.target = &image;
	scene.render();

	//cv::Mat out = image;
	//cv::resize(image, out, cv::Size(), 10, 10);

	cv::imwrite("C:/Users/mbuck/Desktop/output.png", image);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", image); // Show our image inside it.


	cv::waitKey(0);
}