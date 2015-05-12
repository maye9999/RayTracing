#include "common.h"
#include "primitive.h"
#include <iostream>

using namespace std;
void RayTracer::trace(const Ray& ray, int depth, Color& color, bool refracted)
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
			brdf = i->getBRDF();
		}
	}
	if(!is_intersect)
	{
		color = Vec(0, 0, 0);
	}
	else
	{
		color += localGeo.material->ambient;
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
		// Reflect
		if (localGeo.material->reflect.notZero())
		{
			Ray reflect_ray;
			Vec reflect_dir = ray.direction - 2.0 * (ray.direction * localGeo.normal) * localGeo.normal;
			reflect_ray.direction = Vec::normalize(reflect_dir);
			reflect_ray.start_position = localGeo.pos + 1E-4 * reflect_ray.direction;
			reflect_ray.t_min = 0;
			reflect_ray.t_max = T_MAX;
			
			Color reflect_color;
			trace(reflect_ray, depth+1, reflect_color, refracted);
			color += Vec::mul(localGeo.material->reflect, reflect_color);
		}
		// Refract
		if (localGeo.material->refract.notZero())
		{
			double n = (refracted ? localGeo.material->refract_index : 1.0 / localGeo.material->refract_index);
			double cos_theta_i = -1.0 * ray.direction * localGeo.normal;
			double tmp = 1.0 - n * n * (1.0 - cos_theta_i * cos_theta_i);
			if (tmp > -EPS)
			{
				Ray refract_ray;
				double cos_theta_t = sqrt(tmp);
				refract_ray.direction = n * ray.direction - (n * (ray.direction * localGeo.normal) + cos_theta_t) * localGeo.normal;
				refract_ray.direction = Vec::normalize(refract_ray.direction);
				refract_ray.start_position = localGeo.pos + 1E-4 * refract_ray.direction;
				refract_ray.t_max = T_MAX;
				refract_ray.t_min = 0;
				Color refract_color;
				trace(refract_ray, depth+1, refract_color, !refracted);
				color += Vec::mul(localGeo.material->refract, refract_color);
			}
		}
	}
}
