#include "GeometryNode.h"



CGeometryNode::CGeometryNode(void) {
}


CGeometryNode::~CGeometryNode(void) {
}


void CGeometryNode::Update(void) {

	// update the children
	CSceneNode::Update();
}


COBJNode::COBJNode(void) {

	_model_p = NULL;
}


COBJNode::~COBJNode(void) {

	// clean up
	if(_model_p != NULL) {
		glDeleteLists(_modelDL, 1);
		glmDelete(_model_p);
		_model_p = NULL;
	}
}


void COBJNode::Update(void) { // Draw the geometry
	//printf("CAVE_INDEX %i\n", CAVE_INDEX);
	// save all attributes
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_NORMALIZE);
#if CAVEMOD

	// if defined DL for this thread

	if (DLarr[CAVE_INDEX]==-1){
		// insert in arr
		DLarr[CAVE_INDEX]=glmList(_model_p, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	}
	glCallList(DLarr[CAVE_INDEX]);
	//glCallList(_modelDL);
#else
	// draw model
	glCallList(_modelDL);
#endif
	// restore attributes
	glPopAttrib();

	// update the children
	CGeometryNode::Update();
}


bool COBJNode::Load(char* filename) {

#if CAVEMOD
	/*init DLarr */
	for (int i=0; i<10; i++){
		DLarr[i]=-1;
	}
#endif

	if ( _model_p == NULL ) { // model still not loaded
		// read model from file
		_model_p = glmReadOBJ(filename);

		if(_model_p == NULL)
			return false;

		// translate model to the origin and scale it
		// to fit in a unit cube around the origin = "unitize"
		glmUnitize(_model_p);
		// precompute normal vectors
		glmFacetNormals(_model_p);
		glmVertexNormals(_model_p, 90.0);
#if CAVEMOD
		// do not create DL... create on first update instead
		//_modelDL = glmList(_model_p, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
#else
		// create display list
		_modelDL = glmList(_model_p, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
#endif
	}

	return true;
}
