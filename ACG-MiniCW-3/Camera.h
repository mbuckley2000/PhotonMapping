#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct ImagePlane {
	glm::vec2 worldSize;
	glm::ivec2 resolution;
};

class Camera
{
public:
	glm::vec3 focus; //Focal position

	//Direction vectors
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 backward;

	float focalLength;

	glm::vec3 calculatePixelWorldPos(unsigned int pixelX, unsigned int pixelY);
	ImagePlane imagePlane;
	Camera();
	virtual ~Camera();
};