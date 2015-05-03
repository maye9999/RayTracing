#include "vec.h"
#include "mat.h"
#include "common.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int main()
{
	Scene scene(500, 500);
	Triangle t(Point(0, 0, 0), Point(0, 1, 0), Point(0, 1, 1));
	scene.objects.insert(scene.objects.end(), &t);
	scene.camera.setCamera(Point(1, 1, 0), Point(0, 1, 0), Vec(0, 0, 1), 90);
	scene.render();
	return 0;
}