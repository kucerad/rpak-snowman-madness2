#ifndef __PHYSICS
#define __PHYSICS

#include "Vector.h"
#include "timer.h"

//parametry sveta
#define WORLD_SIZE_2  30.0
#define WORLD_HEIGHT  30.0
#define ZEME 0.0 // vyska zeme

#define DEG_TO_RAD (3.14159/180.0) // konstanta pro prevod stupnu na radiany

double RYCHLOST_KOULE = 0.7; //rychlost, jakou ma na pocatku vrzena koule

// nastaveni fyziky
CVector3D	GRAVITACE(0.0, -9.81, 0.0);
CVector3D	VITR(0.0,0.0,1.0);
double		HUSTOTA_PROSTREDI = 0.6;
double		KOEF_ODPORU = 0.6;
double		KOEF_TRENI = 0.3;
double		dt = 0.001;
double		EPSILON = 0.00000001;
CVector3D	VEPSILON(EPSILON);
double		PI = 3.14159265;
double 		PI_2 = PI/2;

CTimer timer;

int randSeed = 0;
int r_s = 0;
double u(0), v(0);

//funkce pro generovani nahodnych cisel
inline double AKrandom(double min, double max){
	randSeed = int(timer.RealTime()*100000);
	//printf("randseed=%i\n", randSeed);
	srand(randSeed*r_s);
	//printf("rand = %i\n", rand());
	double ran = double(rand());
	//printf("ran=%f\n", ran);
	double r = ran/RAND_MAX * (max-min) + min;
	r_s = randSeed;
	return r;
};

//funkce pro generovani nahodnych cisel
double random(int min, int max) {
	return rand()%(max-min-2)-(max-1) + rand()%100*0.01;
} 

#endif //__PHYSICS
