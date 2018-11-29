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

using namespace Eigen;

int main()
{
	Vector2i resolution(200, 200);

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
	sphere.colour << 0.6, 0.3, 1;
	sphere.ambient = 0.5;
	sphere.specularCoeff = 0.5;
	sphere.specularPower = 32;

	//PointLight light(Vec3(0, 0.9, 2), Vec3(0.7, 0.7, 0.7));
	//DirectionalLight light(Vec3(0, -1, 0), Vec3(0.8, 0.8, 0.8));
	SquareLight light(Vec3(0.8, 0.8, 0.8), Vec3(0, 0.9, 2), Vec3(0, -1, 0), 0.1);
	//c, p, n, s

	Mesh mesh("C:/Users/mbuck/Desktop/cube.obj");
	mesh.position << 0, -0.5, 2;
	//mesh.position << 0, 0, 1.5;
	mesh.colour << 0.5, 0.5, 1;
	mesh.ambient = 0.2;
	mesh.specularCoeff = 0;
	mesh.specularPower = 32;
	mesh.scale = 0.2;
	mesh.shadow = true;

	
	Plane plane;
	plane.normal << 0, 1, 0;
	plane.offset = 5; //Distance from origin (In terms of normal)
	plane.colour << 1, 0, 0;
	plane.ambient = 0.2;
	plane.specularCoeff = 0.5;
	plane.specularPower = 32;



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
	ts.at(0).colour << 0, 1, 0;
	ts.at(1).colour << 0, 1, 0;
	//Top White
	ts.push_back(Triangle(vs.at(3), vs.at(5), vs.at(1)));
	ts.push_back(Triangle(vs.at(7), vs.at(5), vs.at(3)));
	ts.at(2).colour << 1, 1, 1;
	ts.at(3).colour << 1, 1, 1;
	//Left Red
	ts.push_back(Triangle(vs.at(7), vs.at(3), vs.at(6)));
	ts.push_back(Triangle(vs.at(6), vs.at(3), vs.at(2)));
	ts.at(4).colour << 1, 0, 0;
	ts.at(5).colour << 1, 0, 0;
	//Bottom White
	ts.push_back(Triangle(vs.at(4), vs.at(2), vs.at(0)));
	ts.push_back(Triangle(vs.at(4), vs.at(6), vs.at(2)));
	ts.at(6).colour << 1, 1, 1;
	ts.at(7).colour << 1, 1, 1;
	ts.at(6).shadow = true;
	ts.at(7).shadow = true;
	//Back White
	ts.push_back(Triangle(vs.at(2), vs.at(1), vs.at(0)));
	ts.push_back(Triangle(vs.at(2), vs.at(3), vs.at(1)));
	ts.at(8).colour << 1, 1, 1;
	ts.at(9).colour << 1, 1, 1;

	//Light visualisation
	Vec3 a = Vec3(1, 0.99, 2.5);
	Vec3 b = Vec3(0.5, 0.99, 2.5);
	Vec3 c = Vec3(0.5, 0.99, 3);
	Vec3 d = Vec3(1, 0.99, 3);
	Triangle t1 = Triangle(a, b, c);
	Triangle t2 = Triangle(a, c, d);
	t1.colour << 1, 1, 1;
	t2.colour << 1, 1, 1;
	t1.ambient = 1;
	t2.ambient = 1;
	t1.specularCoeff = 0.5;
	t1.specularPower = 32;
	t2.specularCoeff = 0.5;
	t2.specularPower = 32;

	Scene scene;
	//scene.objects.push_back(&sphere);
	//scene.objects.push_back(&floor);
	scene.objects.push_back(&mesh);
	scene.camera = &camera;
	scene.light = &light;

	//scene.objects.push_back(&t1);
	//scene.objects.push_back(&t2);


	for (int i = 0; i < ts.size(); i++) {
		ts.at(i).ambient = 0.2;
		ts.at(i).specularCoeff = 0;
		ts.at(i).specularPower = 32;
		//ts.at(i).colour << random(0.5, 1), random(0.5, 1), random(0.5, 1);
		scene.objects.push_back(&ts.at(i));
	}

	//std::vector<Sphere> spheres;

	//int maxSphere = 2;

	//for (int i = 0; i < maxSphere; i++) {
	//	Sphere sphere(random(0.5, 1));
	//	sphere.position << random(-1, 1), random(-1, 1), random(6, 10);
	//	sphere.colour << random(0.6, 1), random(0.6, 1), random(0.6, 1);
	//	sphere.ambient = random(0.15, 0.3);
	//	sphere.specularCoeff = random(0.3, 0.6);
	//	sphere.specularPower = 32;
	//	spheres.push_back(sphere);
	//}

	//for (int i = 0; i < maxSphere; i++) {
	//	//scene.objects.push_back(&spheres.at(i));
	//}

	cv::Mat_<cv::Vec3b> image(resolution(1), resolution(0), cv::Vec3b(0, 0, 0));
	scene.target = &image;


	std::cout << "Mapping photons" << std::endl;
	PhotonMap photonMap = PhotonMap(&scene);
	photonMap.mapPhotons(100000);

	std::cout << "Finished mapping photons" << std::endl;

	std::vector<Photon>* photons = &(photonMap.photons);

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
	cv::imshow("Display window", image); // Show our image inside it.


	cv::waitKey(0);
}