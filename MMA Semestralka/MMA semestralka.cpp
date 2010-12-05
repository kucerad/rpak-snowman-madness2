#include "physics.h"
#include <math.h>
#include "common.h"
#include "SceneNode.h"
#include "GeometryNode.h"
#include "TransformNode.h"
#include "materials.inc"
#include "image.h"
#include "TextureLoader.h"

//id textur
GLuint textureIDs[10];

#include "ParticleWorld.h"
#include "ParticleGenerator.h"
#include <time.h>
ParticleGenerator	pGen(vec(0,5,2), vec(0,1,0));
ParticleWorld		pWorld(&pGen);

#include "Snehulak.h"
std::vector<Snehulak> snehulaci;

#include "Koule.h"
std::vector<Koule> snehoveKoule;
std::vector<Koule> kolize;

#define WIDTH 800 // sirka okna
#define HEIGHT 680 // vyska okna

#define SMALL_ANGLE2 1.0 //konstanta maleho uhlu
double delka_kroku; //delka kroku
GLdouble smerPohybu[3]; //pole pro ulozeni smeru pohybu
GLdouble uhelPohledu=180.0; //pocatecni uhel pohledu
GLdouble beta = 0.0; //uhel, pod kterym avatar pozoruje scenu
GLdouble sunAngle = 90; //pocatecni natoceni slunce

double sunAngleSpeed; //uhel udavajici rychlost obehu slunce
bool posvit = false; //kdyz true, tak bude svitit baterka
bool slunceSviti = true; //kdyz true, tak bude svitit slunce
bool zamlzeno = false; //kdyz true, bude husta mlha
bool snezi = true;
bool jedna = true;
bool dva = true;
bool dopredu = false;
bool dozadu = false;
bool doleva = false;
bool doprava = false;
bool nahoru = false;
bool dolu = false;

double RYCHLOST_KOULE = 0.7;

float vyska_postavy; //konstanta vysky avatara
GLdouble pozice[3]={0.0, vyska_postavy, 0.0}; //pozice avatara pri startu aplikace
GLdouble smerPohledu[3];
bool walkmode=true; // kdyz true, bude mozne prochazet scenou

GLdouble xm[3]; //pole pro ulozeni poloh kurzoru
GLdouble ym[3]; //pole pro ulozeni poloh kurzoru

//Definice jmen uzitych pri vytvareni grafu sceny
#define STROM_FILE_NAME "data/strom.obj"
#define BENCH_FILE_NAME "data/Bench.obj"

CSceneNode* rootNode_p = NULL; // koren grafu sceny

//Parametry zeme
GLfloat zemeDiffuseColor[] = {0.1, 0.1, 0.1, 1}; // barva
GLfloat zemePlaneSize = 100.0f; // velikost jednoho dilku
GLint zemePlaneSubDiv = 30; // rozdeleni

//parametry pro obrazovku
#define PANEL_SIZE_X 2
#define PANEL_SIZE_Y 2
static float whichFrame = 6;

//nacita konfiguracni soubor
void nactiSoubor(){
	FILE * fr;
	if ((fr = fopen("data/config.ini", "r")) == NULL) {
		sunAngleSpeed = 1;
		delka_kroku = 0.2;
		vyska_postavy = 1.7;
	} else {
		float x;
		if (fscanf(fr, "%f\n", &x) == NULL) sunAngleSpeed = 1;
		else sunAngleSpeed = x;
		if (fscanf(fr, "%f\n", &x) == NULL) delka_kroku = 0.2; 
		else delka_kroku = x;
		if (fscanf(fr, "%f\n", &x) == NULL) vyska_postavy = 1.7; 
		else vyska_postavy = x;
	}
}

