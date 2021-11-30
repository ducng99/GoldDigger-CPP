#include "Utilities.h"
#include <cstdlib>

int Utilities::GenRand(int min, int max)
{
	return rand() % (max - min + 1) + min;
}
