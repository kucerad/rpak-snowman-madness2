#ifndef __SWITCHES_H
#define __SWITCHES_H

bool posvit = false;		//kdyz true, tak bude svitit baterka
bool slunceSviti = true;	//kdyz true, tak bude svitit slunce
bool zamlzeno = false;		//kdyz true, bude husta mlha
bool snezi = true;			//kdyz true, bude snezit
bool dopredu = false;		//promenne pro plynuly pohyb
bool dozadu = false;
bool doleva = false;
bool doprava = false;
bool nahoru = false;
bool dolu = false;
bool debug = false;			// on/off debug mode
bool pause2 = false;			//zastavuje update vsech objektu
float rychlost = 0;
CMatrix rotMatrix;

// stav tlacitek v minulem kroku
bool b0 = false;
bool b1 = false;
bool b2 = false;
bool b3 = false;
bool b4 = false;
bool b5 = false;
bool b6 = false;
bool b7 = false;

int zbran = 0;

float velikost_koule0 = 0.5;
float velikost_koule1 = 0.17;

#endif // __SWITCHES_H
