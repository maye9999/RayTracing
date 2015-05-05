#include "common.h"
#include "phong.h"

#include <iostream>
#include <cmath>
using namespace std;


Color PhongBRDF::compute(const LocalGeometry& local_geo, const Ray& light_ray, const Color& light_color, const Vec& eye_dir)
{
	Color ret_color = ambient;
	Normal norm = Vec::normalize(local_geo.normal);
	Vec half = Vec::normalize(eye_dir + light_ray.direction);
	double nDotL = norm * light_ray.direction;
	if(nDotL > EPS)
		ret_color += Vec::mul(diffuse, light_color) * nDotL;	
	double nDotH = norm * half;
	if(nDotH > EPS)
		ret_color += Vec::mul(specular, light_color) * pow(nDotH, n);
	return ret_color;
}
