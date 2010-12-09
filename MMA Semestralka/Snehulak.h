#ifndef __SNEHULAK
#define __SNEHULAK 

#include "TransformNode.h"
#include "Vector.h"
#include <GL/glut.h>
#include "physics.h"
#include "Materials.h"

class Snehulak {
public:
	Snehulak(CVector3D _pozice, CVector3D _polomer, CVector3D _vyska);
	
	~Snehulak(void);
	
	//jednoduche vykresleni snehulaka
	void vykreslit(void);

	//posune snehulaka v danem smeru  - chuze snehulaka
	void posun(void);

	//zjisti, zda snehulak koliduje s vrzenou kouli
	bool koliduje(CVector3D pos, double polomerKoule);


	CVector3D pozice, polomer, vyska;
	double smer;
	CTransformNode * rodic; //ukazatel na rodice v grafu sceny

};

#endif __SNEHULAK