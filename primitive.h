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

	virtual Point getXLeft() = 0;
	virtual Point getXRight() = 0;
	virtual Point getYLeft() = 0;
	virtual Point getYRight() = 0;
	virtual Point getZLeft() = 0;
	virtual Point getZRight() = 0;

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
	Triangle(const Point& aa, const Point& bb, const Point& cc) 
		: Primitive(), a(aa), b(bb), c(cc), given_normal(false) 
	{
		if (a.x <= b.x && a.x <= c.x)
			xl = &a;
		else if (b.x <= a.x && b.x <= c.x)
			xl = &b;
		else
			xl = &c;

		if (a.x >= b.x && a.x >= c.x)
			xr = &a;
		else if (b.x >= a.x && b.x >= c.x)
			xr = &b;
		else
			xr = &c;

		if (a.y <= b.y && a.y <= c.y)
			yl = &a;
		else if (b.y <= a.y && b.y <= c.y)
			yl = &b;
		else
			yl = &c;

		if (a.y >= b.y && a.y >= c.y)
			yr = &a;
		else if (b.y >= a.y && b.y >= c.y)
			yr = &b;
		else
			yr = &c;

		if (a.z <= b.z && a.z <= c.z)
			zl = &a;
		else if (b.z <= a.z && b.z <= c.z)
			zl = &b;
		else
			zl = &c;

		if (a.z >= b.z && a.z >= c.z)
			zr = &a;
		else if (b.z >= a.z && b.z >= c.z)
			zr = &b;
		else
			zr = &c;
	}
	Triangle(const Point& a, const Point& b, const Point& c, const Vec& n) : Primitive(), a(a), b(b), c(c), normal(n), given_normal(true) {}
	virtual bool intersect(const Ray& ray, double& t_hit, LocalGeometry& localGeo, Color* color);
	virtual bool intersectWithLight(const Ray& ray);
	virtual void print();

	virtual Point getXLeft() {return *xl;}
	virtual Point getXRight() {return *xr;}
	virtual Point getYLeft() {return *yl;}
	virtual Point getYRight() {return *yr;}
	virtual Point getZLeft() {return *zl;}
	virtual Point getZRight() {return *zr;}

	Point a, b, c;
private:
	Point *xl, *xr, *yl, *yr, *zl, *zr;
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

	virtual Point getXLeft();
	virtual Point getXRight();
	virtual Point getYLeft();
	virtual Point getYRight();
	virtual Point getZLeft();
	virtual Point getZRight();
private:
	Point center;
	double radius;
};


#endif