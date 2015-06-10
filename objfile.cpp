#include "objfile.h"
#include "primitive.h"
#include <iostream>
using namespace std;


ObjFile::ObjFile(const char* name)
	: file_name(name)
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
	char texture_name[256];
	vector<Point> vecVertices;
	vector<pair<double, double> > vecTextureVertices;
	vector<Normal> vecNormals;
	vecVertices.clear();
	vecTextureVertices.clear();
	objects.clear();
	int lineNumber = 0;
	Texture *texture = nullptr;
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
						// TODO:
						// CHANGE FILE NAME
						Triangle *t = new Triangle(vecVertices[a-1], vecVertices[b-1], vecVertices[c-1]);
						t->setTexture(texture);
						TextureMapping tm;
						tm.ut = vecTextureVertices[ta-1];
						tm.vt = vecTextureVertices[tb-1];
						tm.wt = vecTextureVertices[tc-1];
						t->setTextureMapping(tm);
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
						objects.push_back(t);

						int d;
						if(fscanf(fp, "%d", &d) == 1)
						{
							Triangle *t = new Triangle(vecVertices[a-1], vecVertices[c-1], vecVertices[d-1]);
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
					fscanf(fp, "%s", texture_name);
					texture = new Texture(texture_name);
					printf("Using Texture: %s\n", texture_name);
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
