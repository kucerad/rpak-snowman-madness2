#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

#include "TransformNode.h"


CTransformNode::CTransformNode(void) {

	LoadIdentity();
}


CTransformNode::~CTransformNode(void) {
}


void CTransformNode::Update(void) {

	// attributes should be saved/restored by drawing nodes
    glPushMatrix();

	// transforms are relative to the parent node
	glMultMatrixd( (GLdouble *)_matrix._matrix );

    CSceneNode::Update();

    glPopMatrix();
}


void CTransformNode::SetMatrix(const CMatrix& matrix_p) {
	
	_matrix = matrix_p;
}


void CTransformNode::SetMatrix(const double *matrix_p) {

	MATRIXCOPY(_matrix._matrix, matrix_p)
}


void CTransformNode::GetMatrix(CMatrix& matrix_p) {

	matrix_p = _matrix;
}


void CTransformNode::GetMatrix(double* matrix_p) {

	MATRIXCOPY(matrix_p, _matrix._matrix)
}


void CTransformNode::LoadIdentity(void) {

	_matrix.LoadIdentityMatrix();
}


void CTransformNode::Translate(double dx, double dy, double dz) {

	_matrix.Translate(dx, dy, dz);
}


void CTransformNode::Scale(double sx, double sy, double sz) {

	_matrix.Scale(sx, sy, sz);
}


void CTransformNode::Rotate(double angle, double px, double py, double pz) {

	_matrix.Rotate(px, py, pz, angle);
}
