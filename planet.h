#if !defined(__PLANET_H__)
#define __PLANET_H__
#include "starpirate.h"
#include <list>

using namespace std;

class Planet
{
	private:
		unsigned int planetColour;
		int id;
		float coords[3];
	public:
		Planet(unsigned int Colour, float *location, int *planetCount);
		int getid();
		unsigned int getColour();
		GLfloat getX();
		GLfloat getY();
		GLfloat getZ();
		void move(float *vector);
};
#endif

