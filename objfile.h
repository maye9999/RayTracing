#ifndef __OBJFILE_H_
#define __OBJFILE_H_

#include "common.h"
#include "SimpleOBJ/SimpleObject.h"

class ObjFile : public File, private SimpleOBJ::CSimpleObject
{
public:
	ObjFile(const char* name);
	virtual void parse(std::vector<Primitive*>& objects);

private:
};

#endif