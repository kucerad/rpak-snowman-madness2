
/*
 * Vector operations (class CVector3D)
 *
 * NOTES:
 * 1. Points/vertices are to be thought of as column vectors (class CVector3D).
 * 2. Transformation of a point p by a matrix M is: p' = M * p
 * 3. 4x4 transformation matrices are stored in memory in column major order (class CMatrix).
 */

#ifndef __VECTOR_H
#define __VECTOR_H

#include <iostream>
#include <math.h>
#include "common.h"
#include "Matrix.h"


// Forward-declare some classes
class CMatrix;

#define EPSF 1E-7  /* when tested float to zero */
#define EPSD 1E-10 /* when tested double to zero */

class CVector3D {

public:

  double _vector[4];

  // constructors
  CVector3D() {
    _vector[W] = 1.0;
  }

  CVector3D(double x, double y, double z) {
    _vector[X] = x;
    _vector[Y] = y;
    _vector[Z] = z;
    _vector[W] = 1.0;
  }

  CVector3D(double value) {
    _vector[X] = _vector[Y] = _vector[Z] = value;
    _vector[W] = 1.0;
  }

  CVector3D(const CVector3D& vect) {
    _vector[X] = vect._vector[X];
    _vector[Y] = vect._vector[Y];
    _vector[Z] = vect._vector[Z];
    _vector[W] = vect._vector[W];
  }

  CVector3D& operator=(const CVector3D& obj) {
    _vector[X] = obj._vector[X];
    _vector[Y] = obj._vector[Y];
    _vector[Z] = obj._vector[Z];
    _vector[W] = obj._vector[W];

    return *this;
  }
  void printOut(void){
	printf("[%f, %f, %f]\n", _vector[X], _vector[Y],_vector[Z]);
  } 
  void Set(double x, double y, double z, double w) {
    _vector[X] = x;
    _vector[Y] = y;
    _vector[Z] = z;
    _vector[W] = w;
  }

  void Set(double x, double y, double z) {
    _vector[X] = x;
    _vector[Y] = y;
    _vector[Z] = z;
  }

  double& operator[] (int index) {
    return _vector[index];
  }

  // Unary operators
  CVector3D operator+ () const;
  CVector3D operator- () const;

  // Assignment operators
  CVector3D& operator+= (const CVector3D& vect);
  CVector3D& operator-= (const CVector3D& vect);
  CVector3D& operator*= (const CVector3D& vect);
  CVector3D& operator*= (double value);
  CVector3D& operator/= (double value);

  // Binary operators
  friend CVector3D operator+ (const CVector3D& vectA, const CVector3D& vectB);
  friend CVector3D operator- (const CVector3D& vectA, const CVector3D& vectB);
  friend CVector3D operator* (const CVector3D& vectA, const CVector3D& vectB);
  friend CVector3D operator* (const CVector3D& vectA, double value);
  friend CVector3D operator* (double value, const CVector3D& vectB);
  friend CVector3D operator* (const CMatrix&, const CVector3D&);
  friend CVector3D operator/ (const CVector3D& vectA, const CVector3D& vectB);
  friend CVector3D operator/ (const CVector3D& vectA, double value);

  friend std::ostream& operator<< (std::ostream& stream, const CVector3D& vectA);

  friend double DotProduct(const CVector3D& vectA, const CVector3D& vectB);
  friend CVector3D CrossProduct (const CVector3D& vectA, const CVector3D& vectB);

  friend double Length(const CVector3D& vect);
  friend CVector3D Normalize(const CVector3D& vectA);

  // return pointer directly to the _vector variable - it allows direct modification of vector
  inline double *GetPointer(void) {
    return _vector;
  }

};


inline double Length(const CVector3D& vect) {
  
  return sqrt(vect._vector[X]*vect._vector[X] + vect._vector[Y]*vect._vector[Y] + vect._vector[Z]*vect._vector[Z]);
}


