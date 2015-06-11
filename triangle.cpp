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
// 	if (given_normal)
// 	{
// 		norm = normal;
// 	}
// 	else
// 	{
		norm = Vec::normalize(Vec::cross(c-a, b-a));
// 	}
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
	cv::Matx33d m1(ray.direction.x, (a - ray.start_position).x, (a-c).x,
				   ray.direction.y, (a - ray.start_position).y, (a-c).y,
				   ray.direction.z, (a - ray.start_position).z, (a-c).z);
	cv::Matx33d m2(ray.direction.x, (a-b).x, (a - ray.start_position).x,
				   ray.direction.y, (a-b).y, (a - ray.start_position).y,
				   ray.direction.z, (a-b).z, (a - ray.start_position).z);
	cv::Matx33d m0(ray.direction.x, (a-b).x, (a - c).x,
				   ray.direction.y, (a-b).y, (a - c).y,
				   ray.direction.z, (a-b).z, (a - c).z);
	double beta = cv::determinant(m1) / cv::determinant(m0);
	double gamma = cv::determinant(m2) / cv::determinant(m0);
// 	cv::Matx22d mat;
// 	cv::Vec2d aa;
// 	if(fabs(b.x - a.x) < EPS && fabs(c.x - a.x) < EPS)
// 	{
// 		mat = cv::Matx22d((b-a).z, (c-a).z, (b-a).y, (c-a).y);
// 		aa = mat.inv() * cv::Vec2d((p-a).z, (p-a).y);
// 	}
// 	else if(fabs(b.y - a.y) < EPS && fabs(c.y - a.y) < EPS)
// 	{
// 		mat = cv::Matx22d((b-a).z, (c-a).z, (b-a).x, (c-a).x);
// 		aa = mat.inv() * cv::Vec2d((p-a).z, (p-a).x);
// 	}
// 	else
// 	{
// 		mat = cv::Matx22d((b-a).x, (c-a).x, (b-a).y, (c-a).y);
// 		aa = mat.inv() * cv::Vec2d((p-a).x, (p-a).y);
// 	}
	//double beta = aa[0];
	//double gamma = aa[1];
	
	if(beta > -EPS && gamma > -EPS && beta + gamma < 1 + EPS)
	{
		t_hit = t;
		localGeo.pos = p;
		localGeo.normal = Vec::normalize((tmp < -EPS) ? norm : -1 * norm);
		localGeo.material = material;
		if (color)
		{
			double alpha = 1.0 - beta - gamma;
			double x = alpha * texture_mapping.ut.first +
				beta * texture_mapping.vt.first +
				gamma * texture_mapping.wt.first;
			
			double y = alpha * texture_mapping.ut.second +
				beta * texture_mapping.vt.second +
				gamma * texture_mapping.wt.second;
			y = 1.0 - y;
			assert(x < 1.0);
			assert(y < 1.0);
			assert(x > 0.0);
			assert(y > 0.0);
			color->x = texture->mat(int(y * texture->height), int(x * texture->width))[2] / 255.0;
			color->y = texture->mat(int(y * texture->height), int(x * texture->width))[1] / 255.0;
			color->z = texture->mat(int(y * texture->height), int(x * texture->width))[0] / 255.0;
		}
		return true;
	}
	return false;
}

bool Triangle::intersectWithLight(const Ray& ray)
{
	Vec direction = ray.direction;
	Point pos = ray.start_position;
	Normal norm;
	if (given_normal)
	{
		norm = normal;
	}
	else
	{
		norm = Vec::normalize(Vec::cross(c-a, b-a));
	}
	double tmp = direction * norm;
	if (fabs(tmp) < EPS)
	{
		return false;
	} 
	double t = (a * norm - pos * norm) / tmp;
	if(t < EPS || t > ray.t_max)
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
		return true;
	}
	return false;
}

void Triangle::print()
{
	printf("Tri: (%lf %lf %lf) (%lf %lf %lf) (%lf %lf %lf)\n", a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z);
}