#include "vec.h"
#include "mat.h"
#include <iostream>

using namespace std;

int main()
{
	Vec a(1, 1, 1);
	Vec b = Vec::normalize(a);
	cout << b << endl;
	Mat s;
	s(0, 0) = 1;
	cout << s << endl;
	return 0;
}