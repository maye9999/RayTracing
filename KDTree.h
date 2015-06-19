#ifndef __KDTREE_H_
#define __KDTREE_H_

#include "common.h"
#include "primitive.h"


struct KDNode
{
	int dim;
	KDNode* left;
	KDNode* right;
	std::vector<Primitive*> objects;
	Point small, big;

	bool intersect(const Ray& ray, double t_hit);
};

class KDTree
{
public:
	KDTree(const std::vector<Primitive*>& objects) : _objects(objects) {}
	~KDTree();

	void buildTree();
	bool findNearestPrimitive(KDNode* root, const Ray& ray, double& t, LocalGeometry& local_geo, BRDF*& brdf, Color* texture_color);
	bool intersectWithLight(KDNode* root, const Ray& ray);
	KDNode* _root;
private:
	std::vector<Primitive*> _objects;

	void divideAndBuild(KDNode* root);	
};


#endif