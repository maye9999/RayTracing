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

// A Sample from image plane
struct Sample
{
	double x, y;
	Sample(double x, double y) : x(x), y(y) {}
};

struct Texture
{
	cv::Mat_<cv::Vec3b> mat;		// Color Matrix
	int width, height;
	Texture(std::string file_name)
	{
		// Use OpenCV to read image
		mat = cv::imread(file_name);
		width = mat.cols;
		height = mat.cols;
	}
};

// Object's Material
struct Material
{
	Vec ambient;
	Vec diffuse;
	Vec specular;
	Vec reflect;
	Vec refract;
	double n;				// specular coefficient
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

// A structure for intercept point
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

	// Compute whether shadow or light
	virtual bool computeShadow(const LocalGeometry& localGeo, Ray& light_ray, Color& light_color) = 0;
	
protected:
	Scene* scene;
};


class BRDF
{
public:
	BRDF() {}
	virtual ~BRDF() {}
	// Get the color of that point
	virtual Color compute(const LocalGeometry& local_geo,
						const Ray& light_ray,
						const Color& light_color,
						const Vec& eye_dir) = 0;
private:

};

struct Camera
{
	Camera(int width, int height) : width(width), height(height) {}
	void setCamera(Point eye, Point center, Vec up, double fov, bool DOF = false, double aperture = 0, double distance = 0);
	void generateRay(const Sample& sample, Ray& ray);	// Generate a ray from camera to a sample
	Vec w, u, v;			// Coordinates of Camera view system
	Point eye, center, up;
	int width, height;
	double aperture;			// for DOF
	double focal_distance;		// distance between focus and the eye
	double fovx_tan, fovy_tan;	// Field of View
	bool use_DOF;
};


struct RayTracer
{
	RayTracer(Scene* s, KDTree* k) : scene(s), kd_tree(k) {}
	void trace(const Ray& ray, int depth, Color& color);		// Trace the color
	Scene* scene;
	KDTree* kd_tree;
};

// Output matrix to image
class Film
{
public:
	Film(int width, int height, bool super_sampling);
	void commit(const Sample& sample, const Color& color);	// update the color of that sample
	void writeImage();

private:
	bool super_sampling;
	cv::Mat_<cv::Vec3b> mat;
	int height, width;
};

class File
{
public:
	// Parse objects from file
	virtual bool parse(std::vector<Primitive*>& objects) = 0;
};

class Scene
{
public:
	Scene(int width, int height, bool super_sampling = false, int DOF_times = 0);
	void render(int cores = 1, bool use_kd_tree = false);	// Get the result

	bool loadFile(File* f)	{return f->parse(objects);}
	void setGlobalMaterial(Material* material);
	void setGlobalBRDF(BRDF* brdf);

	void renderPartial(std::pair<double, double> p);	// Render part of the image, used by Multi-threads


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
	int DOF_times;			// the times that sample from a DOF	
};

#endif