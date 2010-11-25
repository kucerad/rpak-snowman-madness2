#ifndef __PARTICLEWORLD
#define __PARTICLEWORLD
#include <vector>
#include "Vector.h"
#include "Particle.h"
#include "ParticleGenerator.h"

typedef CVector3D vec;


class ParticleWorld{
public:
	ParticleWorld(int _count, ParticleGenerator* _generator ):
	  count(_count),
	  generator(_generator){
		
	}
	~ParticleWorld(){}

	void printOut(void){

		printf("PWorld: [\n");

		for (int i=0; i<count; i++){
			printf("\t");
			particles[i].printOut();
		}
		printf("]\n");

	}
	void init(void){
		// erase first
		particles.erase(particles.begin(), particles.end());
		// fill
		for (int i=0; i<count; i++){
			particles.push_back(generator->generate());
		}
	}
	void draw(void){
		for (int i=0; i<particles.size(); i++){
			particles[i].draw();
		}
	}
	void update(void){
		for (int i=0; i<particles.size(); i++){
			if (particles[i].life<=0 || particles[i].position[Y]<0){
				// delete particle
				particles.erase(particles.begin()+i, particles.begin()+i+1);
				i--;
				continue;
			} 
			particles[i].update();
		}
	}
	int count;
	vec position;
	ParticleGenerator * generator;
	std::vector<Particle> particles;
};

#endif