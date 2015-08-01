#ifndef __OBJFILE_H_
#define __OBJFILE_H_

#include "common.h"

class ObjFile : public File
{
public:
	ObjFile(const char* name);
	virtual bool parse(std::vector<Primitive*>& objects);

private:
	std::string file_name;
	std::map<std::string, std::pair<Material*, Texture*> > materials;
	bool parseMTLFile(const char* name);
};

#endif