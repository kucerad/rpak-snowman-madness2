#ifndef __PARTICLEGEN
#define __PARTICLEGEN
#include "Vector.h"
#include "Particle.h"
#include "physics.h"

typedef CVector3D vec;

/* 
Simple and base particle generator

other generators must inherit from this one and overload generate method!!!
*/

class ParticleGenerator{
public:
	ParticleGenerator( vec _position, vec _direction ): 
	  position(_position),
	  direction(_direction){
		
	}
	~ParticleGenerator(){}
	
	vec position;
	vec direction;
	virtual Particle generate(void){
		// gen speed
		double a(AKrandom(-1,1)), b(AKrandom(-1,1)), c(AKrandom(-1,1));
		
		//printf("%f, %f, %f\n", a,b,c); //moc nahodne teda nejsou :(
		
		vec dir(a, b, c);
		
		double speed = AKrandom(1,2);
		Particle p(position, Normalize(dir)*speed, vec(1,0,0), 0, vec(0,1,0), 0);
		
		return p;
		
	}
};
#endif