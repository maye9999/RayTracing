#include "common.h"
#include "light.h"
#include "objfile.h"
#include "phong.h"
#include "primitive.h"
#include <iostream>
#include <thread>

using namespace std;

int mainb()
{
	Scene scene(800, 600, false);
	//Scene scene(800, 600, false);

	PhongBRDF brdf;
	Material material(Vec(0.0, 0.1, 0.0), Vec(0.2, 0.4, 0.2), Vec(0.1, 0.1, 0.1), 20, Vec(0, 0, 0)/*, Vec(0.9, 0.9, 0.9), 1.5*/);
	Material mat_refr(Color(0.15, 0.15, 0.15), Color(0., 0., 0.), Color(0.4, 0.4, 0.4), 20, Color(0, 0, 0), Color(0.9, 0.9, 0.9), 1.5);
	Material mat_refl(Color(0.1, 0.1, 0.1), Color(0., 0., 0.), Color(0.3, 0.3, 0.3), 20, Color(0.9, 0.9, 0.9));
	//Material material(Vec(1, 1, 1), Vec(0.1, 0.1, 0.1), Vec(0.1, 0.1, 0.1), 20);

	Material m(Vec(0.6, 0.6, 0.6), Vec(0.5, 0.5, 0.6), Vec(0.3, 0.3, 0.3), 20);

	ObjFile* file = new ObjFile("bg3_obj.obj");
	//Texture tt("space_frigate_6_color.bmp");
	if(!scene.loadFile(file))
	{
		return -1;
	}
	for(auto p : scene.objects)
	{
		auto pp = (Triangle*)p;
// 		pp->a.x += 1.2;
// 		pp->a.y += 1;
// 		pp->b.x += 1.2;
// 		pp->b.y += 1;
// 		pp->c.x += 1.2;
// 		pp->c.y += 1;
// 		pp->a.z += 15;
// 		pp->b.z += 15;
// 		pp->c.z += 15;
	//	pp->setTexture(&tt);
	}
//	scene.setGlobalMaterial(&m);


	Triangle s1(Point(-80, -6, -80), Point(-80, -6, 80), Point(80, -6, 80));
	Triangle s2(Point(-80, -6, -80), Point(80, -6, -80), Point(80, -6, 80));
	Material m2(Color(0.1, 0.1, 0.1), Color(0.1, 0.1, 0.1), Color(0.1, 0.1, 0.1), 20, Color(0.9, 0.9, 0.9));
	s1.setMaterial(&m2);
	s2.setMaterial(&m2);

	Texture * texture = new Texture("floor.bmp");
	TextureMapping *tm2 = new TextureMapping;
	tm2->ut = make_pair(0, 0);
	tm2->vt = make_pair(1, 0);
	tm2->wt = make_pair(1, 1);
	TextureMapping *tm = new TextureMapping;
	tm->ut = make_pair(0, 0);
	tm->vt = make_pair(0, 1);
	tm->wt = make_pair(1, 1);
	s1.setTexture(texture);
	s2.setTexture(texture);
	s1.setTextureMapping(*tm);
	s2.setTextureMapping(*tm2);

	//scene.objects.push_back(&s1);
	//scene.objects.push_back(&s2);

	scene.setGlobalBRDF(&brdf);

	PointLight light(&scene, Point(100, 100, 90), Color(0.9, 0.9, 0.9));
	PointLight light2(&scene, Point(20, 30, -20), Color(0.9, 0.9, 0.9));
	PointLight light3(&scene, Point(20, 50, 20), Color(0.9, 0.9, 0.9));
	scene.light_objects.push_back(&light);
	scene.light_objects.push_back(&light2);
	//scene.light_objects.push_back(&light3);

	scene.camera.setCamera(Point(20, 10, -20), Point(0, 0, 0), Vec(0, 1, 0), 50);
	scene.render(8, true);
	return 0;
}