#include <math.h>
#include <iomanip>
#include "common.h"
#include "Matrix.h"
#include "Vector.h"


//using namespace std;

double CMatrix::_identityMatrix[16] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0 };

double CMatrix::_zeroMatrix[16] = {
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0 };



#define MAT(m, r, c)  (m)[(c)*4+(r)]
#define PRECISION_LIMIT (1.0e-15)

#define ACCUMULATE \
  if(t >= 0.0)     \
    pos += t;      \
  else             \
    neg += t;


/* Adapted from graphics gems II. */
bool CMatrix::Invert() {
 double out[16];
 register double det;
 register double pos, neg, t;

  /* Calculate the determinant of upper left 3x3 submatrix and determine if the matrix is singular */ 
  /* as limited by the double precision floating-point data representation..                       */
  pos = neg = 0.0;

  t =  _matrix[0] * _matrix[5] * _matrix[10];
  ACCUMULATE

  t =  _matrix[1] * _matrix[6] * _matrix[8];
  ACCUMULATE

  t =  _matrix[2] * _matrix[4] * _matrix[9];
  ACCUMULATE

  t = -_matrix[2] * _matrix[5] * _matrix[8];
  ACCUMULATE

  t = -_matrix[1] * _matrix[4] * _matrix[10];
  ACCUMULATE

  t = -_matrix[0] * _matrix[6] * _matrix[9];
  ACCUMULATE

  det = pos + neg;

  /* Is the submatrix A singular? */
  if((det == 0.0) || (fabs(det / (pos - neg)) < PRECISION_LIMIT)) { //  if(det*det < 1e-25) 
    /* Matrix M has no inverse */
    std::cerr << "CMatrix::Invert() - singular matrix" << std::endl;
    return false;
  }

  /* Calculate inverse(A) = adj(A) / det(A) */
  det = 1.0 / det;

  out[ 0] = (  (_matrix[5] * _matrix[10] - _matrix[6] * _matrix[9] ) * det);
  out[ 4] = (- (_matrix[4] * _matrix[10] - _matrix[6] * _matrix[8] ) * det);
  out[ 8] = (  (_matrix[4] * _matrix[ 9] - _matrix[5] * _matrix[8] ) * det);
  out[ 1] = (- (_matrix[1] * _matrix[10] - _matrix[2] * _matrix[9] ) * det);
  out[ 5] = (  (_matrix[0] * _matrix[10] - _matrix[2] * _matrix[8] ) * det);
  out[ 9] = (- (_matrix[0] * _matrix[ 9] - _matrix[1] * _matrix[8] ) * det);
  out[ 2] = (  (_matrix[1] * _matrix[ 6] - _matrix[2] * _matrix[5] ) * det);
  out[ 6] = (- (_matrix[0] * _matrix[ 6] - _matrix[2] * _matrix[4] ) * det);
  out[10] = (  (_matrix[0] * _matrix[ 5] - _matrix[1] * _matrix[4] ) * det);

  /* Do the translation part */
  out[12] = - (_matrix[12] * out[0] + _matrix[13] * out[4] + _matrix[14] * out[ 8]);
  out[13] = - (_matrix[12] * out[1] + _matrix[13] * out[5] + _matrix[14] * out[ 9]);
  out[14] = - (_matrix[12] * out[2] + _matrix[13] * out[6] + _matrix[14] * out[10]);

  out[15] = 1.0;
  out[3] = out[7] = out[11] = 0.0;

  MATRIXCOPY(_matrix, out);
   
  return true;
}

#undef MAT
#undef PRECISION_LIMIT
#undef ACCUMULATE


/* Invert the given matrix using the above inversion routine. */
CMatrix Invert(const CMatrix& matM) {
 CMatrix InvertMe = matM;
    
  if(InvertMe.Invert() == false)
    InvertMe.LoadIdentityMatrix();

  return InvertMe;
}


/* Transpose the matrix. */
void CMatrix::Transpose() {
 
#define SWAP(tmp, m, index1, index2) \
 tmp = m[index1]; \
 m[index1] = m[index2]; \
 m[index2] = tmp;
  
 double temp;
 
  SWAP(temp, _matrix,  4,  1);
  SWAP(temp, _matrix,  8,  2);
  SWAP(temp, _matrix, 12,  3);
  SWAP(temp, _matrix,  9,  6);
  SWAP(temp, _matrix, 13,  7);
  SWAP(temp, _matrix, 14, 11);
      
#undef SWAP
}


