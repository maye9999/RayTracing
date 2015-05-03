#include "common.h"

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
		ray_tracer.tarce(ray, 0, color);
		film.commit(sample, color);
	}
	film.writeImage();
}