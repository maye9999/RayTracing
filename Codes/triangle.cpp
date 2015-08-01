#include "common.h"
#include "primitive.h"
#include <opencv2/core/core.hpp>
#include <cmath>
#include <iostream>

using namespace std;
bool Triangle::intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo, Color* color)
{
	Vec direction = ray.direction;
	Point pos = ray.start_position;
	Normal norm;
	norm = Vec::normalize(Vec::cross(b-a, c-a));
	double tmp = direction * norm;
	if (fabs(tmp) < EPS)
	{
		return false;
	} 
	double t = (a * norm - pos * norm) / tmp;
	if(t < 0 || t > t_hit)
		return false;
	// Now Check whether in Triangle
	Point p = ray.start_position + ray.direction * t;
	Normal n = Vec::cross(b-a, c-a);
	double alpha, beta;
	if(n * Vec::cross(b-a, p-a) < 0)
		return false;
	if((alpha = n * Vec::cross(c-b, p-b)) < 0)
		return false;
	if((beta = n * Vec::cross(a-c, p-c)) < 0)
		return false;
	double deno = n * n;
	alpha /= deno;
	beta /= deno;
	t_hit = t;
	localGeo.pos = p;
	localGeo.normal = Vec::normalize((tmp < 0) ? norm : -1 * norm);
	localGeo.material = material;
	//cout << alpha << " " << beta << endl;
	if (texture)
	{
		double gamma = 1.0 - beta - alpha;
		double x = alpha * texture_mapping.ut.first +
			beta * texture_mapping.vt.first +
			gamma * texture_mapping.wt.first;

		double y = alpha * texture_mapping.ut.second +
			beta * texture_mapping.vt.second +
			gamma * texture_mapping.wt.second;
		y = 1.0 - y;
		color->x = texture->mat(int(y * texture->height), int(x * texture->width))[2] / 255.0;
		color->y = texture->mat(int(y * texture->height), int(x * texture->width))[1] / 255.0;
		color->z = texture->mat(int(y * texture->height), int(x * texture->width))[0] / 255.0;
	}
	else
	{
		color->x = 1.0;
		color->y = 1.0;
		color->z = 1.0;
	}
	return true;
}

bool Triangle::intersectWithLight(const Ray& ray)
{
	Vec direction = ray.direction;
	Point pos = ray.start_position;
	Normal norm;
	norm = Vec::normalize(Vec::cross(b-a, c-a));
	double tmp = direction * norm;
	if (fabs(tmp) < EPS)
	{
		return false;
	} 
	double t = (a * norm - pos * norm) / tmp;
	if(t < 0 || t > ray.t_max)
		return false;
	// Now Check whether in Triangle
	Point p = ray.start_position + ray.direction * t;
	Normal n = Vec::cross(b-a, c-a);
	if(n * (Vec::cross(b-a, p-a)) < 0)
		return false;
	if(n * (Vec::cross(c-b, p-b)) < 0)
		return false;
	if(n * (Vec::cross(a-c, p-c)) < 0)
		return false;
	return true;
}

void Triangle::print()
{
	printf("Tri: (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf)\n", a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z);
}