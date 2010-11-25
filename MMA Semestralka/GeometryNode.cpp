
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

	// save all attributes
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_NORMALIZE);

	// draw model
	glCallList(_modelDL);

	// restore attributes
	glPopAttrib();

	// update the children
	CGeometryNode::Update();
}


bool COBJNode::Load(char* filename) {

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
		// create display list
		_modelDL = glmList(_model_p, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	}

	return true;
}