#ifndef __PHONG_H_
#define __PHONG_H_

#include "common.h"
#include "vec.h"

// Phong Model for rendering
class PhongBRDF : public BRDF
{
public:
	PhongBRDF()	: BRDF() {}
	virtual ~PhongBRDF() {}
	virtual Color compute(const LocalGeometry& local_geo, const Ray& light_ray, const Color& light_color, const Vec& eye_dir);
};

#endif