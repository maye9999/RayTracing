#include "common.h"
#include "primitive.h"
#include <iostream>

using namespace std;
void RayTracer::tarce(const Ray& ray, int depth, Color& color)
{
	if (depth >= 5)
	{
		return;
	}
	bool is_intersect = false;
	double t = T_MAX;
	LocalGeometry localGeo;
	for(auto i : scene->objects)
	{
		if(i->intersect(ray, t, localGeo))
			is_intersect = true;
	}
	if(is_intersect)
	{
		
		color = Vec(1, 0, 0);
	}
}
