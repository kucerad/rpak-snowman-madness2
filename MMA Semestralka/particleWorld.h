#ifndef __PARTICLEWORLD
#define __PARTICLEWORLD
#include <vector>
#include "Vector.h"
#include "Particle.h"
#include "ParticleGenerator.h"

typedef CVector3D vec;


class ParticleWorld{
public:
	ParticleWorld(ParticleGenerator* _generator ):
	  generator(_generator){
		
	}
	~ParticleWorld(){}

	void printOut(void){

		printf("PWorld: [\n");

		for (int i=0; i<particles.size(); i++){
			printf("\t");
			particles[i].printOut();
		}
		printf("]\n");

	}
	void init(void){
		// erase first
		particles.erase(particles.begin(), particles.end());
		// fill
		for (int i=0; i<1; i++){
			particles.push_back(generator->generate());
		}
	}

	void add(int j){
		// fill
		for (int i=0; i<j; i++){
			particles.push_back(generator->generate());
		}
	}

	void addRandom(int j) {
		// fill
		for (int i=0; i<j; i++){
			generator->position = vec(random(-WORLD_SIZE_2,WORLD_SIZE_2), AKrandom(3,WORLD_HEIGHT/2), AKrandom(-WORLD_SIZE_2,WORLD_SIZE_2));
			particles.push_back(generator->generate());
		}
	}

	void draw(void){
		for (int i=particles.size()-1; i>0; i--){
			particles[i].draw();
		}
	}
	void update(double time){
		for (int i=0; i<particles.size(); i++){
			if (particles[i].jeVenku()){ //particles[i].life<=0 || 
				// delete particle
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