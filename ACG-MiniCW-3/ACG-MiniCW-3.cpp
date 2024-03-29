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
#include "Common.h"
#include "SquareLight.h"
#include "PointLight.h"
#include "PhotonMap.h"
#include "Material.h"
#include "BRDF.h"

using namespace Eigen;

int main()
{

	//Render resolution The output will be half of this for super sampling
	Vector2i resolution(400, 400);
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
	glass.transmissive = true;
	glass.specularProbability = 1;
	glass.diffuseProbability = 0;
	glass.reflectiveness = 0.15;
	glass.refractiveness = 0.85;

	Sphere sphere(0.3);
	sphere.position << -0.4, -0.4, 2;
	sphere.material = glass;
	scene.objects.push_back(&sphere);

	//PointLight light(Vec3(0, 0.9, 2), Vec3(1, 1, 1));
	//DirectionalLight light(Vec3(0, -1, 0), Vec3(0.8, 0.8, 0.8));
	SquareLight light(Vec3(1, 1, 1), Vec3(0, 0.9, 2), Vec3(0, -1, 0), 0.1);
	//c, p, n, s

	Material whiteWall = Material();
	whiteWall.diffuseProbability = 1.0;
	whiteWall.specularProbability = 0;

	std::cout << "Loading gold BDRF" << std::endl;
	Material gold;
	read_brdf("C:/Users/mbuck/Downloads/gold-metallic-paint.binary", gold.brdf);
	std::cout << "Loaded" << std::endl;

	std::cout << "Loading gold BDRF" << std::endl;
	Material redWall = whiteWall;
	read_brdf("C:/Users/mbuck/Downloads/dark-red-paint.binary", redWall.brdf);
	std::cout << "Loaded" << std::endl;

	std::cout << "Loading white BDRF" << std::endl;
	read_brdf("C:/Users/mbuck/Downloads/white-paint.binary", whiteWall.brdf);
	std::cout << "Loaded" << std::endl;

	//Hi POLY bunny
	Mesh bunny("C:/Users/mbuck/Desktop/bunny_norm.obj");
	bunny.position << 0, -0.5, 2;
	bunny.material = glass;
	bunny.scale = 0.5;
	bunny.shadow = false;
	bunny.calculateTriangles();
	bunny.calculateBoundingBox();
	std::cout << "Building bunny KD-Tree" << std::endl;
	bunny.buildKDTree();
	scene.objects.push_back(&bunny);

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


	/*
		Set up our cornell box
	*/
	std::vector<Triangle> ts;
	//Right
	ts.push_back(Triangle(vs.at(1), vs.at(5), vs.at(4)));
	ts.push_back(Triangle(vs.at(0), vs.at(1), vs.at(4)));
	ts.at(0).material = redWall;
	ts.at(1).material = redWall;
	//Top White
	ts.push_back(Triangle(vs.at(3), vs.at(5), vs.at(1)));
	ts.push_back(Triangle(vs.at(7), vs.at(5), vs.at(3)));
	ts.at(2).material = whiteWall;
	ts.at(3).material = whiteWall;
	//Left
	ts.push_back(Triangle(vs.at(7), vs.at(3), vs.at(6)));
	ts.push_back(Triangle(vs.at(6), vs.at(3), vs.at(2)));
	ts.at(4).material = whiteWall;
	ts.at(5).material = whiteWall;
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

	
	//scene.objects.push_back(&mesh);
	scene.camera = &camera;
	scene.light = &light;

	for (int i = 0; i < ts.size(); i++) {
		scene.objects.push_back(&ts.at(i));
	}

	
	//Set up render target
	cv::Mat_<cv::Vec3b> image(resolution(1), resolution(0), cv::Vec3b(0, 0, 0));
	scene.target = &image;

	//Map photons for photon and caustic map
	std::cout << "Mapping photons" << std::endl;
	PhotonMap photonMap(&scene, false, false, Vec3());
	PhotonMap causticMap(&scene, true, true, bunny.position); //Aim the photons at the bunny position
	photonMap.mapPhotons(10000);
	causticMap.mapPhotons(1000);
	scene.photonMap = &photonMap;
	scene.causticMap = &causticMap;

	std::cout << "Finished mapping photons" << std::endl;

	std::cout << "Rendering" << std::endl;

	cv::namedWindow("Render output", cv::WINDOW_AUTOSIZE); // Create a window for display.

	scene.render();
	std::cout << "Finished rendering" << std::endl;

	//Super sampling. Half the size and use bilinear filtering
	cv::Mat out = image;
	cv::resize(image, out, cv::Size(), 0.5, 0.5);
	cv::imwrite("C:/Users/mbuck/Desktop/output_ss.png", out);

	cv::imshow("Render output", out); // Show our image inside it.

	cv::waitKey(0);
}