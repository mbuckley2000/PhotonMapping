#include "pch.h"
#include "Scene.h"
#include "Ray.h"
#include "Vectors.h"
#include "Common.h"

cv::Vec3b toRGB(Vec3 v) {
	for (int i = 0; i < 3; i++) {
		v(i) = max(v(i), 0);
		v(i) = min(v(i), 1);
	}

	return cv::Vec3b(v(2) * 255, v(1) * 255, v(0) * 255);
}

void Scene::render()
{

	float t, u, v;

	for (int x = 0; x < camera->imagePlane.resolution(0); x++) {
		for (int y = 0; y < camera->imagePlane.resolution(1); y++) {
			const Vec3 pixelWorldPos = this->camera->calculatePixelWorldPos(x, y);

			Ray ray(pixelWorldPos, pixelWorldPos - this->camera->focus); //Pos, dir


			Object* hitObj = NULL;

			if (ray.intersectsWith(*this, hitObj, t, u, v)) {
				//We have a hit
				const Vec3 intersectionPoint = ray.position + (t * ray.direction);

				Vec3 colour;

				//Ambient
				const Vec3 ambient = Vec3(0, 0, 0);// hitObj->ambient * light->colour;
				
				int num_rays = 1;
				int hitCount = 0;
				bool shadowed = false;
				//Check for shadow
				if (hitObj->shadow) {
					for (int i = 0; i < num_rays; i++) {
						Ray shadowTest = Ray(intersectionPoint, -light->vectorTo(intersectionPoint));
						//shadowTest.position = shadowTest.position + (0.000001 * shadowTest.direction);
						float shadowT;
						if (shadowTest.intersectsWith(*this, shadowT)) {
							if (shadowT <= (light->getDistanceFrom(intersectionPoint))) {
								hitCount++;
								shadowed = true;
							}
						}
					}

					//Shadow
					//colour = ambient.cwiseProduct(hitObj->getColour(u, v));
				}
				
				
					//Diffuse
					const Vec3 lightVector = -light->vectorTo(intersectionPoint);
					const Vec3 faceNormal = hitObj->getNormalAt(intersectionPoint);

					Vec3 diffuse;
					diffuse << 0, 0, 0;

					const float dot = max(faceNormal.dot(lightVector), 0);

					if (dot >= 0) {
						diffuse = dot * light->colour;
					}

					//Specular
					const Vec3 viewVector = (pixelWorldPos - intersectionPoint).normalized();
					const Vec3 reflectionVector = reflectVector(-lightVector, faceNormal);
					const float spec = pow(max(viewVector.dot(reflectionVector), 0), hitObj->material.specularPower);
					const Vec3 specular = hitObj->material.specularProbability * spec * light->colour;


					//Colour
					colour = (ambient + diffuse + specular).cwiseProduct(hitObj->getColour(u, v));
					
					if (shadowed) {
						const float maxLoss = 0.5;//1 - hitObj->ambient;
						colour *= 1 - (maxLoss*hitCount/num_rays);
					}

				(*this->target)(y, x) = toRGB(colour);
			}
		}
	}
}

Scene::Scene()
{
}


Scene::~Scene()
{
}
