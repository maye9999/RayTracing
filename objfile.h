#ifndef __OBJFILE_H_
#define __OBJFILE_H_

#include "common.h"
#include "SimpleOBJ/SimpleObject.h"

class ObjFile : public File
{
public:
	ObjFile(const char* name);
	virtual bool parse(std::vector<Primitive*>& objects);

private:
	std::string file_name;
};

#endif