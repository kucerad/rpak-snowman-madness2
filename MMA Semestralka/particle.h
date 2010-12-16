#ifndef __PARTICLE
#define __PARTICLE 
#include "Vector.h"
#include "physics.h"

typedef CVector3D vec;

class Particle{
public:
	/* konstruktor */
	Particle(vec _pos, vec _speed, vec _axis, double _angle, vec _normal, double _mass=1, double _size=1, double _lifetime=100):
		position(_pos),
		speed(_speed),
		axis(_axis),
		mass(_mass),
		size(_size),
		lifetime(_lifetime)
	{
		transf.LoadIdentityMatrix();
		transf.Rotate(_axis[X], _axis[Y], _axis[Z], _angle);
		normal = transf * Normalize(_normal);
		timeElapsed = 0;
	}

	/* destruktor */
	~Particle(){};

	vec			position;
	vec			speed;
	vec			axis;
	vec			normal;
	double		mass;
	double		size;
	double		lifetime;
	CMatrix		transf;
	double		timeElapsed;

	//vypis parametru castice
	void printOut(void){
		printf("pos[%f, %f, %f] speed[%f, %f, %f]\n", position[X], position[Y], position[Z], speed[X], speed[Y], speed[Z]);
	}
	// hlidani na opusteni sceny
	bool outTest(void) {
		if (position[X] < WORLD_SIZE_2 && position[X] > -WORLD_SIZE_2 && position[Z] < WORLD_SIZE_2 && position[Z] > -WORLD_SIZE_2 && position[Y] > 0) {
			return false;
		}
		else {
			return true;	
		}
	}
	// je castice ziva?
	bool isAlive(){
		if (timeElapsed>lifetime){
			return false;
		}
		return true;
	}
	virtual void update(double dt)=0{
		
	};
	virtual void draw()=0{
		
	};

};

class SnowParticle:  public Particle {
  public:
	SnowParticle(vec _position, vec _speed, vec _axis, double _angle, vec _normal, double _mass=5, double _size=0.5f, double _lifetime=5000):
	  Particle(_position, _speed, _axis, _angle, _normal, _mass, _size, _lifetime)
	  {}
	

	~SnowParticle(){};

	CMatrix		diffTransf;
	vec			Fn;
	vec			Ft;
	vec			S;
	vec			Nspeed;
	vec			Tspeed;

	//vypocet parametru castice v dalsim kroku
	virtual void update(double dt){
		dt*=0.1;
		diffTransf.LoadIdentityMatrix();
		timeElapsed += dt;
		/*
		if (life<timeElapsed){
			return;
		}
		*/
		// celkova rychlost v prostredi
		S = vec( -VITR + speed );
		
		// zavislost na natoceni k normale
		double	k = DotProduct(S, normal);
		// rychlost ve smeru normaly
		Nspeed = k * normal;
		// rychlost v rovine vlocky
		Tspeed = S - Nspeed;
		double	r = Length(Nspeed);
		// odpor vzduchu
		Fn = Nspeed * (-0.5 * r*r*size*size*KOEF_ODPORU*HUSTOTA_PROSTREDI) ;
		// treni o vzduch
		Ft = Tspeed * (- KOEF_TRENI )* HUSTOTA_PROSTREDI ;

		// zrychleni
		speed += (GRAVITACE + Fn + Ft)/mass;
		// posun
		
		position += speed*(dt);

		// natocit a update normaly
		double speedScale = Length(speed);
			
		// rotace...		
			
		double z = sin(PI_2+timeElapsed*50)*5;
		double angleDiff =  (z+Length(Ft))/mass;			
		
		diffTransf.Rotate(normal[X], normal[Y], normal[Z], 0.1*speedScale);
		diffTransf.Rotate(axis[X], axis[Y], axis[Z], angleDiff);
		
		normal = Normalize(diffTransf * normal);
		axis = Normalize(diffTransf * axis);
		transf = diffTransf*transf;
		
	}