//nastaveni textury
void setTexture(GLuint ID, CImage* image) {
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, image->GetComponents(), image->GetWidth(), image->GetHeight(), 1, image->GetFormat(), GL_UNSIGNED_BYTE, image->GetPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//inicializace použitých textur
void initTextures() {
 CTextureLoader *textureLoader = new CTextureLoader();
 CImage *image = new CImage();

	glGenTextures(10, textureIDs);

	if (textureLoader->Load("data/iceflow_front.tga", image) != false) {
		setTexture(textureIDs[0], image);
	}

	if (textureLoader->Load("data/iceflow_back.tga", image) != false) {
		setTexture(textureIDs[1], image);
	}

	if (textureLoader->Load("data/iceflow_left.tga", image) != false) {
		setTexture(textureIDs[2], image);
	}

	if (textureLoader->Load("data/iceflow_right.tga", image) != false) {
		setTexture(textureIDs[3], image);
	}

	if (textureLoader->Load("data/iceflow_top.tga", image) != false) {
		setTexture(textureIDs[4], image);
	}

	if (textureLoader->Load("data/zeme_texture2.tga", image) != false) {
		setTexture(textureIDs[5], image);
	}

	// obrazovky
	char filename[128];
	for(int i=1; i<4; i++) {
		sprintf(filename, "data/panel%i.tga", i);
		if (textureLoader->Load(filename, image) != false) {
			setTexture(textureIDs[5+i], image);
		}
	}

	if (textureLoader->Load("data/snowflake.tga", image) != false) {
		setTexture(textureIDs[9], image);
	}
  delete image;
  delete textureLoader;
}

//vytvoreni skyboxu
void skybox() {
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	// front
	//glColor3d(1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 0.0);glVertex3d( WORLD_SIZE_2, 0, WORLD_SIZE_2);
  	  glTexCoord2f(1.0, 0.0);glVertex3d(-WORLD_SIZE_2, 0, WORLD_SIZE_2);
	  glTexCoord2f(1.0, 1.0);glVertex3d(-WORLD_SIZE_2, WORLD_HEIGHT, WORLD_SIZE_2);
	  glTexCoord2f(0.0, 1.0);glVertex3d( WORLD_SIZE_2, WORLD_HEIGHT, WORLD_SIZE_2);
	glEnd();

	// back
	//glColor3d(0.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 0.0);glVertex3d(-WORLD_SIZE_2, 0, -WORLD_SIZE_2);
  	  glTexCoord2f(1.0, 0.0);glVertex3d( WORLD_SIZE_2, 0, -WORLD_SIZE_2);
	  glTexCoord2f(1.0, 1.0);glVertex3d( WORLD_SIZE_2, WORLD_HEIGHT, -WORLD_SIZE_2);
	  glTexCoord2f(0.0, 1.0);glVertex3d(-WORLD_SIZE_2, WORLD_HEIGHT, -WORLD_SIZE_2);
	glEnd();

	// left
	//glColor3d(0.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 0.0);glVertex3d(-WORLD_SIZE_2, 0,  WORLD_SIZE_2);
  	  glTexCoord2f(1.0, 0.0);glVertex3d(-WORLD_SIZE_2, 0, -WORLD_SIZE_2);
	  glTexCoord2f(1.0, 1.0);glVertex3d(-WORLD_SIZE_2, WORLD_HEIGHT, -WORLD_SIZE_2);
	  glTexCoord2f(0.0, 1.0);glVertex3d(-WORLD_SIZE_2, WORLD_HEIGHT, WORLD_SIZE_2);
	glEnd();

	// right
	//glColor3d(1.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 0.0);glVertex3d(WORLD_SIZE_2, 0, -WORLD_SIZE_2);
  	  glTexCoord2f(1.0, 0.0);glVertex3d(WORLD_SIZE_2, 0,  WORLD_SIZE_2);
	  glTexCoord2f(1.0, 1.0);glVertex3d(WORLD_SIZE_2, WORLD_HEIGHT,  WORLD_SIZE_2);
	  glTexCoord2f(0.0, 1.0);glVertex3d(WORLD_SIZE_2, WORLD_HEIGHT, -WORLD_SIZE_2);
	glEnd();

	// top
	//glColor3d(1.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[4]);
	glBegin(GL_QUADS);
  	  glTexCoord2f(1.0, 1.0);glVertex3d(WORLD_SIZE_2, WORLD_HEIGHT,  WORLD_SIZE_2);
	  glTexCoord2f(0.0, 1.0);glVertex3d(-WORLD_SIZE_2, WORLD_HEIGHT, WORLD_SIZE_2);
	  glTexCoord2f(0.0, 0.0);glVertex3d(-WORLD_SIZE_2, WORLD_HEIGHT, -WORLD_SIZE_2);
	  glTexCoord2f(1.0, 0.0);glVertex3d(WORLD_SIZE_2, WORLD_HEIGHT,  -WORLD_SIZE_2);
	glEnd();

	// bottom
	//glColor3d(0.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 0.0);glVertex3d(-WORLD_SIZE_2, 0.0, WORLD_SIZE_2);
  	  glTexCoord2f(1.0, 0.0);glVertex3d(WORLD_SIZE_2, 0.0,  WORLD_SIZE_2);
	  glTexCoord2f(1.0, 1.0);glVertex3d(WORLD_SIZE_2, 0.0,  -WORLD_SIZE_2);
	  glTexCoord2f(0.0, 1.0);glVertex3d(-WORLD_SIZE_2, 0.0, -WORLD_SIZE_2);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
}

//nakresli plochu z tringlestripu
void kresliPlochu(void) {
	glNormal3f(0.0, 1.0, 0.0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[3]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, zemeDiffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular[3]);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0);

	float size = zemePlaneSize; 
	float subDiv = zemePlaneSubDiv;

	float dz;
	dz = size/subDiv;

	GLfloat px, pz;
	pz = px = -size/2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
	for(int z=0; z<subDiv; z++) { 
		glBegin(GL_TRIANGLE_STRIP); 
		px = -size/2;
			for(int x=0; x<subDiv+1; x++) {
				if (jedna) {
					glTexCoord2d(0,1);
				} else {
					glTexCoord2d(1,1);
				} 
				glVertex3f(px, ZEME, pz); 
				if (dva) {
					glTexCoord2d(1,0);
				} else {
					glTexCoord2d(0,0);
				} 
				glVertex3f(px, ZEME, pz+dz); 
				jedna = !jedna;
				dva = !dva;
				px += dz;
			}
		glEnd();
		pz += dz;
	}
	glDisable(GL_TEXTURE_2D);
}

