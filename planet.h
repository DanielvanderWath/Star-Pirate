#if !defined(__PLANET_H__)
#define __PLANET_H__
#include "starpirate.h"
#include <list>

using namespace std;

class Planet
{
	private:
		list<Planet*> links;
		unsigned int planetColour;
		int id;
		float coords[3];
	public:
		Planet(unsigned int Colour, float *location);
		void addLink(Planet *link);
		void printLinks();
		int getid();
		unsigned int getColour();
		GLfloat getX();
		GLfloat getY();
		GLfloat getZ();
};
#endif

