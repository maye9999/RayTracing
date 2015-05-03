#include "common.h"
#include "primitive.h"

#include <iostream>
#include <cmath>

using namespace std;

bool Sphere::intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo)
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
			return true;
		} 
		else
			return false;
	}
	else
	{
		// On Sphere
		cerr << "On Sphere" << endl;
		localGeo.pos = ray.start_position;
		localGeo.normal = (l * ray.direction > EPS) ? -1 * l : l;
		t_hit = 0;
		return true;
	}
}

bool Sphere::intersectWithLight(const Ray& ray)
{
	return false;
}

const BRDF* Sphere::getBRDF(const LocalGeometry& localGeo)
{
	return brdf;
}

void Sphere::setBRDF(BRDF* brdf_)
{
	brdf = brdf_;
}