//kresli podstavu pod brontosaurem
void kresliPodstavu(void) {
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient[9]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[9]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular[9]);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess[9]);
	glutSolidCube(1.0);
	glDisable(GL_LIGHTING);
}

//kresli informacni panel
void infoPanel() {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textureIDs[(int)whichFrame]);
	glBegin(GL_QUADS);
	  glTexCoord2f(0.0, 0.0);glVertex3d(-PANEL_SIZE_X, 0.0, 0);
  	  glTexCoord2f(1.0, 0.0);glVertex3d(PANEL_SIZE_X, 0.0, 0);
	  glTexCoord2f(1.0, 1.0);glVertex3d(PANEL_SIZE_X, PANEL_SIZE_Y, 0);
	  glTexCoord2f(0.0, 1.0);glVertex3d(-PANEL_SIZE_X, PANEL_SIZE_Y, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();

}

//vykresli zemi
class CZemeNode : public CGeometryNode { //vykresli zemi
	void Update(void){
		kresliPlochu();
	}
};

//vykresli podstavec
class CPodstavecNode : public CGeometryNode { 
	void Update(void){
		kresliPodstavu();
	}
};

//vykresli skybox
class CSkyboxNode : public CGeometryNode { 
	void Update(void){
		skybox();
	}
};

class CPanelNode : public CGeometryNode { 
	void Update(void){
		infoPanel();
	}
};

class CSnehulakNode : public CGeometryNode { 
	void Update(void){
		sn->vykreslit();
	}
public:
	Snehulak * sn;
};

//nastaveni parametru baterky jako light1
void baterka(void) { 
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0f);
	GLfloat barvaBaterkyAmb[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_AMBIENT, barvaBaterkyAmb);
	glLightfv(GL_LIGHT1, GL_SPECULAR, barvaBaterkyAmb);
	GLfloat barvaBaterkyDif[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, barvaBaterkyDif);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);

	GLfloat poziceSvetla[] = {pozice[0], pozice[1], pozice[2], 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, poziceSvetla);

    GLfloat smerSvetla[] = {cos(uhelPohledu*DEG_TO_RAD), tan(beta*DEG_TO_RAD), sin(uhelPohledu*DEG_TO_RAD), 1.0f};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, smerSvetla);	
}


//zapne nebo vypne baterku - light1
void prepniBaterku(void) { 
	if (!posvit) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}
	posvit = !posvit;
}

//nastaveni parametru slunce
void slunce(void) { 
	GLfloat barvaSlunceAmb[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, barvaSlunceAmb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, barvaSlunceAmb);
	GLfloat barvaSlunceDif[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, barvaSlunceDif);
	GLfloat sunPos[] = {0.0, WORLD_HEIGHT*sin(sunAngle*DEG_TO_RAD), WORLD_SIZE_2*cos(sunAngle*DEG_TO_RAD), 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);
	//glPushMatrix();
	//glTranslated(0.0, 20*sin(sunAngle*DEG_TO_RAD), 20*cos(sunAngle*DEG_TO_RAD));
	//glutSolidSphere(1.0,10.0,10.0);
	//glPopMatrix();
}

