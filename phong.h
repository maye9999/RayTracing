#ifndef __PHONG_H_
#define __PHONG_H_

#include "common.h"
#include "vec.h"

class PhongBRDF : public BRDF
{
public:
	PhongBRDF(const Vec& ambient, const Vec& diffuse, const Vec& specular, double n)
		: BRDF(), ambient(ambient), diffuse(diffuse), specular(specular), n(n) {}
	virtual ~PhongBRDF() {}
	virtual Color compute(const LocalGeometry& local_geo, const Ray& light_ray, const Color& light_color, const Vec& eye_dir);
private:
	Vec ambient;
	Vec diffuse;
	Vec specular;
	double n;
};

#endif