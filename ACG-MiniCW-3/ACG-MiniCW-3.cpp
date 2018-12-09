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
#include "BRDF.h"

using namespace Eigen;

int main()
{
	Vector2i resolution(100, 100);
	Scene scene;

	Camera camera;
	camera.focus << 0, 0, -1;
	camera.backward << 0, 0, -1;
	camera.up << 0, 1, 0;
	camera.right << 1, 0, 0;
	camera.focalLength = 2;
	camera.imagePlane.worldSize << 2, 2;
	camera.imagePlane.resolution << resolution(0), resolution(1);

	Material glass;
	glass.refractive = true;
	glass.specularPower = 32;
	glass.specularProbability = 1;
	glass.diffuseProbability = 0;
	glass.baseColour << 0, 0.416, 0.306;
	glass.reflectiveness = 0.15;
	glass.refractiveness = 0.85;

	Sphere sphere(0.3);
	sphere.position << 0.4, -0.7, 2;
	sphere.material = glass;
	scene.objects.push_back(&sphere);

	PointLight light(Vec3(0, 0.9, 2), Vec3(1, 1, 1));
	//DirectionalLight light(Vec3(0, -1, 0), Vec3(0.8, 0.8, 0.8));
	//SquareLight light(Vec3(0.8, 0.8, 0.8), Vec3(0, 0.9, 2), Vec3(0, -1, 0), 0.1);
	//c, p, n, s

	Material whiteWall = Material();
	whiteWall.baseColour << 1, 1, 1;
	whiteWall.diffuseProbability = 1.0;
	whiteWall.specularProbability = 0;

	////LOW POLY
	//Mesh lpbunny("C:/Users/mbuck/Desktop/Bunny-LowPoly.obj");
	//lpbunny.position << -0.3, 0, 1.5;
	//lpbunny.material = glass;
	//lpbunny.scale = 3;
	//lpbunny.shadow = false;
	//scene.objects.push_back(&lpbunny);

	

	

	Material greenWall = whiteWall;
	greenWall.baseColour << 0, 1, 0;

	Material redWall = whiteWall;
	redWall.baseColour << 1, 0, 0;

	std::cout << "Loading gold BDRF" << std::endl;
	Material gold;
	gold.usingBRDF = true;
	read_brdf("C:/Users/mbuck/Downloads/gold-metallic-paint.binary", gold.brdf);
	std::cout << "Loaded" << std::endl;


	//Hi POLY
	//Mesh bunny("C:/Users/mbuck/Desktop/bunny_norm.obj");
	//bunny.position << 0, -0.8, 2;
	//bunny.material = glass;
	//bunny.scale = 0.25;
	//bunny.shadow = false;
	//scene.objects.push_back(&bunny);

	/*std::cout << "Loading red BDRF" << std::endl;
	redWall.usingBRDF = true;
	read_brdf("C:/Users/mbuck/Downloads/specular-red-phenolic.binary", redWall.brdf);
	std::cout << "Loaded" << std::endl;

	std::cout << "Loading green BDRF" << std::endl;
	greenWall.usingBRDF = true;
	read_brdf("C:/Users/mbuck/Downloads/green-metallic-paint.binary", greenWall.brdf);
	std::cout << "Loaded" << std::endl;*/

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
	ts.at(0).material = gold;
	ts.at(1).material = gold;
	//Top White
	ts.push_back(Triangle(vs.at(3), vs.at(5), vs.at(1)));
	ts.push_back(Triangle(vs.at(7), vs.at(5), vs.at(3)));
	ts.at(2).material = gold;
	ts.at(3).material = gold;
	//Left Red
	ts.push_back(Triangle(vs.at(7), vs.at(3), vs.at(6)));
	ts.push_back(Triangle(vs.at(6), vs.at(3), vs.at(2)));
	ts.at(4).material = gold;
	ts.at(5).material = gold;
	//Bottom White
	ts.push_back(Triangle(vs.at(4), vs.at(2), vs.at(0)));
	ts.push_back(Triangle(vs.at(4), vs.at(6), vs.at(2)));
	ts.at(6).material = gold;
	ts.at(7).material = gold;
	ts.at(6).shadow = true;
	ts.at(7).shadow = true;
	//Back White
	ts.push_back(Triangle(vs.at(2), vs.at(1), vs.at(0)));
	ts.push_back(Triangle(vs.at(2), vs.at(3), vs.at(1)));
	ts.at(8).material = gold;
	ts.at(9).material = gold;

	
	//scene.objects.push_back(&mesh);
	scene.camera = &camera;
	scene.light = &light;

	for (int i = 0; i < ts.size(); i++) {
		scene.objects.push_back(&ts.at(i));
	}

	cv::Mat_<cv::Vec3b> image(resolution(1), resolution(0), cv::Vec3b(0, 0, 0));
	scene.target = &image;

	std::cout << "Mapping photons" << std::endl;
	PhotonMap photonMap(&scene);
	photonMap.mapPhotons(100000);

	std::cout << "Finished mapping photons" << std::endl;
	std::vector<Photon>* photons = &(photonMap.photons);

	std::cout << "Rendering" << std::endl;

	cv::namedWindow("Render output", cv::WINDOW_AUTOSIZE); // Create a window for display.

	scene.photonMap = &photonMap;
	scene.render();
	std::cout << "Finished rendering" << std::endl;

	cv::Mat out = image;
	cv::resize(image, out, cv::Size(), 10, 10);

	cv::imwrite("C:/Users/mbuck/Desktop/output.png", image);

	cv::imshow("Render output", image); // Show our image inside it.


	cv::waitKey(0);
}