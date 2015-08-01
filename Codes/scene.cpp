#include "common.h"
#include "primitive.h"
#include "KDTree.h"
#include <thread>

using namespace std;

Scene::Scene(int width, int height, bool super_sampling, int DOF_times)
	: sample(0.5, 0.5), film(width, height, super_sampling),
	super_sampling(super_sampling), kd_tree(nullptr), DOF_times(DOF_times), 
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
		Ray ray;
		Color color = Color(0, 0, 0);
		if(!camera.use_DOF)
		{
			camera.generateRay(s, ray);
			ray_tracer.trace(ray, 0, color);
		}
		else
		{
			for (int i = 0; i < DOF_times; ++i)
			{
				Color t(0, 0, 0);
				camera.generateRay(s, ray);
				ray_tracer.trace(ray, 0, t);
				color += t;
			}
			color = 1.0 / DOF_times * color;
		}
		film.commit(s, color);
	}
}


void Scene::render(int cores, bool use_kd_tree)
{
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
