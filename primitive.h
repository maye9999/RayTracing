#ifndef __PRIMITIVE_H_
#define __PRIMITIVE_H_

#include "common.h"

class Primitive
{
public:
	Primitive()	{brdf = nullptr;}
	virtual ~Primitive() {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo) = 0;
	virtual bool intersectWithLight(const Ray& ray) = 0;
	virtual BRDF* getBRDF(const LocalGeometry& localGeo) = 0;
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
	virtual BRDF* getBRDF(const LocalGeometry& localGeo);
	virtual void setBRDF(BRDF* brdf_);
private:
	Point a, b, c;
};


class Sphere : public Primitive
{
public:
	Sphere(const Point& center_, double radius_) : Primitive(), center(center_), radius(radius_) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo);
	virtual bool intersectWithLight(const Ray& ray);
	virtual BRDF* getBRDF(const LocalGeometry& localGeo);
	virtual void setBRDF(BRDF* brdf_);
private:
	Point center;
	double radius;
};

#endif