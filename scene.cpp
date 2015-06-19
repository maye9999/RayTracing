#include "common.h"
#include "primitive.h"
#include "KDTree.h"
#include <thread>

using namespace std;

Scene::Scene(int width, int height, bool super_sampling)
	: sample(0.5, 0.5), film(width, height, super_sampling),
	super_sampling(super_sampling), kd_tree(nullptr), 
	camera(width, height), ray_tracer(this, kd_tree), width(width), height(height)
{
	srand(unsigned(time(NULL)));
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
// 		if(s.x == 430.5 && s.y == 250.5)
// 			cout << "tt" << endl;
		Ray ray;
		camera.generateRay(s, ray);
		Color color = Color(0, 0, 0);
		//m.lock();
		ray_tracer.trace(ray, 0, color);		
		film.commit(s, color);
		//m.unlock();
	}
}


void Scene::render(int cores, bool use_kd_tree)
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
	clock_t t1, t2;
	t1 = clock();
	if (use_kd_tree)
	{
		kd_tree = new KDTree(objects);
		kd_tree->buildTree();
		ray_tracer.kd_tree = kd_tree;
	}
	thread threads[16];
	for(int i = 0; i < cores; ++i)
		threads[i] = thread(&Scene::renderPartial, this, make_pair(width*i/cores + 0.5, width*(i+1)/cores - 0.5));
	for(int i = 0; i < cores; ++i)
		threads[i].join();
	t2 = clock();
	cout << "Using Time: " << double(t2 - t1) / CLOCKS_PER_SEC << endl;
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
