#ifndef __KOULE
#define __KOULE

#include "Vector.h"

class Koule {
public:
	Koule(CVector3D _pos, CVector3D _smer, double _rychlost, double _polomer) {
		pozice = _pos;
		smerPohybu = _smer;
		polomer = _polomer;
		rychlost = Normalize(smerPohybu);
		rychlost*= _rychlost*1000;
		obsah = 3.1415*polomer*polomer/100;
		hmotnost = 0.5;// 4/3 * 3.1415 * polomer*polomer*polomer * 0.001;
	}

	void vykreslit(void) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[16]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[16]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular[16]);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess[16]);

		glPushMatrix();
		glEnable(GL_LIGHTING);
		glTranslated(pozice[0],pozice[1],pozice[2]);
		glutSolidSphere(polomer, 25, 25);
		glDisable(GL_LIGHTING);
		glPopMatrix();
	}

	void vykreslit2() {

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[19]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[19]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular[19]);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess[19]);

		glPushMatrix();
		glEnable(GL_LIGHTING);
		glTranslated(pozice[0],pozice[1],pozice[2]);
		//glutSolidSphere(polomer, 25, 25);
		glDisable(GL_LIGHTING);
		glPopMatrix();

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[16]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[16]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular[16]);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess[16]);

		glPushMatrix();
		glEnable(GL_LIGHTING);
		glTranslated(pozice[0],0,pozice[2]);
		glutSolidSphere(polomer*0.7, 5, 5);
		glTranslated((pozice[1]-2)/4,0,0);
		glutSolidSphere(polomer*0.4, 5, 5);
		glTranslated(-(pozice[1]-2)/3,0,(pozice[1]-3)/3);
		glutSolidSphere(polomer*0.5, 5, 5);
		glDisable(GL_LIGHTING);

		glPopMatrix();
	}

	void update(void) {
		// SETRVACNOST
		// F = m.a
		double r = Length(rychlost);
		//printf("r %f\n",r);
		CVector3D odpor = Normalize(rychlost) * (-0.5* r*obsah*KOEF_ODPORU*HUSTOTA_PROSTREDI);
		//rychlost += odpor;
		rychlost += (-odpor+GRAVITACE)/hmotnost;
		pozice+=rychlost*dt;
		//printf("odpor ");
		//odpor.printOut();
		//printf("pozice ");
		//pozice.printOut();

	} 

	bool jeVenku(void) {
		if (pozice[0] < WORLD_SIZE_2 && pozice[0] > -WORLD_SIZE_2 && pozice[2] < WORLD_SIZE_2 && pozice[2] > -WORLD_SIZE_2 && pozice[1] < WORLD_HEIGHT && pozice[1] > 0) {
			return false;
		}
		else {
			return true;	
		}
	}

	bool koliduje(void) {
		// move snowman to some new location
		for (int i=0; i<snehulaci.size(); i++) {
			if (snehulaci[i].koliduje(pozice, polomer)) {
				// create particle world...
				pGen.position = pozice;
				pWorld.add(5);

				double v = random(5,15);
				double u = random(0,360);
				snehulaci[i].pozice[X] = cos(u)*v;
				snehulaci[i].pozice[Z] = sin(u)*v;
				return true;
			}
		}
		return false;
	}
	CVector3D rychlost;
	CVector3D pozice;
	CVector3D smerPohybu;
	double hmotnost;
	double obsah;	
	double polomer;
};


#endif __KOULE