//zapne nebo vypne slunce
void prepniSlunce(void) { 
	if (!slunceSviti) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	slunceSviti = !slunceSviti;
}

//nastaveni parametru mlhy
void mlha(void) { 
	GLfloat fogColor[4] = {0.7, 0.7, 0.7, 1.0};
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.1);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
}

//zapne nebo vypne zobrazeni mlhy
void prepniMlhu(void) { 
	if(!zamlzeno) {
		glEnable(GL_FOG);
		HUSTOTA_PROSTREDI +=1;
	}
	else {
		glDisable(GL_FOG);
		HUSTOTA_PROSTREDI -=1;
	}
	zamlzeno = !zamlzeno;

}

//vytvori novou kouli
void hodKouli() {
	if (snehoveKoule.size()<10) {
		CVector3D p(pozice[0], pozice[1], pozice[2]);
		CVector3D s(smerPohledu[0], smerPohledu[1], smerPohledu[2]);
		Koule k(p+s*4,s, RYCHLOST_KOULE, (random(-15,15)/15)*0.05+0.5);
		snehoveKoule.push_back(k);
	}
}

//nastaveni parametru pro prvni staticky pohled
void statickyPohled1(void) { 
	walkmode=false;
	pozice[0] = 15.0; pozice[1] = 15.0; pozice[2] = -15.0;
	uhelPohledu=120.0;
	beta=-20.0;
}

//nastaveni parametru pro druhy staticky pohled
void statickyPohled2(void) { 
	walkmode=false;
	pozice[0] = 29.0; pozice[1] = 15.0; pozice[2] = 29.0;
	uhelPohledu=-135.0;
	beta=-30.0;
}

//vypne nebo zapne snezeni
void prepniSnezeni(void) {
	snezi = !snezi;
}

//vypne nebo zapne walkmode
void prepniWalkmode(void) { 
	walkmode=!walkmode;
}

//Otoci pohled doprava
void otocDoprava() { 
	uhelPohledu += SMALL_ANGLE2;
	if( uhelPohledu > 360.0 ) uhelPohledu -= 360.0;
}

// Otoci pohled doleva
void otocDoleva() { 
	uhelPohledu -= SMALL_ANGLE2;
	if( uhelPohledu < -360.0 ) uhelPohledu += 360.0;
}

//Posune pozici dopredu
void posunDopredu() { 
	pozice[0] += delka_kroku * smerPohybu[0];
	pozice[1] += delka_kroku * smerPohybu[1];
	pozice[2] += delka_kroku * smerPohybu[2];
	if (pozice[0] > WORLD_SIZE_2-2) {pozice[0] = WORLD_SIZE_2-2;}
	if (pozice[0] < -WORLD_SIZE_2+2) {pozice[0] = -WORLD_SIZE_2+2;}
	if (pozice[2] > WORLD_SIZE_2-2) {pozice[2] = WORLD_SIZE_2-2;}
	if (pozice[2] < -WORLD_SIZE_2+2) {pozice[2] = -WORLD_SIZE_2+2;}
}

//Posune pozici dozadu
void posunDozadu() { 
	pozice[0] -= delka_kroku * smerPohybu[0];
	pozice[1] -= delka_kroku * smerPohybu[1];
	pozice[2] -= delka_kroku * smerPohybu[2];
	if (pozice[0] > WORLD_SIZE_2-2) {pozice[0] = WORLD_SIZE_2-2;}
	if (pozice[0] < -WORLD_SIZE_2+2) {pozice[0] = -WORLD_SIZE_2+2;}
	if (pozice[2] > WORLD_SIZE_2-2) {pozice[2] = WORLD_SIZE_2-2;}
	if (pozice[2] < -WORLD_SIZE_2+2) {pozice[2] = -WORLD_SIZE_2+2;}
}

//Otoci pohled nahoru
void otocNahoru() { 
	beta += SMALL_ANGLE2;
	if( beta > 75.0 ) beta = 75.0;
}

