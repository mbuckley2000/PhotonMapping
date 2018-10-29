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
#include <glm/vec3.hpp>

#define resX 600
#define resY 800

int main()
{

	Camera camera;
	camera.focus = glm::vec3(0, 0, 0);
	camera.backward = glm::vec3(0, 0, -1);
	camera.up = glm::vec3(0, 1, 0);
	camera.right = glm::vec3(1, 0, 0);
	camera.focalLength = 1;
	camera.imagePlane.worldSize = glm::vec2(1, 1);
	camera.imagePlane.resolution = glm::ivec2(resX, resY);;

	Sphere sphere(1);
	sphere.position = glm::vec3(0, 0, 2);
	sphere.colour = glm::vec3(0, 0, 1);
	
	Mesh mesh("C:/Users/mbuck/Downloads/cube (1).obj");
	mesh.position = glm::vec3(0, 0, 2);
	mesh.colour = glm::vec3(0, 1, 0);
	
	Scene scene;
	//scene.objects.push_back(&sphere);
	scene.objects.push_back(&mesh);
	scene.camera = &camera;

	cv::Mat_<cv::Vec3b> image(resX, resY, cv::Vec3b(0, 0, 0));
	scene.target = &image;
	scene.render();

	cv::Mat out = image;
	cv::resize(image, out, cv::Size(), 1, 1);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", out); // Show our image inside it.


	cv::waitKey(0);
}