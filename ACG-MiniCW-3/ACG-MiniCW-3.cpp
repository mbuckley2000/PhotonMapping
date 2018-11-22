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
#include "Plane.h"
#include "Common.h"

using namespace Eigen;

int main()
{
	Vector2i resolution(200, 200);

	Camera camera;
	camera.focus << 0, 0, 0;
	camera.backward << 0, 0, -1;
	camera.up << 0, 1, 0;
	camera.right << 1, 0, 0;
	camera.focalLength = 1;
	camera.imagePlane.worldSize << 1, 1;
	camera.imagePlane.resolution << resolution(0), resolution(1);

	Sphere sphere(1);
	sphere.position << 0, 0, 2;
	sphere.colour << 0.6, 0.3, 1;
	sphere.ambient = 0.23;
	sphere.specularCoeff = 0.5;
	sphere.specularPower = 32;

	DirectionalLight light(Vec3(0.1, 1.2, 0.5), Vec3(1, 1, 1));
	
	Mesh mesh("C:/Users/mbuck/Desktop/bunny.obj");
	//mesh.position << -1.5, 2, 5;
	mesh.position << 0, 0, 1.5;
	mesh.colour << 1, 1, 1;
	mesh.ambient = 0.2;
	mesh.specularCoeff = 0.5;
	mesh.specularPower = 32;
	
	Plane plane;
	plane.normal << 0, 1, 0;
	plane.offset = -5; //Distance from origin (In terms of normal)
	plane.colour << 1, 1, 1;
	plane.ambient = 0.2;
	plane.specularCoeff = 0.5;
	plane.specularPower = 32;


	Scene scene;
	//scene.objects.push_back(&sphere);
	//scene.objects.push_back(&plane);
	scene.objects.push_back(&mesh);
	scene.camera = &camera;
	scene.light = &light;

	std::vector<Sphere> spheres;

	int maxSphere = 2;

	for (int i = 0; i < maxSphere; i++) {
		Sphere sphere(random(0.5, 1));
		sphere.position << random(-1, 1), random(-1, 1), random(2, 4);
		sphere.colour << random(0.6, 1), random(0.6, 1), random(0.6, 1);
		sphere.ambient = random(0.15, 0.3);
		sphere.specularCoeff = random(0.3, 0.6);
		sphere.specularPower = 32;
		spheres.push_back(sphere);
	}

	for (int i = 0; i < maxSphere; i++) {
		//scene.objects.push_back(&spheres.at(i));
	}

	cv::Mat_<cv::Vec3b> image(resolution(1), resolution(0), cv::Vec3b(0, 0, 0));
	scene.target = &image;
	scene.render();

	cv::Mat out = image;
	cv::resize(image, out, cv::Size(), 10, 10);

	cv::imwrite("C:/Users/mbuck/Desktop/output.png", image);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", image); // Show our image inside it.


	cv::waitKey(0);
}