//Otoci pohled dolu
void otocDolu() { 
	beta -= SMALL_ANGLE2;
	if( beta < -65.0 ) beta = -65.0;
}

void Reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLfloat)w/h, 1.0, 10000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Idle(void) {

	sunAngle += sunAngleSpeed; //uhel pro obeh slunce
	if(sunAngle > 180.0) sunAngle = 0.0;
	
	if(whichFrame >= 9) {whichFrame = 6;}
	whichFrame+=0.005;

		double u = random(-2,2);
		double v = random(-2,2);
		CVector3D poz(u,(u+v)*0.1,v);
	VITR += poz;

	if (snezi) {
		// 
		pWorld.addRandom(1);

		// update particle worlds
		pWorld.update(timer.RealTime());
	}

	for(int i=0; i<snehoveKoule.size(); i++) {
	  snehoveKoule[i].update();
	  if (snehoveKoule[i].koliduje()) {

		  //pWorld.addRandom(1);
		  
		  Koule k(snehoveKoule[i].pozice, snehoveKoule[i].smerPohybu, 0, snehoveKoule[i].polomer);
		  kolize.push_back(k);
		  snehoveKoule.erase(snehoveKoule.begin()+i,snehoveKoule.begin()+i+1);
	  }
	  else if (snehoveKoule[i].jeVenku()) {
		  snehoveKoule.erase(snehoveKoule.begin()+i,snehoveKoule.begin()+i+1);
	  }
    }

	for (int j=0; j<snehulaci.size(); j++) {
		snehulaci[j].posun();
		snehulaci[j].rodic->LoadIdentity();
		snehulaci[j].rodic->Translate(snehulaci[j].pozicex,snehulaci[j].pozicey,snehulaci[j].pozicez);
	}

	if (dopredu)	{posunDopredu();}
	if (dozadu)		{posunDozadu();}
	if (doleva)		{otocDoleva();}
	if (doprava)	{otocDoprava();}
	if (nahoru)		{otocNahoru();}
	if (dolu)		{otocDolu();}

	glutWarpPointer(WIDTH/2,HEIGHT/2);

	glutPostRedisplay();
}



void Display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  smerPohledu[0] = cos(uhelPohledu*DEG_TO_RAD)*cos(beta*DEG_TO_RAD);
  smerPohledu[1] = sin(beta*DEG_TO_RAD);
  smerPohledu[2] = sin(uhelPohledu*DEG_TO_RAD)*cos(beta*DEG_TO_RAD);

  smerPohybu[0] = cos(uhelPohledu*DEG_TO_RAD);
  smerPohybu[1] = 0.0;
  smerPohybu[2] = sin(uhelPohledu*DEG_TO_RAD);

  GLdouble upVector[3];
  upVector[0] = -cos(uhelPohledu*DEG_TO_RAD)*sin(beta*DEG_TO_RAD);
  upVector[1] = cos(beta*DEG_TO_RAD);
  upVector[2] = -sin(uhelPohledu*DEG_TO_RAD)*sin(beta*DEG_TO_RAD);

  if (walkmode) {
	pozice[1] = vyska_postavy;
  }

  // nastaveni polohy pozorovatele a smeru pohledu na scenu
  gluLookAt(
    pozice[0], pozice[1], pozice[2], // eye = poloha kamery
    pozice[0]+smerPohledu[0], pozice[1]+smerPohledu[1], pozice[2]+smerPohledu[2], // center = bod na ktery koukame (smer pohledu = eye-center)
    upVector[0], upVector[1], upVector[2]  // up vektor (orientace kamery podel smeru pohledu)
  );
	
  if (slunceSviti) { //aktivace slunce
	slunce();
  }

  if (posvit) { //aktivace baterky
	baterka();
  }

  if (zamlzeno) { //aktivace mlhy
	mlha();
  }
  
  for(int i=0; i<snehoveKoule.size(); i++) {
	  snehoveKoule[i].vykreslit();
  }

  for(int i=0; i<kolize.size(); i++) {
	  kolize[i].vykreslit2();
  }

  // draw scene graph
  rootNode_p->Update();

  if (snezi) {
	// draw particle worlds
	pWorld.draw();
  }
  
  glutSwapBuffers(); 
}

