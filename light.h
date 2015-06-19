#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "common.h"
#include "KDTree.h"
#include <iostream>

class PointLight : public Light
{
public:
	PointLight(Scene* s, const Point& pos, const Color& color) : Light(s), pos(pos), color(color) {}
	
	virtual bool computeShadow(const LocalGeometry& localGeo, Ray& light_ray, Color& light_color)
	{
		generateLightRay(localGeo, light_ray, light_color);
		bool found = false;
		if(scene->kd_tree != nullptr)
			found = scene->kd_tree->intersectWithLight(scene->kd_tree->_root, light_ray);
		else
		{
			for(auto j : scene->objects)
			{
				if(j->intersectWithLight(light_ray))
				{
					found = true;
					break;
				}
			}
		}
		return found;
	}
private:
	Point pos;
	Color color;

	void generateLightRay(const LocalGeometry& localGeo, Ray& light_ray, Color& color)
	{
		light_ray.direction = Vec::normalize(pos - localGeo.pos);
		light_ray.start_position = localGeo.pos + 1E-5 * light_ray.direction;
		light_ray.t_max = sqrt((pos - localGeo.pos) * (pos - localGeo.pos));
		color = this->color;
	}
};

class PlaneLight : public Light
{
public:
	PlaneLight(Scene* s, Point p, Vec u, Vec v, Color c, int samples = 4) : Light(s), pos(p), u(u), v(v), color(c), samples(samples) {}

	virtual bool computeShadow(const LocalGeometry& localGeo, Ray& light_ray, Color& light_color)
	{
		int through_times = 0;
		for(int i = 0; i < samples; ++i)
		{
			for(int j = 0; j < samples; ++j)
			{
				Point p = pos + (double(i) / samples) * u + (double(j) / samples) * v;
				double div = 1.0 / samples;
				double r = (double)rand() / RAND_MAX * div;
				p = p + r * u;
				r = (double)rand() / RAND_MAX * div;
				p = p + r * v;
				Ray ray;
				ray.direction = Vec::normalize(p - localGeo.pos);
				ray.start_position = localGeo.pos + 1E-5 * ray.direction;
				ray.t_max = sqrt((p - localGeo.pos) * (p - localGeo.pos));
				bool found = false;
				if(scene->kd_tree != nullptr)
					found = scene->kd_tree->intersectWithLight(scene->kd_tree->_root, ray);
				else
				{
					for(auto j : scene->objects)
					{
						if(j->intersectWithLight(ray))
						{
							found = true;
							break;
						}
					}
				}
				if (!found)
					through_times++;
			}
		}
		if(through_times == 0)
		{
			return true;
		}
		Point center = pos + 0.5 * u + 0.5 * v;
		light_ray.direction = Vec::normalize(center - localGeo.pos);
		light_ray.start_position = localGeo.pos + 1E-5 * light_ray.direction;
		light_ray.t_max = sqrt((center - localGeo.pos) * (center - localGeo.pos));
		light_color = double(through_times) / samples / samples * color;
		return false;
	}

	bool intersect(const Ray& ray, double& t_hit)
	{
		Vec direction = ray.direction;
		Point p = ray.start_position;
		Vec norm = Vec::normalize(Vec::cross(u, v));
		double tmp = direction * norm;
		if (fabs(tmp) < EPS)
		{
			return false;
		} 
		double t = (pos * norm - p * norm) / tmp;
		if(t < 0 || t > t_hit)
			return false;
		Point pp = ray.start_position + ray.direction * t;
		if(norm * Vec::cross(u, pp-pos) < 0)
			return false;
		if(norm * Vec::cross(v, pp-pos-u) < 0)
			return false;
		if(norm * Vec::cross(-1.0*u, pp-pos-u-v) < 0)
			return false;
		if(norm * Vec::cross(-1.0*v, pp-pos-v) < 0)
			return false;
		t_hit = t;
		return true;
	}
	Color color;
private:
	Vec u, v;
	Point pos;	
	int samples;
};


#endif