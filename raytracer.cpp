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
	BRDF *brdf;
	for(auto i : scene->objects)
	{
		if(i->intersect(ray, t, localGeo))
		{
			is_intersect = true;
			brdf = i->getBRDF(localGeo);
		}
	}
	if(!is_intersect)
	{
		color = Vec(0, 0, 0);
	}
	else
	{
		Vec eye_dir = Vec::normalize(scene->camera.eye - localGeo.pos);
		for(auto i : scene->light_objects)
		{
			Ray light_ray;
			Color light_color;
			i->generateLightRay(localGeo, light_ray, light_color);
			bool found = false;
			for(auto j : scene->objects)
			{
				if(j->intersectWithLight(light_ray))
				{
					found = true;
					break;
				}
			}
			if(!found)
			{
				// Light can cast to that position
				color += brdf->compute(localGeo, light_ray, light_color, eye_dir);
			}
		}
	}
}