/* Transpose the given matrix using the transpose routine above. */
CMatrix Transpose(const CMatrix& matM) {
 CMatrix TransposeMe = matM;

  TransposeMe.Transpose();

  return TransposeMe;
}


void CMatrix::LoadIdentityMatrix() {

  MATRIXCOPY(_matrix, _identityMatrix);
}


/* Construct an identity matrix. */
CMatrix IdentityMatrix() {
 CMatrix matM;

  MATRIXCOPY(matM._matrix, CMatrix::_identityMatrix);

  return matM;
}


void CMatrix::LoadZeroMatrix() {

 MATRIXCOPY(_matrix, _zeroMatrix);
}


/* Construct a zero matrix. */
CMatrix ZeroMatrix() {
 CMatrix matM;

  MATRIXCOPY(matM._matrix, CMatrix::_zeroMatrix);

  return matM;
}


void CMatrix::Translate(double x, double y, double z) {

  //  _matrix[12] = _matrix[0] * x + _matrix[4] * y + _matrix[8]  * z + _matrix[12];
  //  _matrix[13] = _matrix[1] * x + _matrix[5] * y + _matrix[9]  * z + _matrix[13];
  //  _matrix[14] = _matrix[2] * x + _matrix[6] * y + _matrix[10] * z + _matrix[14];
  //  _matrix[15] = _matrix[3] * x + _matrix[7] * y + _matrix[11] * z + _matrix[15];
  _matrix[12] += _matrix[0] * x + _matrix[4] * y + _matrix[8]  * z;
  _matrix[13] += _matrix[1] * x + _matrix[5] * y + _matrix[9]  * z;
  _matrix[14] += _matrix[2] * x + _matrix[6] * y + _matrix[10] * z;
  _matrix[15] += _matrix[3] * x + _matrix[7] * y + _matrix[11] * z;
}


/* Construct a translation matrix given the location to translate to. */
CMatrix TranslationMatrix(const CVector3D& location) {
 CMatrix matM = IdentityMatrix();

  matM._matrix[12] = location._vector[X];
  matM._matrix[13] = location._vector[Y];
  matM._matrix[14] = location._vector[Z];

  return matM;
}


/* Construct a rotation matrix.  Rotates Angle degrees about the X axis. */
CMatrix RotationXMatrix(double angle) {
 CMatrix matM;
 double ang = angle * CMath::_deg2rad;
 double cosine = cos(ang);
 double sine = sin(ang);

  matM.LoadIdentityMatrix();

  matM._matrix[5] = cosine;
  matM._matrix[9] = -sine;
  matM._matrix[6] = sine;
  matM._matrix[10] = cosine;

  return matM;
}


/* Construct a rotation matrix.  Rotates Angle degrees about the Y axis. */
CMatrix RotationYMatrix(double angle) {
 CMatrix matM;
 double ang = angle * CMath::_deg2rad;
 double cosine = cos(ang);
 double sine = sin(ang);

  matM.LoadIdentityMatrix();

  matM._matrix[0] = cosine;
  matM._matrix[8] = -sine;
  matM._matrix[2] = sine;
  matM._matrix[10] = cosine;

  return matM;
}


/* Construct a rotation matrix.  Rotates Angle degrees about the Z axis. */
CMatrix RotationZMatrix(double angle) {
 CMatrix matM;
 double ang = angle * CMath::_deg2rad;
 double cosine = cos(ang);
 double sine = sin(ang);
 
  matM.LoadIdentityMatrix();

  matM._matrix[0] = cosine;
  matM._matrix[4] = -sine;
  matM._matrix[1] = sine;
  matM._matrix[5] = cosine;

  return matM;
}


/* Construct a yaw-pitch-roll rotation matrix.	Rotate Yaw    */
/* radians about the XY axis, rotate Pitch radians in the     */
/* plane defined by the Yaw rotation, and rotate Roll radians */
/* about the axis defined by the previous two angles.         */
CMatrix RotationYPRMatrix(double Yaw, double Pitch, double Roll) {
 CMatrix matM;
 double ch = cos(Yaw);
 double sh = sin(Yaw);
 double cp = cos(Pitch);
 double sp = sin(Pitch);
 double cr = cos(Roll);
 double sr = sin(Roll);

  matM._matrix[0] = ch*cr + sh*sp*sr;
  matM._matrix[4] = -ch*sr + sh*sp*cr;
  matM._matrix[8] = sh*cp;
  matM._matrix[1] = sr*cp;
  matM._matrix[5] = cr*cp;
  matM._matrix[9] = -sp;
  matM._matrix[2] = -sh*cr - ch*sp*sr;
  matM._matrix[6] = sr*sh + ch*sp*cr;
  matM._matrix[10] = ch*cp;

  matM._matrix[12] = matM._matrix[13] = matM._matrix[14] = 0;
  matM._matrix[3] = matM._matrix[7] = matM._matrix[11] = 0;

  matM._matrix[15] = 1;

  return matM;
}


