#ifndef __PARTICLE
#define __PARTICLE 
#include "Vector.h"
#include "physics.h"


// show normals and forces... DEBUG = 1
#define DEBUG 0

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
	double		timeElapsed;

	void printOut(void){
		printf("P[%f, %f, %f] speed[%f, %f, %f]\n", position[X], position[Y], position[Z], speed[X], speed[Y], speed[Z]);
	}
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
		double	k = DotProduct(Normalize(S), normal);
		// rychlost ve smeru normaly
		vec		Nspeed( k * normal );
		// rychlost v rovine vlocky
		vec		Tspeed( S - Nspeed );
		double	r = Length(Nspeed);
		// odpor vzduchu
		Fn = Nspeed * (-0.5 * r*r*size*size*0.01*KOEF_ODPORU*HUSTOTA_PROSTREDI) ;
		// treni o vzduch
		Ft = Tspeed * (- KOEF_TRENI )* HUSTOTA_PROSTREDI ;

		// zrychleni
		speed += (GRAVITACE + Fn + Ft)/mass;
		// posun
		//printf("tDiff:  %f\n",tDiff);
		position += speed*(tDiff);

		// natocit a update normaly
		double speedScale = Length(speed);
			
		// rotace...
// TODO: harmonicka zmena rychlosti a moznost zmeny orientace; 
			axis = Normalize(axis+CrossProduct(normal, Normalize(Ft)));

			double z = sin(timeElapsed);
			//var r:Number = Math.random()*2-1;
			//omega +=  ( r * 0.05 + z * 0.5) / mass; 
			double angleDiff =  (speedScale*0.5 + z * 0.1) / mass;	
			//double angleDiff =  z ;// /mass;	
			
			diffTransf.Rotate(axis[X], axis[Y], axis[Z], angleDiff);
			
			normal = Normalize(diffTransf * normal);
			axis = Normalize(diffTransf * axis);
			transf = transf*diffTransf;
		//}
		
	}
	void draw(void){
		if (life<0){
			return;
		}
		double s2(size/2);

		//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[4]);
		//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[4]);
		//glMaterialfv(GL_FRONT, GL_SPECULAR, specular[4]);
		//glMaterialf(GL_FRONT, GL_SHININESS, shininess[4]);
		//
		glPushMatrix();
			//glEnable(GL_LIGHTING);				
				glTranslated(position[X],position[Y],position[Z]);
#if DEBUG>0
				// speed in wind Force LILA
				glBegin(GL_LINES);
					glColor3f(1.f, 0.0f, 1.f);
					glVertex3d(0.0, 0.0, 0.0);
					double sc = 0.1;
					glVertex3d(S[X]*sc, S[Y]*sc, S[Z]*sc);
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
#endif				
				//glRotated(angle, axis[X], axis[Y], axis[Z]);
				glMultMatrixd(transf._matrix);
				
				glColor3f(1.f,1.f, 1.f);
				
				
				glEnable(GL_TEXTURE_2D);
				// povoleni michani barev
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
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
				// draw normal...
				

			
			//glDisable(GL_LIGHTING);
		glPopMatrix();
	}

};


#endif __PARTICLE