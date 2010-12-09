#include "Snehulak.h"

Snehulak::Snehulak(CVector3D _pozice, CVector3D _polomer, CVector3D _vyska) {
	pozice = _pozice;
	polomer = _polomer;
	vyska = _vyska;
	smer = 0;
}
	
Snehulak::~Snehulak(void) {
}
	
//jednoduche vykresleni snehulaka
void Snehulak::vykreslit(void) {
	glEnable(GL_LIGHTING);
	glPushMatrix();

	//koule
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[9]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[9]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular[9]);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess[9]);
	
	glTranslated(0,vyska[0],0);
	glutSolidSphere(polomer[0], 25, 25);
	glTranslated(0,vyska[1]-vyska[0],0);
	glutSolidSphere(polomer[1], 25, 25);
	glTranslated(0,vyska[2]-vyska[1],0);
	glutSolidSphere(polomer[2], 25, 25);
	
	//hrnec na hlave
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[15]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[15]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular[15]);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess[15]);

	glTranslated(0,polomer[2],0);
	glutSolidCube(polomer[2]*1.4);  
	glTranslated(-polomer[2]*0.7,0,0);
	glutSolidTorus(polomer[2]*0.15, polomer[2]*0.18, 10, 15);  
	glTranslated(polomer[2]*1.4,0,0);
	glutSolidTorus(polomer[2]*0.15, polomer[2]*0.18, 10, 15); 

	glPopMatrix();
	glDisable(GL_LIGHTING);
}

//posune snehulaka v danem smeru  - chuze snehulaka
void Snehulak::posun(void) {
	double u = random(-5,5);
	double v = random(0,10)*0.01;
	smer += u*0.05;
	double x = pozice[X]+cos(smer)*v;
	double z = pozice[Z]+sin(smer)*v;
	CVector3D poz(x,0,z);
	double l = Length(poz);

	if ((x>(WORLD_SIZE_2-1) || x<-(WORLD_SIZE_2-1)) || (l>-4 && l<4) || (z>(WORLD_SIZE_2-1) || z<-(WORLD_SIZE_2-1)))   {
		smer +=180;
	} 

	pozice[X] += cos(smer)*v;
	pozice[Z] += sin(smer)*v;
}

//zjisti, zda snehulak koliduje s vrzenou kouli
bool Snehulak::koliduje(CVector3D pos, double polomerKoule) {
	if (sqrt( (pozice[X]-pos[0])*(pozice[X]-pos[0])+(pozice[Y]+vyska[0]-pos[1])*(pozice[Y]+vyska[0]-pos[1])+(pozice[Z]-pos[2])*(pozice[Z]-pos[2])) < polomer[0]+polomerKoule   ) {
		return true;
	}
	if (sqrt( (pozice[X]-pos[0])*(pozice[X]-pos[0])+(pozice[Y]+vyska[1]-pos[1])*(pozice[Y]+vyska[1]-pos[1])+(pozice[Z]-pos[2])*(pozice[Z]-pos[2])) < polomer[1]+polomerKoule   ) {
		return true;
	}
	if (sqrt( (pozice[X]-pos[0])*(pozice[X]-pos[0])+(pozice[Y]+vyska[2]-pos[1])*(pozice[Y]+vyska[2]-pos[1])+(pozice[Z]-pos[2])*(pozice[Z]-pos[2])) < polomer[2]+polomerKoule   ) {
		return true;
	}
	return false;
}