/* Construct a rotation of a given angle about a given axis.      */
/* Derived from Eric Haines's SPD (Standard Procedural Database). */
/* Code was taken from the Mesa 3D library.                       */ 
CMatrix RotationAxisMatrix(const CVector3D& axis, double angle) {
 CMatrix matM;
 double x, y, z, mag, s, c;
 double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

  x =  axis._vector[X];
  y =  axis._vector[Y];
  z =  axis._vector[Z];

  mag = sqrt(x*x + y*y + z*z);

  if(mag == 0.0) { /* badly specified axis - so return identity matrix */
    matM.LoadIdentityMatrix();

    return matM;
  }

  x /= mag;
  y /= mag;
  z /= mag;

  /*
   *     Arbitrary axis rotation matrix.
   *
   *  This is composed of 5 matrices, Rz, Ry, T, Ry', Rz', multiplied
   *  like so:  Rz * Ry * T * Ry' * Rz'.  T is the final rotation
   *  (which is about the X-axis), and the two composite transforms
   *  Ry' * Rz' and Rz * Ry are (respectively) the rotations necessary
   *  from the arbitrary axis to the X-axis then back.  They are
   *  all elementary rotations.
   *
   *  Rz' is a rotation about the Z-axis, to bring the axis vector
   *  into the x-z plane.  Then Ry' is applied, rotating about the
   *  Y-axis to bring the axis vector parallel with the X-axis.  The
   *  rotation about the X-axis is then performed.  Ry and Rz are
   *  simply the respective inverse transforms to bring the arbitrary
   *  axis back to it's original orientation.  The first transforms
   *  Rz' and Ry' are considered inverses, since the data from the
   *  arbitrary axis gives you info on how to get to it, not how
   *  to get away from it, and an inverse must be applied.
   *
   *  The basic calculation used is to recognize that the arbitrary
   *  axis vector (x, y, z), since it is of unit length, actually
   *  represents the sines and cosines of the angles to rotate the
   *  X-axis to the same orientation, with theta being the angle about
   *  Z and phi the angle about Y (in the order described above)
   *  as follows:
   *
   *  cos ( theta ) = x / sqrt ( 1 - z^2 )
   *  sin ( theta ) = y / sqrt ( 1 - z^2 )
   *
   *  cos ( phi ) = sqrt ( 1 - z^2 )
   *  sin ( phi ) = z
   *
   *  Note that cos ( phi ) can further be inserted to the above
   *  formulas:
   *
   *  cos ( theta ) = x / cos ( phi )
   *  sin ( theta ) = y / sin ( phi )
   *
   *  ...etc.  Because of those relations and the standard trigonometric
   *  relations, it is pssible to reduce the transforms down to what
   *  is used below.  It may be that any primary axis chosen will give the
   *  same results (modulo a sign convention) using thie method.
   *
   *  Particularly nice is to notice that all divisions that might
   *  have caused trouble when parallel to certain planes or
   *  axis go away with care paid to reducing the expressions.
   *  After checking, it does perform correctly under all cases, since
   *  in all the cases of division where the denominator would have
   *  been zero, the numerator would have been zero as well, giving
   *  the expected result.
   */

  s = sin( angle * (M_PI / 180.0) );
  c = cos( angle * (M_PI / 180.0) );

  xx = x * x;
  yy = y * y;
  zz = z * z;
  xy = x * y;
  yz = y * z;
  zx = z * x;
  xs = x * s;
  ys = y * s;
  zs = z * s;
  one_c = 1.0F - c;

  matM._matrix[ 0] = (one_c * xx) + c;
  matM._matrix[ 4] = (one_c * xy) - zs;
  matM._matrix[ 8] = (one_c * zx) + ys;
  matM._matrix[12] = 0.0;

  matM._matrix[ 1] = (one_c * xy) + zs;
  matM._matrix[ 5] = (one_c * yy) + c;
  matM._matrix[ 9] = (one_c * yz) - xs;
  matM._matrix[13] = 0.0;

  matM._matrix[ 2] = (one_c * zx) - ys;
  matM._matrix[ 6] = (one_c * yz) + xs;
  matM._matrix[10] = (one_c * zz) + c;
  matM._matrix[14] = 0.0;

  matM._matrix[ 3] = 0.0;
  matM._matrix[ 7] = 0.0;
  matM._matrix[11] = 0.0;
  matM._matrix[15] = 1.0;
 
  return matM;
}
    

