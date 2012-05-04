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
		//nextPlanetId is seperate from planetCount because we don't want to end up reusing ids when planets get deleted
		int nextPlanetId, planetCount, planetLinkTotal;
		bool valid;
		int id;
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
		void move(float *vector);
		int getid();
		char *getName();
		void unOverLap();
		void printDistances();
};
#endif

