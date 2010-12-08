#ifndef __PARTICLEWORLD
#define __PARTICLEWORLD
#include <vector>
#include "Vector.h"
#include "particle.h"
#include "ParticleGenerator.h"

typedef CVector3D vec;


class ParticleWorld{
public:
	ParticleWorld(ParticleGenerator* _generator ):
	  generator(_generator){
		
	}

	~ParticleWorld(){}

	//vypise parametry vsech particlu ve svete
	void printOut(void){
		printf("PWorld: [\n");
		for (unsigned int i=0; i<particles.size(); i++){
			printf("\t");
			particles[i].printOut();
		}
		printf("]\n");
	}

	//prida j novych castic na stejne pozici
	void add(int j){
		//vec puvodni = generator->position;
		for (int i=0; i<j; i++){
			//generator->position = puvodni + vec(rand(), rand(), rand());
			//printf("P[%f, %f, %f] \n", generator->position[X], generator->position[Y], generator->position[Z]);
			particles.push_back(generator->generate());
		}
		//generator->position = puvodni;
	}

	//prida j nahodnych castic do sceny
	void addRandom(int j) {
		for (int i=0; i<j; i++){
			generator->position = vec(random(-WORLD_SIZE_2,WORLD_SIZE_2), AKrandom(3,WORLD_HEIGHT/2), AKrandom(-WORLD_SIZE_2,WORLD_SIZE_2));
			particles.push_back(generator->generate());
		}
	}

	//vykresli vsechny castice
	void draw(void){
		for (int i=particles.size()-1; i>0; i--){
			particles[i].draw();
		}
	}

	//update vsech castic
	void update(double time){
		for (unsigned int i=0; i<particles.size(); i++){
			if (particles[i].jeVenku()){ 
				//odstraneni castic, ktere opustily scenu
				particles.erase(particles.begin()+i, particles.begin()+i+1);
				i--;
				continue;
			} 
			particles[i].update(time);
		}
	}

	vec position;
	ParticleGenerator * generator;
	std::vector<Particle> particles;
};

#endif
