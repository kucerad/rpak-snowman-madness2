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


#endif // __SWITCHES_H
