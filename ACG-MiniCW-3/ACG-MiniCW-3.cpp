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
#include "SquareLight.h"
#include "PointLight.h"
#include "PhotonMap.h"
#include "Material.h"

using namespace Eigen;

int main()
{
	Vector2i resolution(100, 100);

	Camera camera;
	camera.focus << 0, 0, -1;
	camera.backward << 0, 0, -1;
	camera.up << 0, 1, 0;
	camera.right << 1, 0, 0;
	camera.focalLength = 2;
	camera.imagePlane.worldSize << 2, 2;
	camera.imagePlane.resolution << resolution(0), resolution(1);

	Sphere sphere(0.3);
	sphere.position << 0, 0.5, 2;
	sphere.material.baseColour << 0.6, 0.3, 1;
	sphere.material.specularProbability = 0.5;
	sphere.material.specularPower = 32;

	//PointLight light(Vec3(0, 0.9, 2), Vec3(0.7, 0.7, 0.7));
	//DirectionalLight light(Vec3(0, -1, 0), Vec3(0.8, 0.8, 0.8));
	SquareLight light(Vec3(0.8, 0.8, 0.8), Vec3(0, 0.9, 2), Vec3(0, -1, 0), 0.1);
	//c, p, n, s

	Mesh mesh("C:/Users/mbuck/Desktop/cube.obj");
	mesh.position << 0, -0.5, 2;
	mesh.material.baseColour << 0.5, 0.5, 1;
	mesh.material.specularProbability = 0;
	mesh.scale = 0.2;
	mesh.shadow = true;


	Material whiteWall = Material();
	whiteWall.baseColour << 1, 1, 1;
	whiteWall.diffuseProbability = 0.4;
	whiteWall.specularProbability = 0;

	Material greenWall = whiteWall;
	greenWall.baseColour << 0, 1, 0;

	Material redWall = whiteWall;
	redWall.baseColour << 1, 0, 0;

	/*
		Triangles for final render
	*/

	std::vector<Vec3> vs;
	vs.push_back(Vec3(1, -1, 3));
	vs.push_back(Vec3(1, 1, 3));
	vs.push_back(Vec3(-1, -1, 3));
	vs.push_back(Vec3(-1, 1, 3));
	vs.push_back(Vec3(1, -1, 1));
	vs.push_back(Vec3(1, 1, 1));
	vs.push_back(Vec3(-1, -1, 1));
	vs.push_back(Vec3(-1, 1, 1));

	std::vector<Triangle> ts;
	//Right Green
	ts.push_back(Triangle(vs.at(1), vs.at(5), vs.at(4)));
	ts.push_back(Triangle(vs.at(0), vs.at(1), vs.at(4)));
	ts.at(0).material = greenWall;
	ts.at(1).material = greenWall;
	//Top White
	ts.push_back(Triangle(vs.at(3), vs.at(5), vs.at(1)));
	ts.push_back(Triangle(vs.at(7), vs.at(5), vs.at(3)));
	ts.at(2).material = whiteWall;
	ts.at(3).material = whiteWall;
	//Left Red
	ts.push_back(Triangle(vs.at(7), vs.at(3), vs.at(6)));
	ts.push_back(Triangle(vs.at(6), vs.at(3), vs.at(2)));
	ts.at(4).material = redWall;
	ts.at(5).material = redWall;
	//Bottom White
	ts.push_back(Triangle(vs.at(4), vs.at(2), vs.at(0)));
	ts.push_back(Triangle(vs.at(4), vs.at(6), vs.at(2)));
	ts.at(6).material = whiteWall;
	ts.at(7).material = whiteWall;
	ts.at(6).shadow = true;
	ts.at(7).shadow = true;
	//Back White
	ts.push_back(Triangle(vs.at(2), vs.at(1), vs.at(0)));
	ts.push_back(Triangle(vs.at(2), vs.at(3), vs.at(1)));
	ts.at(8).material = whiteWall;
	ts.at(9).material = whiteWall;

	Scene scene;
	scene.objects.push_back(&mesh);
	scene.camera = &camera;
	scene.light = &light;

	for (int i = 0; i < ts.size(); i++) {
		scene.objects.push_back(&ts.at(i));
	}

	cv::Mat_<cv::Vec3b> image(resolution(1), resolution(0), cv::Vec3b(0, 0, 0));
	scene.target = &image;

	std::cout << "Mapping photons" << std::endl;
	PhotonMap photonMap = PhotonMap(&scene);
	photonMap.mapPhotons(10000);

	std::cout << "Finished mapping photons" << std::endl;
	std::vector<Photon>* photons = &(photonMap.photons);

	std::cout << "Testing KNN" << std::endl;
	std::vector<Photon*> neighbours = photonMap.findNearestNeighbours(Vec3(0, 0, 0), 5);

	std::cout << "Finished testing KNN" << std::endl;


	//std::vector<Sphere> spheres;

	//for (int i = 0; i < photons->size(); i++) {
	//	Sphere sphere(0.4);
	//	sphere.position = photons->at(i).position;
	//	sphere.colour = photons->at(i).flux;
	//	sphere.ambient = 1;
	//	sphere.specularCoeff = 0;
	//	sphere.shadow = false;
	//	sphere.specularPower = 0;
	//	spheres.push_back(sphere);
	//}

	//for (int i = 0; i < photons->size(); i++) {
	//	//scene.objects.push_back(&spheres.at(i));
	//}

	scene.render();

	cv::Mat out = image;
	cv::resize(image, out, cv::Size(), 4, 4);

	cv::imwrite("C:/Users/mbuck/Desktop/output.png", image);

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::imshow("Display window", out); // Show our image inside it.


	cv::waitKey(0);
}