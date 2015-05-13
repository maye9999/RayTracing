#include "common.h"
#include "primitive.h"

Scene::Scene(int width, int height)
	: sample(0.5, 0.5), color(0, 0, 0), film(width, height), 
	sampler(width, height), camera(width, height), ray_tracer(this)
{

}

void Scene::render()
{
	while (sampler.generateNextSample(sample))
	{
		camera.generateRay(sample, ray);
		color = Color(0, 0, 0);
		ray_tracer.trace(ray, 0, color, false);
		film.commit(sample, color);
	}
	film.writeImage();
}

void Scene::setGlobalMaterial(Material* material)
{
	for each (Primitive* var in objects)
	{
		var->setMaterial(material);
	}
}

void Scene::setGlobalBRDF(BRDF* brdf)
{
	for each (Primitive* p in objects)
	{
		p->setBRDF(brdf);
	}
}
