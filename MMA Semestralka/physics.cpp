#include "physics.h"

extern double RYCHLOST_KOULE = 0.7; //rychlost, jakou ma na pocatku vrzena koule

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

//parametry kouli
float velikost_koule = 0.5;
int barva_koule = 16;
double velikost_zabiteho = 1;

CTimer timer;

int randSeed = 0;
int r_s = 0;
double u(0), v(0);