#include "common.h"
#include "primitive.h"
#include <thread>

using namespace std;

Scene::Scene(int width, int height, bool super_sampling)
	: sample(0.5, 0.5), film(width, height, super_sampling),
	super_sampling(super_sampling),
	camera(width, height), ray_tracer(this), width(width), height(height)
{

}


void Scene::renderPartial(pair<double, double> p)
{
	Sample s(p.first, 0.5);
	while(1)
	{
		s.y += 1;
		if(s.y > height)
		{
			if(s.x == int(p.second / 2) + 0.5)
				cerr << "Half Way There!\n";
			if(s.x == int(p.second / 4) + 0.5)
				cerr << "A Quarter Completed!\n";
			if(s.x == int(3 * p.second / 4) + 0.5)
				cerr << "Three Quarters Completed!\n";
			s.y = 0.5;
			s.x += 1;
			if(s.x > p.second)
			{
				break;
			}
		}
		Ray ray;
		camera.generateRay(s, ray);
		Color color = Color(0, 0, 0);
		//m.lock();
		ray_tracer.trace(ray, 0, color);		
		film.commit(s, color);
		//m.unlock();
	}
}


void Scene::render(int cores)
{
	
/*
	while(1)
	{
		sample.y += 1;
		if(sample.y > height)
		{
			if(sample.x == width / 2 + 0.5)
				std::cerr << "Half Way There!\n";
			if(sample.x == width / 4 + 0.5)
				std::cerr << "A Quarter Completed!\n";
			if(sample.x == 3 * width / 4 + 0.5)
				std::cerr << "Three Quarters Completed!\n";
			sample.y = 0.5;
			sample.x += 1;
			if(sample.x > width)
			{
				break;
			}
		}
		camera.generateRay(sample, ray);
		color = Color(0, 0, 0);
		ray_tracer.trace(ray, 0, color);
		film.commit(sample, color);
	}*/
	if(cores == 1)
	{
		renderPartial(make_pair(0.5, width - 0.5));
	}
	else if(cores == 2)
	{
		thread first(&Scene::renderPartial, this, make_pair(0.5, width/2 - 0.5));
		thread second(&Scene::renderPartial, this, make_pair(width/2 + 0.5, width - 0.5));
		first.join();
		second.join();
	}
	else if(cores == 3)
	{
		thread first(&Scene::renderPartial, this, make_pair(0.5, width/3 - 0.5));
		thread second(&Scene::renderPartial, this, make_pair(width/3 + 0.5, width*2/3 - 0.5));
		thread third(&Scene::renderPartial, this, make_pair(width*2/3 + 0.5, width - 0.5));
		first.join();
		second.join();
		third.join();
	}
	else
	{
		thread first(&Scene::renderPartial, this, make_pair(0.5, width/4 - 0.5));
		thread second(&Scene::renderPartial, this, make_pair(width/4 + 0.5, width/2 - 0.5));
		thread third(&Scene::renderPartial, this, make_pair(width/2 + 0.5, width*3/4 - 0.5));
		thread fourth(&Scene::renderPartial, this, make_pair(width*3/4 + 0.5, width - 0.5));
		first.join();
		second.join();
		third.join();
		fourth.join();
	}
	film.writeImage();
}

void Scene::setGlobalMaterial(Material* material)
{
	for (auto var : objects)
	{
		var->setMaterial(material);
	}
}

void Scene::setGlobalBRDF(BRDF* brdf)
{
	for (auto var : objects)
	{
		var->setBRDF(brdf);
	}
}
