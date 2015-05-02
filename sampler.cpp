#include "common.h"
#include <iostream>

bool Sampler::generateNextSample(Sample& sample)
{
	sample.y += 1;
	if(sample.y > width)
	{
		if(sample.x == height / 2 + 0.5)
			std::cerr << "Half Way There!\n";
		if(sample.x == height / 4 + 0.5)
			std::cerr << "A Quarter Completed!\n";
		if(sample.x == 3 * height / 4 + 0.5)
			std::cerr << "Three Quarters Completed!\n";
		sample.y = 0.5;
		sample.x += 1;
		if(sample.x > height)
		{
			return false;
		}

	}
	return true;
}