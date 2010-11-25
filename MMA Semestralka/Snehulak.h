#ifndef __SNEHULAK
#define __SNEHULAK 

#include "TransformNode.h"
#include "Vector.h"

class Snehulak {
public:
	Snehulak(double posx, double posy, double posz, double r1, double v1, double r2, double v2, double r3, double v3) {
		pozicex = posx;
		pozicey = posy;
		pozicez = posz;
		polomer1 = r1;
		polomer2 = r2;
		polomer3 = r3;
		vyska1 = v1;
		vyska2 = v2;
		vyska3 = v3;
		smer = 0;
	}
	
	~Snehulak(void) {
	}
	
	void vykreslit(void) {
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[9]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[9]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular[9]);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess[9]);
		glPushMatrix();
		glTranslated(0,vyska1,0);
		glutSolidSphere(polomer1, 25, 25);
		glTranslated(0,vyska2-vyska1,0);
		glutSolidSphere(polomer2, 25, 25);
		glTranslated(0,vyska3-vyska2,0);
		glutSolidSphere(polomer3, 25, 25);
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[15]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[15]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular[15]);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess[15]);
		glTranslated(0,polomer3,0);
		glutSolidCube(polomer3*1.4);  
		glTranslated(-polomer3*0.7,0,0);
		glutSolidTorus(polomer3*0.15, polomer3*0.18, 10, 15);  
		glTranslated(polomer3*1.4,0,0);
		glutSolidTorus(polomer3*0.15, polomer3*0.18, 10, 15); 

		glDisable(GL_LIGHTING);
		glPopMatrix();
	}

	void posun() {
		double u = random(-5,5);
		double v = random(0,10)*0.01;
		smer += u*0.05;
		double x = pozicex+cos(smer)*v;
		double z = pozicez+sin(smer)*v;
		CVector3D poz(x,0,z);
		double l = Length(poz);

		if ((x>(WORLD_SIZE_2-1) || x<-(WORLD_SIZE_2-1)) || (l>-4 && l<4) || (z>(WORLD_SIZE_2-1) || z<-(WORLD_SIZE_2-1)))   {
			smer +=180;
		} 
		pozicex += cos(smer)*v;
		pozicez += sin(smer)*v;
	}

	bool koliduje(CVector3D pos, double polomerKoule) {
		if (sqrt( (pozicex-pos[0])*(pozicex-pos[0])+(pozicey+vyska1-pos[1])*(pozicey+vyska1-pos[1])+(pozicez-pos[2])*(pozicez-pos[2])) < polomer1+polomerKoule   ) {
			return true;
		}
		if (sqrt( (pozicex-pos[0])*(pozicex-pos[0])+(pozicey+vyska2-pos[1])*(pozicey+vyska2-pos[1])+(pozicez-pos[2])*(pozicez-pos[2])) < polomer2+polomerKoule   ) {
			return true;
		}
		if (sqrt( (pozicex-pos[0])*(pozicex-pos[0])+(pozicey+vyska3-pos[1])*(pozicey+vyska3-pos[1])+(pozicez-pos[2])*(pozicez-pos[2])) < polomer3+polomerKoule   ) {
			return true;
		}
		return false;
	}


	double pozicex, pozicey, pozicez, polomer1, vyska1, polomer2, vyska2, polomer3, vyska3;
	double smer;
	CTransformNode * rodic;

};

#endif __SNEHULAK