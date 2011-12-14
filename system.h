#if !defined(__CLASS_SYSTEM__)
#define __CLASS_SYSTEM__
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <cstring>
#include <limits.h>
#include "starpirate.h"
#include "planet.h"

using namespace std;

class System
{
	private:
		char *name;
		list<Planet*> planetList;
		bool valid;
	public:
		System(const char *Name, int numPlanets);
		~System();
		bool isValid();
		int drawPlanets();
};
#endif

