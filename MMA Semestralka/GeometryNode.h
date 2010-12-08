
#ifndef __GEOMETRYNODE_H
#define __GEOMETRYNODE_H

#if CAVEMOD
	#include <cave_ogl.h>
#endif

//#pragma once
#include "SceneNode.h"
#include "glm.h"
#include <cstring>

#include <map>

typedef std::map<int, GLuint> DisplayListsMap;





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
#if CAVEMOD
	int			DLarr[CAVE_MAX_INDEXES];
#endif
};

#endif // of __GEOMETRYNODE_H
