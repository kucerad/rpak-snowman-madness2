
#ifndef __TRANSFORMNODE_H
#define __TRANSFORMNODE_H

#include "Matrix.h"
#include "SceneNode.h"


class CTransformNode : public CSceneNode {

public:

	CTransformNode(void);

	~CTransformNode(void);

	void Update(void);

	void SetMatrix(const CMatrix& matrix_p);
	void SetMatrix(const double* matrix_p);
	void GetMatrix(double *matrix_p);
	void GetMatrix(CMatrix& matrix_p);

	void LoadIdentity(void);
	void Translate(double dx, double dy, double dz);
	void Scale(double sx, double sy, double sz);
	void Rotate(double angle, double px, double py, double pz);

private:

	CMatrix _matrix;
};

#endif // of __TRANSFORMNODE_H
