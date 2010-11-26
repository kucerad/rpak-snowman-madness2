#ifndef __PARTICLE
#define __PARTICLE 
#include "Vector.h"
#include "physics.h"

typedef CVector3D vec;


class Particle{
public:
	Particle(vec _position, vec _speed, vec _axis, double _angle, vec _normal, double _aspeed, double _mass=10, double _size=1, double _life=500):
	  position(_position),
	  speed(_speed),
	  axis(_axis),
	  angle(_angle),
	  normal(Normalize(_normal)),
	  aspeed(_aspeed),
	  mass(_mass),
	  size(_size),
	  life(_life){}

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
	void printOut(void){
		printf("P[%f, %f, %f] speed[%f, %f, %f]\n", position[X], position[Y], position[Z], speed[X], speed[Y], speed[Z]);
	}
	void update(void){
		//if (life<0){
		//	return;
		//}
		life--;

		// celkova rychlost v prostredi
		vec S( - VITR + speed );
		
		// zavislost na natoceni k normale
		double	k = DotProduct(Normalize(S), normal);
		vec		Nspeed( DotProduct(speed, normal) * normal );
		vec		Tspeed( speed - Nspeed );
		double	r = Length(Nspeed);
		vec		Fn( Nspeed * (-0.5 * r*r*size*size*KOEF_ODPORU*HUSTOTA_PROSTREDI) );
		vec		Ft( Tspeed * KOEF_TRENI * HUSTOTA_PROSTREDI );

		// zrychleni
		speed += (GRAVITACE + Fn + Ft)/mass;
		// posun
		position += speed/100;

		// natocit a update normaly
		

		/*	Ft.copy(u.multiplyC( - 0.5 * C * ro * u.lengthSqr() * S));
			//trace("Fn="+Fn);

			// silu rozlozime na normalovou silu a podelnou silu...
			var Fn: Vector2D = normal.getNormalized();
			var Fp: Vector2D = Fn.getOrthogonalRight();
			Ft.split(Fn, Fp);
			//Fp.copy(Fp.minus(v.scale(0.001*S)));
			var koef = 1;
			ReactiveForce.copy(Fn.plus(Fp.multiplyC(koef).plus(v.getNormalized().scale(-0.01*v.lengthSqr()))));
			outForce.copy(ReactiveForce.plus(Gravity));
			velocity.add(outForce.divideC(mass));
			//position.add(velocity);
			var d:Vector2D = new Vector2D(scene.stageWidth, scene.stageHeight);
			position.add(velocity.plus(d));
			
			// restrict to fall in loop
			position.mod(scene.stageWidth, scene.stageHeight);
			
			//if (v.cosBetween(normal.getOrthogonalRight())>0 && Math.random()>0.8){
			
			// rotate the leaf while falling
			var cuhel:Number = wind.getNormalized().dot(velocity.getNormalized());
			//trace (cuhel);
			z = Math.sin(time*0.1)*velocity.length();
			//var r:Number = Math.random()*2-1;
			//omega +=  ( r * 0.05 + z * 0.5) / mass; 
			omega +=  (cuhel*0.5 + z * 0.1) / mass; 
			angle=omega; 
			
			//trace("angle = "+angle);
			updateOrientation(angle);

		position = 
		*/
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
				glEnable(GL_TEXTURE_2D);
				// povoleni michani barev
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glTranslated(position[X],position[Y],position[Z]);
				//glRotated(angle, axis[X], axis[Y], axis[Z]);
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