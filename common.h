#ifndef __COMMON_H_
#define __COMMON_H_

#include "vec.h"

#include <climits>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

#define T_MAX INT_MAX
const double PI = 3.141592654;
const double EPS = 1e-6;

typedef Vec Normal;
typedef Vec Point;
typedef Vec Color;

class Scene;
class Primitive;

struct Sample
{
	double x, y;
	Sample(double x, double y) : x(x), y(y) {}
};

struct Material
{
	Vec ambient;
	Vec diffuse;
	Vec specular;
	Vec reflect;
	Vec refract;
	double n;
	double refract_index;

	Material() {}
	Material(const Vec& ambient, const Vec& diffuse, const Vec& specular, double n,
			Vec reflect = Vec(0, 0, 0), Vec refract = Vec(0, 0, 0), double refract_index = 0.0)
		: ambient(ambient), diffuse(diffuse), specular(specular), n(n),
		reflect(reflect), refract(refract), refract_index(refract_index) {}
};

/************************************************************************/
/* It Represents the ray P(t) = pos + t*dir, where t in [t_min, t_max]  */
/************************************************************************/
struct Ray
{
	Point start_position;
	Vec direction;
	double t_min, t_max;
	Ray(Point start_position = Point(), Vec direction = Vec(), double t_min = 0, double t_max = T_MAX)
		: start_position(start_position), direction(direction), t_min(t_min), t_max(t_max) {}
};

struct LocalGeometry
{
	Point pos;
	Normal normal;
	Material* material;
};

class Light
{
	
	//********************************************************
	// Generate a Light ray from localGeo.pos to Light source
	//********************************************************
public:
	virtual void generateLightRay(const LocalGeometry& localGeo, Ray& light_ray, Color& color) = 0;
};

class BRDF
{
public:
	BRDF() {}
	virtual ~BRDF() {}
	
	virtual Color compute(const LocalGeometry& local_geo,
						const Ray& light_ray,
						const Color& light_color,
						const Vec& eye_dir) = 0;
private:

};

struct Sampler
{
	Sampler(int width, int height) : width(width), height(height) {}
	bool generateNextSample(Sample& sample);
	int width, height;
};


struct Camera
{
	Camera(int width, int height) : width(width), height(height) {}
	void setCamera(Point eye, Point center, Vec up, double fov);
	void generateRay(const Sample& sample, Ray& ray);
	Vec w, u, v;
	Point eye, center, up;
	int width, height;
	double fovx_tan, fovy_tan;
};


struct RayTracer
{
	RayTracer(Scene* s) : scene(s) {}
	void trace(const Ray& ray, int depth, Color& color, bool refracted);
	Scene* scene;
};

class Film
{
public:
	Film(int width, int height);
	void commit(const Sample& sample, const Color& color);
	void writeImage();

private:
	cv::Mat_<cv::Vec3b> mat;
	int height, width;
};

class Scene
{
public:
	Scene(int width, int height);
	void render();

	std::vector<Primitive*> objects;
	std::vector<Light*> light_objects;
	Camera camera;
private:
	Film film;
	Sample sample;
	Sampler sampler;
	
	RayTracer ray_tracer;
	Ray ray;
	Color color;
};



#endif