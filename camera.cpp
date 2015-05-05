#include "common.h"

void Camera::setCamera(Point eye, Point center, Vec up, double fov)
{
	this->eye = eye;
	this->center = center;
	this->up = up;
	w = Vec::normalize(eye - center);
	u = Vec::normalize(Vec::cross(up, w));
	v = Vec::normalize(Vec::cross(w, u));
	fovy_tan = tan(fov * PI / 2.0 / 180.0);
	fovx_tan = fovy_tan * width / height;
}

void Camera::generateRay(const Sample& sample, Ray& ray)
{
	double alpha = fovx_tan * (sample.x - width / 2.0) / (width / 2.0);
	double beta = fovy_tan * (sample.y - height / 2.0) / (height / 2.0);
	ray.direction = Vec::normalize(alpha * u + beta * v - w);
	ray.start_position = eye;
	ray.t_max = T_MAX;
	ray.t_min = 0;
}