	//vykresli castici
	virtual void draw(void){
		/*
		if (lifetime<timeElapsed){
			return;
		}
		*/
		double s2(size/2);
		double s3(size/4);

		glPushMatrix();
		
			//glEnable(GL_LIGHTING);				
			glTranslated(position[X],position[Y],position[Z]);
//#if DEBUG>0
	if (debug){
			glPushAttrib(GL_LINE_BIT);
			glLineWidth(0.1f);
				// rotation axis WHITE
				double sc = 0.1;
				glBegin(GL_LINES);
					glColor3f(1.f, 1.f, 1.f);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(Nspeed[X]*sc, Nspeed[Y]*sc, Nspeed[Z]*sc);
				glEnd();
				
				// speed in wind Force LILA
				
				glBegin(GL_LINES);
					glColor3f(1.f, 0.0f, 1.f);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(Tspeed[X]*sc, Tspeed[Y]*sc, Tspeed[Z]*sc);
				glEnd();
				// Normal Force GREEN
				
				glBegin(GL_LINES);
					glColor3f(0.f, 1.0f, 0.f);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(Fn[X], Fn[Y], Fn[Z]);
				glEnd();
				
				// Side Force YELLOW
				
				glBegin(GL_LINES);
					glColor3f(1.0f, 1.0f, 0.f);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(Ft[X], Ft[Y], Ft[Z]);
				glEnd();
				
				// Normal RED
				glBegin(GL_LINES);
					glColor3f(1.0f, 0.f, 0.f);
					glVertex3d(0.0, 0.0, 0.0);
					glVertex3d(normal[X], normal[Y], normal[Z]);
				glEnd();
			glPopAttrib();
	}
//#endif				
				//glRotated(angle, axis[X], axis[Y], axis[Z]);
				
				glMultMatrixd(transf._matrix);
				
				glColor3f(1.f,1.f, 1.f);		
				//glEnable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				// povoleni michani barev
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, textureIDs[10]);
				glBegin(GL_QUADS);
				  glTexCoord2f(0.0, 0.0);
				  glVertex3d(-s2, 0.0, -s2);
  				  
				  glTexCoord2f(1.0, 0.0);
				  glVertex3d(s2, 0.0, -s2);
				  
				  glTexCoord2f(1.0, 1.0);
				  glVertex3d(s2, 0.0, s2);
				  
				  glTexCoord2f(0.0, 1.0);
				  glVertex3d(-s2, 0.0, s2);
				glEnd();

				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
			
			//glDisable(GL_LIGHTING);
	
		glPopMatrix();
	}

};


/* PUFF particle
*/
class PuffParticle:  public Particle {
  public:
	PuffParticle(vec _position, vec _speed, vec _axis, double _angle, vec _normal, double _mass=5, double _size=0.5f, double _lifetime=5000):
	  Particle(_position, _speed, _axis, _angle, _normal, _mass, _size, _lifetime)
	  {}
	

	~PuffParticle(){};

	CMatrix		diffTransf;
	vec			S;


	//vypocet parametru castice v dalsim kroku
	virtual void update(double dt){
		dt*=0.1;
		diffTransf.LoadIdentityMatrix();
		timeElapsed += dt;
		/*
		if (life<timeElapsed){
			return;
		}
		*/
		speed += (GRAVITACE)/mass;
		position+=speed*dt*0.05;
		/*		
		
		diffTransf.Rotate(normal[X], normal[Y], normal[Z], 0.1*speedScale);
		diffTransf.Rotate(axis[X], axis[Y], axis[Z], angleDiff);
		
		normal = Normalize(diffTransf * normal);
		axis = Normalize(diffTransf * axis);
		transf = diffTransf*transf;
		*/
	}

	//vykresli castici
	virtual void draw(void){
		/*
		if (lifetime<timeElapsed){
			return;
		}
		*/
		double s2(size/2);
		//double s3(size/4);

		glPushMatrix();
		
			//glEnable(GL_LIGHTING);				
			glTranslated(position[X],position[Y],position[Z]);			
			//glRotated(angle, axis[X], axis[Y], axis[Z]);
				
				glMultMatrixd(transf._matrix);
				
				glColor3f(0.5f,0.5f, 1.f);		
				//glEnable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
				// povoleni michani barev
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, textureIDs[10]);
				glBegin(GL_QUADS);
				  glTexCoord2f(0.0, 0.0);
				  glVertex3d(-s2, 0.0, -s2);
  				  
				  glTexCoord2f(1.0, 0.0);
				  glVertex3d(s2, 0.0, -s2);
				  
				  glTexCoord2f(1.0, 1.0);
				  glVertex3d(s2, 0.0, s2);
				  
				  glTexCoord2f(0.0, 1.0);
				  glVertex3d(-s2, 0.0, s2);
				glEnd();

				glDisable(GL_BLEND);
				glDisable(GL_TEXTURE_2D);
			
			//glDisable(GL_LIGHTING);
	
		glPopMatrix();
	}

};

#endif __PARTICLE