void CMatrix::Rotate(double x, double y, double z, double angle) {
 CMatrix matM;

  matM = RotationAxisMatrix(CVector3D(x, y, z), angle);  /* build otation matrix */
  *this *= matM;                                         /* multiply matrices    */
}


/* Construct a scale matrix given the X, Y, and Z parameters */
/* to scale by.  To scale uniformly, let X==Y==Z.            */
CMatrix ScaleMatrix(double sx, double sy, double sz) {
 CMatrix matM = IdentityMatrix();

  matM._matrix[0] = sx;
  matM._matrix[5] = sy;
  matM._matrix[10] = sz;

  return matM;
}


void CMatrix::Scale(double sx, double sy, double sz) {

  _matrix[0] *= sx;   _matrix[4] *= sy;   _matrix[8]  *= sz;
  _matrix[1] *= sx;   _matrix[5] *= sy;   _matrix[9]  *= sz;
  _matrix[2] *= sx;   _matrix[6] *= sy;   _matrix[10] *= sz;
  _matrix[3] *= sx;   _matrix[7] *= sy;   _matrix[11] *= sz;
}


/* Construct a rotation matrix that makes the x, y, z axes */
/* correspond to the vectors given.                        */
CMatrix GenRotation(const CVector3D& x, const CVector3D& y, const CVector3D& z) {
 CMatrix M = IdentityMatrix();

  M._matrix[0] = x._vector[X];
  M._matrix[4] = x._vector[Y];
  M._matrix[8] = x._vector[Z];
  M._matrix[1] = y._vector[X];
  M._matrix[5] = y._vector[Y];
  M._matrix[9] = y._vector[Z];
  M._matrix[2] = z._vector[X];
  M._matrix[6] = z._vector[Y];
  M._matrix[10] = z._vector[Z];

  return M;
}


/* Construct various "mirror" matrices, which flip coordinate */
/* signs in the various axes specified.                       */
CMatrix MirrorX() {
 CMatrix matM = IdentityMatrix();

  matM._matrix[0] = -1;

  return matM;
}


CMatrix MirrorY() {
 CMatrix matM = IdentityMatrix();
  
  matM._matrix[5] = -1;

  return matM;
}


CMatrix MirrorZ() {
 CMatrix matM = IdentityMatrix();

  matM._matrix[10] = -1;

  return matM;
}


CMatrix RotationOnly(const CMatrix& matX) {
 CMatrix matM = matX;

  matM._matrix[12] = matM._matrix[13] = matM._matrix[14] = 0.0;

  return matM;
}


/* Add corresponding elements of the two matrices. */
CMatrix& CMatrix::operator+= (const CMatrix& matA) {
  
  for(int i=0; i<16; i++)
    _matrix[i] += matA._matrix[i];

  return *this;
}


/* Subtract corresponding elements of the matrices. */
CMatrix& CMatrix::operator-= (const CMatrix& matA) {
 
  for(int i=0; i<16; i++)
    _matrix[i] -= matA._matrix[i];

  return *this;
}


/* Scale each element of the matrix by A. */
CMatrix& CMatrix::operator*= (double value) {

  for(int i=0; i<16; i++)
    _matrix[i] *= value;

  return *this;
}


/* Multiply two matrices. */
CMatrix& CMatrix::operator*= (const CMatrix& matA) {

  for(int i=0; i<4; i++) {
   double ai0 = _matrix[i];
   double ai1 = _matrix[i+4];
   double ai2 = _matrix[i+8]; 
   double ai3 = _matrix[i+12];

    _matrix[i] = ai0*matA._matrix[0] + ai1*matA._matrix[1] + ai2*matA._matrix[2] + ai3*matA._matrix[3];
    _matrix[i+4] = ai0*matA._matrix[4] + ai1*matA._matrix[5] + ai2*matA._matrix[6] + ai3*matA._matrix[7];
    _matrix[i+8] = ai0*matA._matrix[8] + ai1*matA._matrix[9] + ai2*matA._matrix[10] + ai3*matA._matrix[11];
    _matrix[i+12] = ai0*matA._matrix[12] + ai1*matA._matrix[13] + ai2*matA._matrix[14] + ai3*matA._matrix[15];
  }

  return *this;
}


