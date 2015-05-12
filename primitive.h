#ifndef __PRIMITIVE_H_
#define __PRIMITIVE_H_

#include "common.h"

class Primitive
{
public:
	Primitive()	{brdf = nullptr; material = nullptr;}
	virtual ~Primitive() {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo) = 0;
	virtual bool intersectWithLight(const Ray& ray) = 0;

	virtual BRDF* getBRDF() {return brdf;}
	virtual void setBRDF(BRDF* brdf_) {brdf = brdf_;}
	virtual void setMaterial(Material* material_) {material = material_;}
	virtual Material* getMaterial() {return material;}
protected:
	Material* material;
	BRDF* brdf;
};

class Triangle : public Primitive
{
public:
	Triangle(const Point& a, const Point& b, const Point& c) : Primitive(), a(a), b(b), c(c) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo);
	virtual bool intersectWithLight(const Ray& ray);
private:
	Point a, b, c;
};


class Sphere : public Primitive
{
public:
	Sphere(const Point& center_, double radius_) : Primitive(), center(center_), radius(radius_) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo);
	virtual bool intersectWithLight(const Ray& ray);
private:
	Point center;
	double radius;
};

#endif