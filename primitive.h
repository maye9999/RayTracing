#ifndef __PRIMITIVE_H_
#define __PRIMITIVE_H_

#include "common.h"

class Primitive
{
public:
	Primitive()	{brdf = nullptr;}
	virtual ~Primitive()	{delete brdf;}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo) = 0;
	virtual bool intersectWithLight(const Ray& ray) = 0;
	virtual const BRDF* getBRDF(const LocalGeometry& localGeo) = 0;
	virtual void setBRDF(BRDF* brdf_) = 0;
protected:
	BRDF* brdf;
};

class Triangle : public Primitive
{
public:
	Triangle(const Point& a, const Point& b, const Point& c) : Primitive(), a(a), b(b), c(c) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo);
	virtual bool intersectWithLight(const Ray& ray);
	virtual const BRDF* getBRDF(const LocalGeometry& localGeo);
	virtual void setBRDF(BRDF* brdf_);
private:
	Point a, b, c;
};



#endif