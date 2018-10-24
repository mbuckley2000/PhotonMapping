#include "pch.h"
#include "ObjectInstance.h"

bool ObjectInstance::rayIntersects(Ray & ray)
{
	return this->object->rayIntersects(ray, position);
}

ObjectInstance::ObjectInstance(Object& object)
{
	this->object = &object;
}

ObjectInstance::~ObjectInstance()
{
}