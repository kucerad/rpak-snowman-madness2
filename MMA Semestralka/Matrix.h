
/*
 * Matrix operations (class CMatrix)
 *
 * NOTES:
 * 1. 4x4 transformation matrices are stored in memory in column major order.
 * 2. Transformation of a point p by a matrix M is: p' = M * p
 * 3. Points/vertices are to be thought of as column vectors.
 *
 *        matrix            stored as      vector
 *
 *  | A11 A12 A13 Tx |    | 0 4  8 12 |     | 0 |
 *  | A21 A22 A23 Ty |    | 1 5  9 13 |     | 1 |
 *  | A31 A32 A33 Tz |    | 2 6 10 14 |     | 2 |
 *  | Px  Py  Pz  1  |    | 3 7 11 15 |     | 3 |
 * 
 * 4. Multiplication of matrices M' = M * M_new (like in OpenGL)
 *
 */

#ifndef __MATRIX_H
#define __MATRIX_H

#include <iostream>
#include <string>
#include "common.h"
#include "Vector.h"


class CVector3D;

#define MATRIXCOPY(a, b) \
   memcpy((void *) (a), (void *) (b), (size_t) sizeof(double)*16);


class CMatrix {

public:

 double _matrix[16];

  CMatrix() { }

  CMatrix& operator = (const CMatrix& obj) {
    MATRIXCOPY(_matrix, obj._matrix);
    return *this;
  }

  // Assignment operators
  CMatrix& operator+= (const CMatrix& matA);	// add-to
  CMatrix& operator-= (const CMatrix& matA);	// subtract-from
  CMatrix& operator*= (const CMatrix& matA);	// multiply by matrix
  CMatrix& operator*= (double value); 			// scale by scalar

  // Fundamental operations
  bool Invert();									// Invert the matrix
  void Transpose();									// Transpose the matrix
  friend CMatrix Invert(const CMatrix& matM);		// Invert a given matrix
  friend CMatrix Transpose(const CMatrix& matM);	// Transpose a given matrix

  void LoadZeroMatrix();
  void LoadIdentityMatrix();

  void Scale(double sx, double sy, double sz);
  void Rotate(double x, double y, double z, double angle);
  void Translate(double x, double y, double z);

  // Create various types of matrix.
  friend CMatrix IdentityMatrix();
  friend CMatrix ZeroMatrix();
  friend CMatrix TranslationMatrix(const CVector3D& location);
  friend CMatrix RotationXMatrix(double angle);
  friend CMatrix RotationYMatrix(double angle);
  friend CMatrix RotationZMatrix(double angle);
  friend CMatrix RotationYPRMatrix(double Yaw, double Pitch, double Roll);
  friend CMatrix RotationAxisMatrix(const CVector3D& axis, double angle);

  friend CMatrix ScaleMatrix(double sx, double sy, double sz);
  friend CMatrix GenRotation(const CVector3D& x, const CVector3D& y, const CVector3D& z);
  friend CMatrix MirrorX();
  friend CMatrix MirrorY();
  friend CMatrix MirrorZ();
  friend CMatrix RotationOnly(const CMatrix& matX);

  // Binary operators
  friend CMatrix operator+ (const CMatrix& matA, const CMatrix& matB);
  friend CMatrix operator- (const CMatrix& matA, const CMatrix& matB);
  friend CMatrix operator* (const CMatrix& matA, double value);
  friend CMatrix operator* (const CMatrix& matA, const CMatrix& matB);
  friend class CVector3D operator* (const CMatrix& matM, const CVector3D& vect);
  friend class CVector3D RotateOnly(const CMatrix& matM, const CVector3D& vect);

  // Overloaded output operator.
  friend std::ostream& operator<< (std::ostream& stream, const CMatrix& matM);

  double operator[] (int index) {
    return _matrix[index];
  }

private:

  static double _identityMatrix[16];
  static double _zeroMatrix[16];

};

#endif /* __MATRIX_H */
