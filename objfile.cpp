#include "objfile.h"
#include "primitive.h"
#include <iostream>
using namespace std;


ObjFile::ObjFile(const char* name)
	: file_name(name), texture(nullptr)
{

}

bool ObjFile::parse(std::vector<Primitive*>& objects)
{
	FILE* fp = fopen(file_name.c_str(), "r");
	if(fp==NULL)
	{
		printf("Error: Loading %s failed.\n",file_name.c_str());
		return false;
	}
	char buf[256];
	vector<Point> vecVertices;
	vector<pair<double, double> > vecTextureVertices;
	vector<Normal> vecNormals;
	Material* material = nullptr;
	int lineNumber = 0;
	while(fscanf(fp, "%s", buf) != EOF)
	{
		lineNumber ++;
		switch(buf[0])
		{
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), fp);
			break;
		case 'v':				/* v, vn, vt */
			switch(buf[1])
			{
			case '\0':			    /* vertex */
				{
					Point vP;
					if(fscanf(fp, "%lf %lf %lf", &vP.x, &vP.y, &vP.z)==3)
					{
						vecVertices.push_back(vP);
					}
					else 
					{
						printf("Error: Wrong Number of Values(Should be 3). at Line %d\n",lineNumber);
						return false;
					}
				}
				break;
			case 't':
				{
					double x, y;
					if(fscanf(fp, "%lf %lf", &x, &y)==2)
					{
						if(x < 0)
							x = 1 - x;
						if(y < 0)
							y = 1 - y;
						if(x > 1)
							x = x - floor(x);
						if(y > 1)
							y = y - floor(y);
						vecTextureVertices.push_back(make_pair(x, y));
					}
					else 
					{
						printf("Error: Wrong Number of Values(Should be 3). at Line %d\n",lineNumber);
						return false;
					}
				}
				break;
			case 'n':
				{
					Normal norm;
					if(fscanf(fp, "%lf %lf %lf", &norm.x, &norm.y, &norm.z)==3)
					{
						vecNormals.push_back(norm);
					}
					else 
					{
						printf("Error: Wrong Number of Values(Should be 3). at Line %d\n",lineNumber);
						return false;
					}
				}
				break;
			default:
				/* eat up rest of line */
				fgets(buf, sizeof(buf), fp);
				break;
			}
			break;

		case 'f':				/* face */
			{
				int n;
				int a, b, c;
				int ta, tb, tc;
				if(fscanf(fp, "%s", buf)!=1)
				{
					printf("Error: Wrong Face at Line %d\n",lineNumber);
					return false;
				}

				/* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
				if (strstr(buf, "//"))
				{
					/* v//n */
					if( sscanf(buf, "%d//%d", &a,&n) ==2  &&
						fscanf(fp, "%d//%d", &b, &n) ==2  &&
						fscanf(fp, "%d//%d", &c, &n) ==2)
					{
						Triangle *t = new Triangle(vecVertices[a-1], vecVertices[b-1], vecVertices[c-1]);
						if(material)
							t->setMaterial(material);
						objects.push_back(t);
					}
					else
					{
						printf("Error: Wrong Face at Line %d\n",lineNumber);
						return false;
					}
				}
				else if (sscanf(buf, "%d/%d/%d", &a, &ta, &n) == 3)
				{
					/* v/t/n */
					if( fscanf(fp, "%d/%d/%d", &b, &tb, &n) ==3 &&
						fscanf(fp, "%d/%d/%d", &c, &tc, &n) ==3 )
					{
						Triangle *t = new Triangle(vecVertices[a-1], vecVertices[b-1], vecVertices[c-1]);
						t->setTexture(texture);
						TextureMapping tm;
						tm.ut = vecTextureVertices[ta-1];
						tm.vt = vecTextureVertices[tb-1];
						tm.wt = vecTextureVertices[tc-1];
						t->setTextureMapping(tm);
						if(material)
							t->setMaterial(material);
						objects.push_back(t);
						int d, td;
						if(fscanf(fp, "%d/%d/%d", &d, &td, &n) == 3)
						{
							Triangle *t = new Triangle(vecVertices[a-1], vecVertices[c-1], vecVertices[d-1]);
							t->setTexture(texture);
							TextureMapping tm;
							tm.ut = vecTextureVertices[ta-1];
							tm.vt = vecTextureVertices[tc-1];
							tm.wt = vecTextureVertices[td-1];
							t->setTextureMapping(tm);
							if(material)
								t->setMaterial(material);
							objects.push_back(t);
						}
					}
					else
					{
						printf("Error: Wrong Face at Line %d\n",lineNumber);
						return false;
					}
				}
				else if (sscanf(buf, "%d/%d", &a, &ta) == 2)
				{
					/* v/t */
					if( fscanf(fp, "%d/%d", &b, &tb) == 2 &&
						fscanf(fp, "%d/%d", &c, &tc) == 2 )
					{
						Triangle *t = new Triangle(vecVertices[a-1], vecVertices[b-1], vecVertices[c-1]);
						t->setTexture(texture);
						TextureMapping tm;
						tm.ut = vecTextureVertices[ta-1];
						tm.vt = vecTextureVertices[tb-1];
						tm.wt = vecTextureVertices[tc-1];
						t->setTextureMapping(tm);
						if(material)
							t->setMaterial(material);
						objects.push_back(t);

						int d, td;
						if(fscanf(fp, "%d/%d", &d, &td) == 2)
						{
							Triangle *t = new Triangle(vecVertices[a-1], vecVertices[c-1], vecVertices[d-1]);
							t->setTexture(texture);
							TextureMapping tm;
							tm.ut = vecTextureVertices[ta-1];
							tm.vt = vecTextureVertices[tc-1];
							tm.wt = vecTextureVertices[td-1];
							t->setTextureMapping(tm);
							if(material)
								t->setMaterial(material);
							objects.push_back(t);
						}

					}
					else
					{
						printf("Error: Wrong Face at Line %d\n",lineNumber);
						return false;
					}
				}
				else
				{
					/* v */
					if( sscanf(buf, "%d", &a) ==1 && 
						fscanf(fp, "%d", &b)  ==1 &&
						fscanf(fp, "%d", &c)  ==1 )
					{
						Triangle *t = new Triangle(vecVertices[a-1], vecVertices[b-1], vecVertices[c-1]);
						if(material)
							t->setMaterial(material);
						objects.push_back(t);

						int d;
						if(fscanf(fp, "%d", &d) == 1)
						{
							Triangle *t = new Triangle(vecVertices[a-1], vecVertices[c-1], vecVertices[d-1]);
							if(material)
								t->setMaterial(material);
							objects.push_back(t);
						}
					}
					else
					{
						printf("Error: Wrong Face at Line %d\n",lineNumber);
						return false;
					}
				}
			}

			break;
		case 'u':
			{
				if(buf[1] == 's' && buf[2] == 'e' && buf[3] == 'm' && buf[4] == 't' && buf[5] == 'l')
				{
					char mtl_name[256];
					fscanf(fp, "%s", mtl_name);
					try
					{
						material = materials[mtl_name];
						printf("Material: %s\n", mtl_name);
					}
					catch(exception& e)
					{
						printf("Material %s Doesn't Exist!\n", mtl_name);
					}
				}
			}
			break;

		case 'm':
			{
				if(strcmp(buf, "mtllib") == 0)
				{
					char mtl_name[256];
					fscanf(fp, "%s", mtl_name);
					if(!parseMTLFile(mtl_name))
						return false;
				}
			}
			break;

		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), fp);
			break;
		}
	}

	printf("Loading from %s successfully.\n",file_name.c_str());
	printf("Vertex Number = %d\n",vecVertices.size());
	printf("Triangle Number = %d\n",objects.size());
	fclose(fp);
	return true;
}