/* Add corresponding elements of the matrices. */
CMatrix operator+ (const CMatrix& matA, const CMatrix& matB) {
 CMatrix ret;

  for(int i=0; i<16; i++)
    ret._matrix[i] = matA._matrix[i] + matB._matrix[i];

  return ret;
}


/* Subtract corresponding elements of the matrices. */
CMatrix operator- (const CMatrix& matA, const CMatrix& matB) {
 CMatrix ret;

  for(int i=0; i<16; i++)
    ret._matrix[i] = matA._matrix[i] - matB._matrix[i];

  return ret;
}


/* Multiply matrices. */
CMatrix operator* (const CMatrix& matA, const CMatrix& matB) {
 CMatrix ret;

  for(int i=0; i<4; i++) {
   double ai0 = matA._matrix[i];
   double ai1 = matA._matrix[i+4];
   double ai2 = matA._matrix[i+8]; 
   double ai3 = matA._matrix[i+12];

    ret._matrix[i] = ai0*matB._matrix[0] + ai1*matB._matrix[1] + ai2*matB._matrix[2] + ai3*matB._matrix[3];
    ret._matrix[i+4] = ai0*matB._matrix[4] + ai1*matB._matrix[5] + ai2*matB._matrix[6] + ai3*matB._matrix[7];
    ret._matrix[i+8] = ai0*matB._matrix[8] + ai1*matB._matrix[9] + ai2*matB._matrix[10] + ai3*matB._matrix[11];
    ret._matrix[i+12] = ai0*matB._matrix[12] + ai1*matB._matrix[13] + ai2*matB._matrix[14] + ai3*matB._matrix[15];
  }

  return ret;
}


/* Transform a vector by a matrix. */
CVector3D operator* (const CMatrix& matM, const CVector3D& vect) {
 CVector3D ret;
 double denom;

  ret._vector[X] = vect._vector[X]*matM._matrix[0] + vect._vector[Y]*matM._matrix[4] + vect._vector[Z]*matM._matrix[8] + vect._vector[W]*matM._matrix[12];
  ret._vector[Y] = vect._vector[X]*matM._matrix[1] + vect._vector[Y]*matM._matrix[5] + vect._vector[Z]*matM._matrix[9] + vect._vector[W]*matM._matrix[13];
  ret._vector[Z] = vect._vector[X]*matM._matrix[2] + vect._vector[Y]*matM._matrix[6] + vect._vector[Z]*matM._matrix[10] + vect._vector[W]*matM._matrix[14];
  denom = vect._vector[X]*matM._matrix[3] + vect._vector[Y]*matM._matrix[7] + vect._vector[Z]*matM._matrix[11] + vect._vector[W]*matM._matrix[15];

  if(denom != 1.0)
    ret /= denom;

  return ret;
}


/* Apply the rotation portion of a matrix to a vector. */
/* for normal and direction transformation             */ 
CVector3D RotateOnly(const CMatrix& matM, const CVector3D& vect) {
 CVector3D ret;
 double denom;

  ret._vector[X] = vect._vector[X]*matM._matrix[0] + vect._vector[Y]*matM._matrix[4] + vect._vector[Z]*matM._matrix[8];
  ret._vector[Y] = vect._vector[X]*matM._matrix[1] + vect._vector[Y]*matM._matrix[5] + vect._vector[Z]*matM._matrix[9];
  ret._vector[Z] = vect._vector[X]*matM._matrix[2] + vect._vector[Y]*matM._matrix[6] + vect._vector[Z]*matM._matrix[10];
  denom = vect._vector[X]*matM._matrix[3] + vect._vector[Y]*matM._matrix[7] + vect._vector[Z]*matM._matrix[11] + vect._vector[W]*matM._matrix[15];

  if(denom != 1.0)
    ret /= denom;

  return ret;
}


/* Scale each element of the matrix by value. */
CMatrix operator* (const CMatrix& matA, double value) {
 CMatrix ret;

  for(int i=0; i<16; i++)
    ret._matrix[i] = matA._matrix[i] * value;

  return ret;
}


/* Overloaded << for C++-style output. */
std::ostream& operator<< (std::ostream& stream, const CMatrix& matM) {

  stream << "[_CMatrix_]" << std::endl;
  for(int i=0; i<4; i++) {
    stream << std::setprecision(2) << std::setw(5) 
      << "| "
      << matM._matrix[i]   << " " << matM._matrix[i+4] << " "
      << matM._matrix[i+8] << " " << matM._matrix[i+12]
      << " |" << std::endl;
  }
  stream << std::endl;

  return stream;
}

