#include "common.h"
#include "primitive.h"
#include "phong.h"
#include "KDTree.h"
#include "light.h"
#include <iostream>

using namespace std;
void RayTracer::trace(const Ray& ray, int depth, Color& color)
{
	if (depth >= 5)	// Depth of recursive
	{
		return;
	}
	bool is_intersect = false;
	double t = T_MAX;
	LocalGeometry localGeo;
	BRDF *brdf;
	Color texture_color(1.0, 1.0, 1.0);
	if (kd_tree != nullptr)		// using kd-tree
	{
		is_intersect = kd_tree->findNearestPrimitive(kd_tree->_root, ray, t, localGeo, brdf, &texture_color);
	}
	else
	{
		for(auto i : scene->objects)
		{
			if(i->intersect(ray, t, localGeo, &texture_color))
			{
				is_intersect = true;
				brdf = i->getBRDF();
			}
		}
	}
	bool flag = false;
	for(auto l : scene->plane_lights)
	{
		if(l->intersect(ray, t))
		{
			color = l->color;
			flag = true;
		}
	}
	if(flag)
		return;
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
			bool found = i->computeShadow(localGeo, light_ray, light_color);	
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
			reflect_ray.start_position = localGeo.pos + 1E-3 * reflect_ray.direction;
			reflect_ray.t_min = 0;
			reflect_ray.t_max = T_MAX;
			
			Color reflect_color;
			trace(reflect_ray, depth+1, reflect_color);
			color += Vec::mul(localGeo.material->reflect, reflect_color);
		}
		// Refract
		if (localGeo.material->refract.notZero())
		{
			double n = (localGeo.inside ? localGeo.material->refract_index : 1.0 / localGeo.material->refract_index);
			double cos_theta_i = ray.direction * localGeo.normal;
			double tmp = 1.0 - n * n * (1.0 - cos_theta_i * cos_theta_i);
			if (tmp > -EPS)
			{
				Ray refract_ray;
				double cos_theta_t = sqrt(tmp);
				refract_ray.direction = n * ray.direction - (n * cos_theta_i + cos_theta_t) * localGeo.normal;
				refract_ray.direction = Vec::normalize(refract_ray.direction);
				refract_ray.start_position = localGeo.pos + 1E-3 * refract_ray.direction;
				refract_ray.t_max = T_MAX;
				refract_ray.t_min = 0;
				Color refract_color;
				trace(refract_ray, depth+1, refract_color);
				color += Vec::mul(localGeo.material->refract, refract_color);
			}
			else
			{
				Ray reflect_ray;
				Vec reflect_dir = ray.direction - 2.0 * (ray.direction * localGeo.normal) * localGeo.normal;
				reflect_ray.direction = Vec::normalize(reflect_dir);
				reflect_ray.start_position = localGeo.pos + 1E-3 * reflect_ray.direction;
				reflect_ray.t_min = 0;
				reflect_ray.t_max = T_MAX;

				Color reflect_color;
				trace(reflect_ray, depth+1, reflect_color);
				color += Vec::mul(localGeo.material->refract, reflect_color);
			}
		}
		color = Vec::mul(color, texture_color);
	}
}
