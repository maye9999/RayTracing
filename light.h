#ifndef __LIGHT_H_
#define __LIGHT_H_

#include "common.h"

class PointLight : public Light
{
public:
	PointLight(const Point& pos, const Color& color) : pos(pos), color(color) {}
	void generateLightRay(const LocalGeometry& localGeo, Ray& light_ray, Color& color)
	{
		light_ray.direction = Vec::normalize(pos - localGeo.pos);
		light_ray.start_position = localGeo.pos + 1E-5 * light_ray.direction;
		light_ray.t_max = sqrt((pos - localGeo.pos) * (pos - localGeo.pos));
		color = this->color;
	}

private:
	Point pos;
	Color color;
};

#endif