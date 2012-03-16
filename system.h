#if !defined(__CLASS_SYSTEM__)
#define __CLASS_SYSTEM__
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <cstring>
#include <limits.h>
#include "starpirate.h"
#include "planet.h"
#include "link.h"

using namespace std;

class System
{
	private:
		char *name;
		list<Planet*> planetList;
		int planetCount, planetLinkTotal;
		bool valid;
		float centre[3];
		float rotation[3];
		list<Link*> links;
		int linkCount;
	public:
		System(const char *Name, int numPlanets, float *syscentre);
		~System();
		bool isValid();
		int drawPlanets();
		int getPlanetCount();
		list<Planet*>::iterator getLinksBegin();
		list<Planet*>::iterator getLinksEnd();
		GLfloat getX();
		GLfloat getY();
		GLfloat getZ();
		char *getName();
		void unOverLap();
};
#endif

