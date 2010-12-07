#ifndef __PARTICLE
#define __PARTICLE 
#include "Vector.h"
#include "physics.h"


// show normals and forces... DEBUG = 1
#define DEBUG 1

typedef CVector3D vec;


class Particle{
public:
	Particle(vec _position, vec _speed, vec _axis, double _angle, vec _normal, double _aspeed, double _lTime, double _mass=5, double _size=0.5, double _life=5000, double _rot=0):
	  position(_position),
	  speed(_speed),
	  axis(_axis),
	  angle(_angle),
	  normal(Normalize(_normal)),
	  aspeed(_aspeed),
	  mass(_mass),
	  size(_size),
	  life(_life),
	  rotDiff(_rot),
	  lTime(_lTime){

	  transf.LoadIdentityMatrix();
	  transf.Rotate(_axis[X], _axis[Y], _axis[Z], _angle);
	  normal = transf * Normalize(_normal);
	  timeElapsed = 0;
	}

	~Particle(){};

	vec			position;
	vec			speed;
	vec			axis;
	vec			normal;
	double		aspeed;
	double		mass;
	double		size;
	double		life;
	double		angle;
	double		rotDiff;
	double		lTime;
	CMatrix		diffTransf;
	CMatrix		transf;
	vec			Fn;
	vec			Ft;
	vec			S;
	vec			Nspeed;
	vec			Tspeed;
	double		timeElapsed;

	//vypis parametru castice
	void printOut(void){
		printf("P[%f, %f, %f] speed[%f, %f, %f]\n", position[X], position[Y], position[Z], speed[X], speed[Y], speed[Z]);
	}

	//kontrola, zda castice opustila scenu
	bool jeVenku(void) {
		if (position[0] < WORLD_SIZE_2 && position[0] > -WORLD_SIZE_2 && position[2] < WORLD_SIZE_2 && position[2] > -WORLD_SIZE_2 && position[1] > 0) {
			return false;
		}
		else {
			return true;	
		}
	}

	//vypocet parametru castice v dalsim kroku
	void update(double time){
		diffTransf.LoadIdentityMatrix();
		double tDiff = dt;
		lTime = time;
		timeElapsed += tDiff;
		if (life<0){
			return;
		}
		life--;

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
		
		position += speed*(tDiff);

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
	void draw(void){
		if (life<0){
			return;
		}
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
					glVertex3d(Fn[X]*10, Fn[Y]*10, Fn[Z]*10);
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
				glBindTexture(GL_TEXTURE_2D, textureIDs[9]);
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