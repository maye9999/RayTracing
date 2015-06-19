#ifndef __COMMON_H_
#define __COMMON_H_

#include "vec.h"

#include <climits>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <mutex>

#define T_MAX INT_MAX
const double PI = 3.141592654;
const double EPS = 1E-7;

typedef Vec Normal;
typedef Vec Point;
typedef Vec Color;

class Scene;
class Primitive;
class KDTree;
class PlaneLight;

struct Sample
{
	double x, y;
	Sample(double x, double y) : x(x), y(y) {}
};

struct Texture
{
	cv::Mat_<cv::Vec3b> mat;
	int width, height;
	Texture(std::string file_name)
	{
		mat = cv::imread(file_name);
		width = mat.cols;
		height = mat.cols;
	}
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
			Vec reflect = Vec(0, 0, 0), Vec refract = Vec(0, 0, 0), double refract_index = 0.0, Texture* texture = nullptr)
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
	bool inside;
};

class Light
{
	
	
public:
	Light(Scene* s) : scene(s) {}

	virtual bool computeShadow(const LocalGeometry& localGeo, Ray& light_ray, Color& light_color) = 0;
	
protected:
	Scene* scene;
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
	void setCamera(Point eye, Point center, Vec up, double fov, bool DOF = false, double aperture = 0, double distance = 0);
	void generateRay(const Sample& sample, Ray& ray);
	Vec w, u, v;
	Point eye, center, up;
	int width, height;
	double aperture;
	double focal_distance;
	double fovx_tan, fovy_tan;
	bool use_DOF;
};


struct RayTracer
{
	RayTracer(Scene* s, KDTree* k) : scene(s), kd_tree(k) {}
	void trace(const Ray& ray, int depth, Color& color);
	Scene* scene;
	KDTree* kd_tree;
};

class Film
{
public:
	Film(int width, int height, bool super_sampling);
	void commit(const Sample& sample, const Color& color);
	void writeImage();

private:
	bool super_sampling;
	cv::Mat_<cv::Vec3b> mat;
	int height, width;

	std::mutex m_mutex;
};

class File
{
public:
	virtual bool parse(std::vector<Primitive*>& objects) = 0;
};

class Scene
{
public:
	Scene(int width, int height, bool super_sampling = false, int DOF_times = 0);
	void render(int cores = 1, bool use_kd_tree = false);

	bool loadFile(File* f)	{return f->parse(objects);}
	void setGlobalMaterial(Material* material);
	void setGlobalBRDF(BRDF* brdf);

	void renderPartial(std::pair<double, double> p);


	std::vector<Primitive*> objects;
	std::vector<Light*> light_objects;
	std::vector<PlaneLight*> plane_lights;
	Camera camera;
	KDTree* kd_tree;
private:
	Film film;
	Sample sample;	
	RayTracer ray_tracer;	

	bool super_sampling;
	int width, height;
	int DOF_times;
	std::mutex m;
	
};

#endif