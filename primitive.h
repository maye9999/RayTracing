#ifndef __PRIMITIVE_H_
#define __PRIMITIVE_H_

#include "common.h"

struct TextureMapping
{
	std::pair<double, double> ut, vt, wt;
};

class Primitive
{
public:
	Primitive()	{brdf = nullptr; material = nullptr; texture = nullptr;}
	virtual ~Primitive() {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo, Color* color) = 0;
	virtual bool intersectWithLight(const Ray& ray) = 0;

	virtual BRDF* getBRDF() {return brdf;}
	virtual void setBRDF(BRDF* brdf_) {brdf = brdf_;}
	virtual void setMaterial(Material* material_) {material = material_;}
	virtual Material* getMaterial() {return material;}
	virtual void setTexture(Texture* texture) {this->texture = texture;}
	virtual void setTextureMapping(const TextureMapping& texture_mapping) {this->texture_mapping = texture_mapping;}

	bool hasTexture() {return texture != nullptr;}

	virtual void print() = 0;
protected:
	Material* material;
	BRDF* brdf;
	Texture* texture;
	TextureMapping texture_mapping;
};

class Triangle : public Primitive
{
public:
	Triangle(const Point& a, const Point& b, const Point& c) : Primitive(), a(a), b(b), c(c), given_normal(false) {}
	Triangle(const Point& a, const Point& b, const Point& c, const Vec& n) : Primitive(), a(a), b(b), c(c), normal(n), given_normal(true) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo, Color* color);
	virtual bool intersectWithLight(const Ray& ray);
	virtual void print();
private:
	Point a, b, c;
	Vec normal;
	bool given_normal;
};


class Sphere : public Primitive
{
public:
	Sphere(const Point& center_, double radius_) : Primitive(), center(center_), radius(radius_) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo, Color* color);
	virtual bool intersectWithLight(const Ray& ray);
	virtual void print();
private:
	Point center;
	double radius;
};

#endif