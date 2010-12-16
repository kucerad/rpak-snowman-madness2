#ifndef __PARTICLEGEN
#define __PARTICLEGEN
#include "Vector.h"
#include "particle.h"
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
	virtual Particle* generate(void)=0;
};

class SnowflagGenerator: public ParticleGenerator{
public:
	SnowflagGenerator(vec _position, vec _direction):
		ParticleGenerator(_position,_direction){};
	
	virtual Particle* generate(void){
		// gen speed
		double a(AKrandom(-1,1)), b(AKrandom(-1,1)), c(AKrandom(-1,1));
		
		//printf("%f, %f, %f\n", a,b,c); //moc nahodne teda nejsou :(
		
		vec dir(a, b, c);
		
		double speed = AKrandom(1,2);
		
		//SnowParticle p(position, Normalize(dir)*speed, vec(1,0,0), 0, vec(0,1,0));
		
		return new SnowParticle(position, Normalize(dir)*speed, vec(1,0,0), 0, vec(0,1,0));
	}

};
class SnowpuffGenerator: public ParticleGenerator{
public:
	SnowpuffGenerator(vec _position, vec _direction):
		ParticleGenerator(_position,_direction){};
	
	virtual Particle* generate(void){
		// gen speed
		double a(AKrandom(-2,2)), c(AKrandom(-2,2)), x(AKrandom(-1,1)), y(AKrandom(-1,1)), z(AKrandom(-1,1)), angle(AKrandom(0,360));
		
		//printf("%f, %f, %f\n", a,b,c); //moc nahodne teda nejsou :(
		
		vec dir(a, 10, c);
		
		double speed = AKrandom(30,100);
		
		//SnowParticle p(position, Normalize(dir)*speed, vec(1,0,0), 0, vec(0,1,0));
		
		return new PuffParticle(position, dir*speed, vec(x,y,z), angle, vec(0,1,0), 1, 0.4);
	}

};
#endif