bool ObjFile::parseMTLFile(const char* name)
{
	FILE* fp = fopen(name, "r");
	if(fp==NULL)
	{
		printf("Error: Loading %s failed.\n", name);
		return false;
	}
	char buf[256];
	int lineNumber = 0;
	Material* m = nullptr;
	char m_name[256];
	while(fscanf(fp, "%s", buf) != EOF)
	{
		lineNumber ++;
		switch(buf[0])
		{
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), fp);
			break;
		case 'n':
			{
				if(strcmp(buf, "newmtl") == 0)
				{
					if (m)
					{
						materials[m_name] = m;
					}
					fscanf(fp, "%s", m_name);
					m = new Material;
				}
				else
				{
					fgets(buf, sizeof(buf), fp);
				}
			}
			break;

		case 'K':
			{
				switch (buf[1])
				{
				case 'a':
					{
						Color c;
						if(fscanf(fp, "%lf %lf %lf", &c.x, &c.y, &c.z)==3)
						{
							m->ambient = c;
						}
						else
						{
							printf("Error: Wrong Material at Line %d\n",lineNumber);
							return false;
						}
					}
					break;
				case 'd':
					{
						Color c;
						if(fscanf(fp, "%lf %lf %lf", &c.x, &c.y, &c.z)==3)
						{
							m->diffuse = c;
						}
						else
						{
							printf("Error: Wrong Material at Line %d\n",lineNumber);
							return false;
						}
					}
					break;
				case 's':
					{
						Color c;
						if(fscanf(fp, "%lf %lf %lf", &c.x, &c.y, &c.z)==3)
						{
							m->specular = c;
						}
						else
						{
							printf("Error: Wrong Material at Line %d\n",lineNumber);
							return false;
						}
					}
					break;
				default:
					printf("Ignoring argument: %s\n", buf);
					fgets(buf, sizeof(buf), fp);
					break;
				}
			}
			break;

		case 'N':
			{
				if(buf[1] == 's')
				{
					fscanf(fp, "%lf", &m->n);
				}
				else
				{
					printf("Ignoring argument: %s\n", buf);
					fgets(buf, sizeof(buf), fp);
				}
			}
			break;
		case 'i':
			{
				if(strcmp(buf, "illum") == 0)
				{
					fgets(buf, sizeof(buf), fp);
				}
				else
				{
					printf("Ignoring argument: %s\n", buf);
					fgets(buf, sizeof(buf), fp);
				}
			}
			break;
		case 'm':
			{
				if (strcmp(buf, "map_Kd") == 0)
				{
					char texture_name[256];
					fscanf(fp, "%s", texture_name);
					texture = new Texture(texture_name);
				} 
				else
				{
					printf("Ignoring argument: %s\n", buf);
					fgets(buf, sizeof(buf), fp);
				}
			}
			break;
		default:
			/* eat up rest of line */
			printf("Ignoring argument: %s\n", buf);
			fgets(buf, sizeof(buf), fp);
			break;
		}
	}
	if (m)
	{
		materials[m_name] = m;
	}
	return true;
}