void mySpecialKeyboard (int key, int x, int y) {
  switch (key) {
	case GLUT_KEY_RIGHT: {// otoceni smeru pohledu doprava
		walkmode = true;
		doprava = true;
		break;
	}
	case GLUT_KEY_LEFT: {// otoceni smeru pohledu doleva
		walkmode = true;
		doleva = true;
		break;
	}
	case GLUT_KEY_UP: { // krok vpred
		walkmode = true;
		dopredu = true;
		break;
	}
	case GLUT_KEY_DOWN: { // krok vzad
		walkmode = true;
		dozadu = true;
		break;
	}
	case GLUT_KEY_PAGE_UP: {// zvednuti smeru pohledu
		walkmode = true;
		nahoru = true;
		break;
	}
	case GLUT_KEY_PAGE_DOWN: {// skloneni smeru pohledu
		walkmode = true;
		dolu = true;
	  break;
	}
	default:
	  return;
  }
}

void mySpecialKeyboard2 (int key, int x, int y) {
  switch (key) {
	case GLUT_KEY_RIGHT: {// otoceni smeru pohledu doprava
		walkmode = true;
		doprava = false;
		break;
	}
	case GLUT_KEY_LEFT: {// otoceni smeru pohledu doleva
		walkmode = true;
		doleva = false;
		break;
	}
	case GLUT_KEY_UP: { // krok vpred
		walkmode = true;
		dopredu = false;
		break;
	}
	case GLUT_KEY_DOWN: { // krok vzad
		walkmode = true;
		dozadu = false;
		break;
	}
	case GLUT_KEY_PAGE_UP: {// zvednuti smeru pohledu
		walkmode = true;
		nahoru = false;
		break;
	}
	case GLUT_KEY_PAGE_DOWN: {// skloneni smeru pohledu
		walkmode = true;
		dolu = false;
	  break;
	}
	default:
	  return;
  }
}

void myKeyboard (unsigned char key, int x, int y) {
  switch (key) {
	case'q':
	case'Q':	
    case 27:		/* pri stisku klavesy Esc se aplikace ukonci */      
      exit (0);
      break;
	case 'w': 
	case 'W':
		walkmode = true;
		dopredu = true;
      break;
	case 's': 
	case 'S':
		walkmode = true;
		dozadu = true;
      break;
	case 'a': 
	case 'A':
		walkmode=true;
		doleva = true;
      break;
	case 'd': 
	case 'D':
		walkmode=true;
		doprava = true;
      break;
	case '1': 
		statickyPohled1();
      break;
	case '2': 
		statickyPohled2();
      break;
	case '4': 
		prepniWalkmode();
      break;
	case 'k': 
	case 'K':
		prepniSlunce();
      break;
	case 'l': 
	case 'L':
		prepniBaterku();
      break;
	case 'm': 
	case 'M':
		prepniMlhu();
      break;
	case 'n': 
	case 'N':
		prepniSnezeni();
      break;
	case 32: 
		hodKouli();
      break;
  }
}

void myKeyboard2 (unsigned char key, int x, int y) {
  switch (key) {
	case'q':
	case'Q':	
    case 27:		/* pri stisku klavesy Esc se aplikace ukonci */      
      exit (0);
      break;
	case 'w': 
	case 'W':
		walkmode = true;
		dopredu = false;
      break;
	case 's': 
	case 'S':
		walkmode = true;
		dozadu = false;
      break;
	case 'a': 
	case 'A':
		walkmode = true;
		doleva = false;
      break;
	case 'd': 
	case 'D':
		walkmode=true;
		doprava = false;
      break;
  }
}


void myMotion(int x, int y)	{
	if (walkmode) {
		xm[1]=WIDTH/2; //poloha mysi na stredu
		ym[1]=HEIGHT/2;
		xm[0]=x; //aktuali poloha mysi
		ym[0]=y;
		xm[2]=xm[0]-xm[1]; //rozdil v polohach
		ym[2]=ym[0]-ym[1];

		uhelPohledu += SMALL_ANGLE2*xm[2]/3; //pricteni rozdilu v x souradnici
		if(uhelPohledu > 360.0) uhelPohledu -= 360.0;
		if(uhelPohledu < -360.0) uhelPohledu += 360.0;

		beta -= SMALL_ANGLE2*ym[2]/3; //odecteni rozdilu v y souradnici
		if(beta > 75.0) beta = 75.0;
		if(beta < -65.0) beta = -65.0;
	}
}

