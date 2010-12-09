#ifndef __PHYSICS_H
#define __PHYSICS_H

#include "Vector.h"
#include "timer.h"

//parametry sveta
#define WORLD_SIZE_2  30.0
#define WORLD_HEIGHT  30.0
#define ZEME 0.0 // vyska zeme

#define DEG_TO_RAD (3.14159/180.0) // konstanta pro prevod stupnu na radiany

extern double RYCHLOST_KOULE; //rychlost, jakou ma na pocatku vrzena koule

// nastaveni fyziky
extern CVector3D	GRAVITACE;
extern CVector3D	VITR;
extern double		HUSTOTA_PROSTREDI;
extern double		KOEF_ODPORU;
extern double		KOEF_TRENI;
extern double		dt;
extern double		EPSILON;
extern CVector3D	VEPSILON;
extern double		PI;
extern double 		PI_2;

//parametry kouli
extern float velikost_koule;
extern int barva_koule;
extern double velikost_zabiteho;

extern CTimer timer;

extern int randSeed;
extern int r_s;
extern double u, v;

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
inline double random(int min, int max) {
	return rand()%(max-min-2)-(max-1) + rand()%100*0.01;
} 

#endif //__PHYSICS
