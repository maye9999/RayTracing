#ifndef __KDTREE_H_
#define __KDTREE_H_

#include "common.h"
#include "primitive.h"

// Node for KDTree
struct KDNode
{
	int dim;		// 0 for x and 1 for y and 2 for z
	KDNode* left;
	KDNode* right;
	std::vector<Primitive*> objects;
	Point small, big;					// Left-Down-Back point and Right-Up-Front point

	bool intersect(const Ray& ray, double t_hit);	// intersect the node with a light ray
};

class KDTree
{
public:
	KDTree(const std::vector<Primitive*>& objects) : _objects(objects) {}
	~KDTree();

	void buildTree();
	// Find the nearest primitive in KDTree, return false if nothing hits
	bool findNearestPrimitive(KDNode* root, const Ray& ray, double& t, LocalGeometry& local_geo, BRDF*& brdf, Color* texture_color);
	bool intersectWithLight(KDNode* root, const Ray& ray);
	KDNode* _root;
private:
	std::vector<Primitive*> _objects;

	void divideAndBuild(KDNode* root);		// Recursively build tree
};


#endif