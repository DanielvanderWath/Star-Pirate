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
		int linkCount;
		float coords[3];
	public:
		Planet(unsigned int Colour, float *location, int *planetCount);
		void addLink(Planet *link);
		void printLinks();
		int getid();
		unsigned int getColour();
		int getLinkCount();
		list<Planet*>::iterator getLinksBegin();
		list<Planet*>::iterator getLinksEnd();
		GLfloat getX();
		GLfloat getY();
		GLfloat getZ();
};
#endif

