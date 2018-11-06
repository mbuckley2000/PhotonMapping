#pragma once
#include <Eigen/Dense>;

class Light
{
public:
	Eigen::Vector3f colour;

	virtual Eigen::Vector3f vectorTo(Eigen::Vector3f position) = 0;

	Light(Eigen::Vector3f colour) {
		this->colour = colour;
	};

	virtual ~Light() {};
};