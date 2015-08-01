#include "common.h"
#include "primitive.h"

#include <iostream>
#include <cmath>

using namespace std;

bool Sphere::intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo, Color* color)
{
	Vec l = center - ray.start_position;
	double diff = l * l - radius * radius;
	if (diff < -EPS)
	{
		// Inside Sphere
		double tp = l * ray.direction;
		double d_sqre = l * l - tp * tp;
		assert(radius * radius - d_sqre > -EPS);
		double tmp = sqrt(radius * radius - d_sqre);
		if (tp + tmp < t_hit + EPS)
		{
			t_hit = tp + tmp;
			localGeo.pos = ray.start_position + t_hit * ray.direction;
			localGeo.normal = Vec::normalize(center - localGeo.pos);
			localGeo.material = material;
			localGeo.inside = true;
			return true;
		} 
		else
			return false;
	}
	else if (diff > EPS)
	{
		// Outside Sphere
		double tp = l * ray.direction;
		if(tp < -EPS)
			return false;
		double d_sqre = l * l - tp * tp;
		if(d_sqre - radius*radius > EPS)
			return false;
		double tmp = sqrt(radius * radius - d_sqre);
		if (tp - tmp < t_hit + EPS)
		{
			t_hit = tp - tmp;
			localGeo.pos = ray.start_position + t_hit * ray.direction;
			localGeo.normal = Vec::normalize(localGeo.pos - center);
			localGeo.material = material;
			localGeo.inside = false;
			return true;
		} 
		else
		{
			return false;
		}
	}
	else
	{
		// On Sphere
		cerr << "On Sphere" << endl;
		localGeo.pos = ray.start_position;
		localGeo.normal = Vec::normalize((l * ray.direction > EPS) ? -1 * l : l);
		localGeo.material = material;
		t_hit = 0;
		return true;
	}
}

bool Sphere::intersectWithLight(const Ray& ray)
{
	Vec l = center - ray.start_position;
	double diff = l * l - radius * radius;
	if (diff < -EPS)
	{
		// Inside Sphere
		return true;
	}
	else if (diff > EPS)
	{
		// Outside Sphere
		double tp = l * ray.direction;
		if(tp < -EPS)
			return false;
		double d_sqre = l * l - tp * tp;
		if(d_sqre - radius*radius > EPS)
			return false;
		double tmp = sqrt(radius * radius - d_sqre);
		if (tp - tmp < ray.t_max + EPS)
		{
			return true;
		} 
		else
			return false;
	}
	else
	{
		// On Sphere
		return true;
	}
}

void Sphere::print()
{
	printf("Sphere: (%lf %lf %lf) %lf\n", center.x, center.y, center.z, radius);
}


Point Sphere::getXLeft()
{
	return center - radius * Point(1.0, 0, 0);
}

Point Sphere::getXRight()
{
	return center + radius * Point(1.0, 0, 0);
}

Point Sphere::getYLeft()
{
	return center - radius * Point(0, 1.0, 0);
}

Point Sphere::getYRight()
{
	return center + radius * Point(0, 1.0, 0);
}

Point Sphere::getZLeft()
{
	return center - radius * Point(0, 0, 1.0);
}

Point Sphere::getZRight()
{
	return center + radius * Point(0, 0, 1.0);
}