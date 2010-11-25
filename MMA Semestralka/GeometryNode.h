
#ifndef __GEOMETRYNODE_H
#define __GEOMETRYNODE_H

//#pragma once
#include "SceneNode.h"
#include "glm.h"

class CGeometryNode : public CSceneNode {

public:

	CGeometryNode(void);

	~CGeometryNode(void);

	virtual void Update(void);
};


class COBJNode : public CGeometryNode {

public:

	COBJNode(void);

	~COBJNode(void);

	bool Load(char* filename);

	virtual void Update(void);

private:

	GLMmodel* _model_p;
	GLuint _modelDL;   // display list ID for OBJ model
};

#endif // of __GEOMETRYNODE_H
