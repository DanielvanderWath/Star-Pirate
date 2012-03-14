#if !defined(__CLASS_LINK__)
#define __CLASS_LINK__
#include "starpirate.h"
#include "planet.h"

using namespace std;

class Link
{
	private:
		Planet* A;
		Planet* B;
	public:
		Link(Planet *a, Planet *b);
		~Link();
		void print();
		Planet* getA();
		Planet* getB();
};
#endif