//inicializace pøi startu
void init(void) {
	//printf("timer: %f", timer.RealTime());

	srand(time(NULL));	
	pWorld.init();
	//pWorld.printOut();
	
	nactiSoubor();
	initTextures();

	Snehulak s1(6.0, -0.2, -5.0, 0.99, 1, 0.8, 2.4, 0.6, 3.5);
	Snehulak s2(-7.0, -0.2, 7.0, 1.5, 1.5, 1.2, 3.6, 0.99, 5.3);
	Snehulak s3(10.0, -0.2, -12.0, 0.8, 0.8, 0.6, 1.9, 0.4, 2.7);
	snehulaci.push_back(s1);
	snehulaci.push_back(s2);
	snehulaci.push_back(s3);

	rootNode_p = new CTransformNode(); // koren grafu sceny

CPodstavecNode * podstavec = new CPodstavecNode();
	CTransformNode * podstavec2_p = new CTransformNode(); //PODSTAVEC PANELU
	podstavec2_p->Translate(-3.5, 0.5, 0.0);
	podstavec2_p->Rotate(-1, 0.0, 0.0, 1.0);
	podstavec2_p->Scale(0.2, 1.0, 1.0);
	podstavec2_p->AddChildNode(podstavec);
	rootNode_p->AddChildNode(podstavec2_p);

	CTransformNode * podstavec3_p = new CTransformNode(); //PANEL
	podstavec3_p->Translate(-3.5, 1.0, 0.0);
	podstavec3_p->Rotate(-45, 0.0, 0.0, 1.0);
	podstavec3_p->Scale(0.5*PANEL_SIZE_Y, 0.2, PANEL_SIZE_X);
	podstavec3_p->AddChildNode(podstavec);
	rootNode_p->AddChildNode(podstavec3_p);

CPanelNode * panel = new CPanelNode(); // OBRAZOVKA INFORMACNIHO PANELU
	CTransformNode * panel_p = new CTransformNode();
	panel_p->Translate(-3.07, 0.72, 0.0);
	panel_p->Rotate(90, 0.0, 1.0, 0.0);
	panel_p->Rotate(-45, 1.0, 0.0, 0.0);
	panel_p->Scale(0.5, 0.5, 1.0);
	panel_p->AddChildNode(panel);
	rootNode_p->AddChildNode(panel_p);

COBJNode *bench = new COBJNode(); // LAVICKA
	bench->Load(BENCH_FILE_NAME);

	CTransformNode * bench1_p = new CTransformNode();
	bench1_p->Translate(2.0, 0.78, -1.0);
	bench1_p->Rotate(-90.0, 0.0, 1.0, 0.0);
	bench1_p->Scale(2.0, 2.0, 2.0);
	bench1_p->AddChildNode(bench);
	rootNode_p->AddChildNode(bench1_p);

COBJNode *strom = new COBJNode(); // STROM
	strom->Load(STROM_FILE_NAME);

	CTransformNode * strom_p1 = new CTransformNode();
	strom_p1->Translate(-12.0, 5.0, 4.0);
	strom_p1->Scale(8.0, 10.0, 8.0);
	strom_p1->AddChildNode(strom);
	rootNode_p->AddChildNode(strom_p1);

	CTransformNode * strom_p2 = new CTransformNode();
	strom_p2->Translate(-12.0, 3.0, -10.0);
	strom_p2->Scale(4.0, 6.0, 4.0);
	strom_p2->AddChildNode(strom);
	rootNode_p->AddChildNode(strom_p2);

	CTransformNode * strom_p3 = new CTransformNode();
	strom_p3->Translate(4.0, 1.0, 3.0);
	strom_p3->Scale(3.0, 2.0, 3.0);
	strom_p3->Rotate(60,0,1,0);
	strom_p3->AddChildNode(strom);
	rootNode_p->AddChildNode(strom_p3);

CZemeNode * zeme = new CZemeNode(); // ZEME
	CTransformNode * zeme_p = new CTransformNode();
	zeme_p->AddChildNode(zeme);
	rootNode_p->AddChildNode(zeme_p);

CSkyboxNode * skybox = new CSkyboxNode(); // SKYBOX
	CTransformNode * skybox_p = new CTransformNode();
	skybox_p->Translate(0.0, -12.0, 0.0);
	skybox_p->AddChildNode(skybox);
	rootNode_p->AddChildNode(skybox_p);

CSnehulakNode * sneh = new CSnehulakNode();
	sneh->sn = &snehulaci[0];
	
	CTransformNode * sneh_p = new CTransformNode();
	snehulaci[0].rodic = sneh_p;
	sneh_p->AddChildNode(sneh);
	sneh_p->Translate(sneh->sn->pozicex,sneh->sn->pozicey,sneh->sn->pozicez);
	sneh_p->Rotate(60.0, 0.0, 1.0, 0.0);
	rootNode_p->AddChildNode(sneh_p);

CSnehulakNode * sneh2 = new CSnehulakNode();
	sneh2->sn = &snehulaci[1];
	
	CTransformNode * sneh_p2 = new CTransformNode();
	snehulaci[1].rodic = sneh_p2;
	sneh_p2->AddChildNode(sneh2);
	sneh_p2->Translate(sneh2->sn->pozicex,sneh2->sn->pozicey,sneh2->sn->pozicez);
	sneh_p2->Rotate(10.0, 0.0, 1.0, 0.0);
	rootNode_p->AddChildNode(sneh_p2);

CSnehulakNode * sneh3 = new CSnehulakNode();
	sneh3->sn = &snehulaci[2];
	
	CTransformNode * sneh_p3 = new CTransformNode();
	snehulaci[2].rodic = sneh_p3;
	sneh_p3->AddChildNode(sneh3);
	sneh_p3->Translate(sneh3->sn->pozicex,sneh3->sn->pozicey,sneh3->sn->pozicez);
	sneh_p3->Rotate(10.0, 0.0, 1.0, 0.0);
	rootNode_p->AddChildNode(sneh_p3);

	// ostatní inicializace
	glEnable(GL_LIGHT0);
	glutSetCursor(GLUT_CURSOR_NONE); //skryje kurzor
	glutWarpPointer(WIDTH/2,HEIGHT/2); //nastavi kurzor na stred okna

}