inline double DotProduct(const CVector3D& vectA, const CVector3D& vectB) {

  return vectA._vector[X]*vectB._vector[X] + vectA._vector[Y]*vectB._vector[Y] + vectA._vector[Z]*vectB._vector[Z];
}


inline CVector3D CVector3D::operator+ () const {
  
  return *this;
}


inline CVector3D CVector3D::operator- () const {
  
  return CVector3D(-_vector[X], -_vector[Y], -_vector[Z]);
}


inline CVector3D& CVector3D::operator+= (const CVector3D& vectA) {
  
  _vector[X] += vectA._vector[X];  
  _vector[Y] += vectA._vector[Y];  
  _vector[Z] += vectA._vector[Z];
  
  return *this;
}


inline CVector3D& CVector3D::operator-= (const CVector3D& vectA) {
  
  _vector[X] -= vectA._vector[X];  
  _vector[Y] -= vectA._vector[Y];  
  _vector[Z] -= vectA._vector[Z];
  
  return *this;
}


inline CVector3D& CVector3D::operator*= (double value) {
  
  _vector[X] *= value;  
  _vector[Y] *= value;  
  _vector[Z] *= value;
  
  return *this;
}


inline CVector3D& CVector3D::operator/= (double value) {
  
  _vector[X] /= value;  
  _vector[Y] /= value;  
  _vector[Z] /= value;
  
  return *this;
}


inline CVector3D& CVector3D::operator*= (const CVector3D& vectA) {
  
  _vector[X] *= vectA._vector[X];
  _vector[Y] *= vectA._vector[Y];  
  _vector[Z] *= vectA._vector[Z];
  
  return *this;
}


inline CVector3D operator+ (const CVector3D& vectA, const CVector3D& vectB) {
  
  return CVector3D(
    vectA._vector[X] + vectB._vector[X], 
    vectA._vector[Y] + vectB._vector[Y], 
    vectA._vector[Z] + vectB._vector[Z]);
}


inline CVector3D operator- (const CVector3D& vectA, const CVector3D& vectB) {
  
  return CVector3D(
    vectA._vector[X] - vectB._vector[X], 
    vectA._vector[Y] - vectB._vector[Y], 
    vectA._vector[Z] - vectB._vector[Z]);
}


inline CVector3D operator* (const CVector3D& vectA, const CVector3D& vectB) {
  
  return CVector3D(
    vectA._vector[X] * vectB._vector[X],
    vectA._vector[Y] * vectB._vector[Y], 
    vectA._vector[Z] * vectB._vector[Z]);
}


inline CVector3D operator* (const CVector3D& vectA, double value) {
  
  return CVector3D(
    vectA._vector[X] * value,
    vectA._vector[Y] * value, 
    vectA._vector[Z] * value);
}


inline CVector3D operator* (double value, const CVector3D& vectB) {
  
  return vectB * value;
}


inline CVector3D operator/ (const CVector3D& vectA, const CVector3D& vectB) {
  
  return CVector3D(
    vectA._vector[X] / vectB._vector[X],
    vectA._vector[Y] / vectB._vector[Y], 
    vectA._vector[Z] / vectB._vector[Z]);
}


inline CVector3D operator/ (const CVector3D& vectA, double value) {
  
  return CVector3D(
    vectA._vector[X] / value,
    vectA._vector[Y] / value,
    vectA._vector[Z] / value);
}


inline CVector3D CrossProduct(const CVector3D& vectA, const CVector3D& vectB) {
  
  return CVector3D(
    vectA._vector[Y] * vectB._vector[Z] - vectA._vector[Z] * vectB._vector[Y],
    vectA._vector[Z] * vectB._vector[X] - vectA._vector[X] * vectB._vector[Z],
    vectA._vector[X] * vectB._vector[Y] - vectA._vector[Y] * vectB._vector[X]);
}

#endif /* __VECTOR_H */
