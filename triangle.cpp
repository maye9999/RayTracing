#include "common.h"
#include "primitive.h"
#include <opencv2/core/core.hpp>
#include <cmath>
#include <iostream>

using namespace std;
bool Triangle::intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo)
{
	Vec direction = ray.direction;
	Point pos = ray.start_position;
	Normal norm = Vec::normalize(Vec::cross(c-a, b-a));
	double tmp = direction * norm;
	if (fabs(tmp) < EPS)
	{
		return false;
	} 
	double t = (a * norm - pos * norm) / tmp;
	if(t < EPS || t > t_hit)
		return false;
	// Now Check whether in Triangle
	Point p = ray.start_position + ray.direction * t;
	cv::Matx22d mat;
	cv::Vec2d aa;
	if(fabs(b.x - a.x) < EPS && fabs(c.x - a.x) < EPS)
	{
		mat = cv::Matx22d((b-a).z, (c-a).z, (b-a).y, (c-a).y);
		aa = mat.inv() * cv::Vec2d((p-a).z, (p-a).y);
	}
	else if(fabs(b.y - a.y) < EPS && fabs(c.y - a.y) < EPS)
	{
		mat = cv::Matx22d((b-a).z, (c-a).z, (b-a).x, (c-a).x);
		aa = mat.inv() * cv::Vec2d((p-a).z, (p-a).x);
	}
	else
	{
		mat = cv::Matx22d((b-a).x, (c-a).x, (b-a).y, (c-a).y);
		aa = mat.inv() * cv::Vec2d((p-a).x, (p-a).y);
	}
	double beta = aa[0];
	double gamma = aa[1];
	if(beta > -EPS && gamma > -EPS && beta + gamma < 1 + EPS)
	{
		t_hit = t;
		localGeo.pos = p;
		localGeo.normal = Vec::normalize((tmp < -EPS) ? norm : -1 * norm);
		return true;
	}
	return false;
}

bool Triangle::intersectWithLight(const Ray& ray)
{
	Vec direction = ray.direction;
	Point pos = ray.start_position;
	Normal norm = Vec::normalize(Vec::cross(c-a, b-a));
	double tmp = direction * norm;
	if (fabs(tmp) < EPS)
	{
		return false;
	} 
	double t = (a * norm - pos * norm) / tmp;
	if(t < EPS || t > ray.t_max)
		return false;
	// Now Check whether in Triangle
	cv::Matx33d mat(direction.x, a.x - b.x, a.x - c.x,
		direction.y, a.y - b.y, a.y - c.y,
		direction.z, a.z - b.z, a.z - c.z);
	cv::Vec3d x = mat.solve(cv::Vec3d(a.x - pos.x, a.y - pos.y, a.z - pos.z), 0);
	if(fabs(x[0] - t) > EPS)
	{
		cerr << "ERR" << endl;
		assert(0);
	}
	if (x[1] > EPS && x[1] < 1 + EPS && x[2] > EPS && x[2] < 1 + EPS && x[2] + x[1] < 1 + EPS)
	{
		return true;
	}
	return false;
}

BRDF* Triangle::getBRDF(const LocalGeometry& localGeo)
{
	return brdf;
}

void Triangle::setBRDF(BRDF* brdf_)
{
	brdf = brdf_;
}