//prirazeni akci k jednotlivym polozkam
void myMenu(int polozkaMenu){ 
  switch(polozkaMenu) {
	case 1: //umisti pozorovatele na prvni staticke stanoviste
		statickyPohled1();
		break;
	case 2: //umisti pozorovatele na druhe staticke stanoviste
		statickyPohled2();		
		break;
    case 4: //walk mode
		prepniWalkmode();
      break;
	case 11:  //zapne nebo vypne baterku
		prepniBaterku();
      break;
    case 12:  //zapne nebo vypne slunce
		prepniSlunce();
      break;
    case 21:  //zapne nebo vypne slunce
		prepniMlhu();
      break;
    case 99: //ukonci aplikaci
      exit(0);
      break;
  }
  glutWarpPointer(WIDTH/2,HEIGHT/2);
}

/* Funkce zpracovavajici stisk/uvolneni libovolneho tlacitka mysi */
void myMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		hodKouli();
	}
}

void menu(void) {
	/* Podmenu Kamera */
	int menuKamera = glutCreateMenu(myMenu);
	glutAddMenuEntry("1. statický pohled (1)", 1);
	glutAddMenuEntry("2. statický pohled (2)", 2);
	glutAddMenuEntry("Walk mode ON/OFF (4)", 4);

	/* Podmenu Svetla */
	int menuSvetla = glutCreateMenu(myMenu);
	glutAddMenuEntry("Baterka ON/OFF (l)", 11);
	glutAddMenuEntry("Slunce ON/OFF (k)", 12);

	/* Vytvoreni halvniho menu */
	glutCreateMenu(myMenu);
	glutAddSubMenu("Kamera", menuKamera);
	glutAddSubMenu("Svetla", menuSvetla);
	glutAddMenuEntry("Mlha ON/OFF (m)", 21); 
	glutAddMenuEntry("Konec", 99);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initGL(void) {
  glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LEQUAL);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING); //zapne osvetleni
}

int main(int argc, char **argv) {
  timer.Reset();
  timer.Start();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);

  glutCreateWindow("MMA semestralni práce pro CAVE - particle system");

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutIdleFunc(Idle);
  glutKeyboardFunc(myKeyboard);
  glutKeyboardUpFunc(myKeyboard2);
  glutSpecialFunc(mySpecialKeyboard);
  glutSpecialUpFunc(mySpecialKeyboard2);
  glutPassiveMotionFunc(myMotion);
  glutMouseFunc(myMouse);

  menu();

  init();
  initGL();
	
  glutMainLoop();
  timer.Stop();
  return 0;        
}

