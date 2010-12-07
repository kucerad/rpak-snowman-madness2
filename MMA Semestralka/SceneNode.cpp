#include "SceneNode.h"


CSceneNode::CSceneNode(CSceneNode* parentNode_p, const char* nodeName_p) : m_Name_p(nodeName_p) {

	m_Parent_p = parentNode_p;
}


CSceneNode::~CSceneNode(void) {

	m_Parent_p = NULL;
	m_Children.clear();
}


// this method goes through the children-vector and calls the Update() method on each child that does not equal NULL. 
void CSceneNode::Update(void) {

	if(!m_Children.empty()) {

		size_t numChildren = m_Children.size();
		for(size_t i = 0; i < numChildren; ++i) {

			if(m_Children[i] != NULL) {
				m_Children[i]->Update();
			}
		}
	}
}


CSceneNode* CSceneNode::GetParentNode(void) const {

	return m_Parent_p;
} // GetParentNode()


void CSceneNode::SetParentNode(CSceneNode * newParent_p) {

	if(m_Parent_p != NULL) {
		m_Parent_p->RemoveChildNode(this);
	}

	m_Parent_p = newParent_p;
} // SetParentNode()


void CSceneNode::AddChildNode(CSceneNode* childNode_p) {

	if(childNode_p != NULL) {
		if(childNode_p->GetParentNode() != NULL) {
			childNode_p->SetParentNode(this);
		}
		m_Children.push_back(childNode_p);
	}
} // AddChildNode()


void CSceneNode::RemoveChildNode(CSceneNode* childNode_p) {

	if(childNode_p != NULL && !m_Children.empty()) {

		size_t numChildren = m_Children.size();
		for(size_t i = 0; i < numChildren; i++) {
			if(m_Children[i] == childNode_p) {
				m_Children.erase(m_Children.begin() + i);
				break; // break the for loop
			}
		}
	}
} // RemoveChildNode()


const char* CSceneNode::GetNodeName(void) const {

	return m_Name_p;
} // GetNodeName()


CSceneNode* CSceneNode::GetChildNodeByName(const char *searchName) {
 CSceneNode* retNode = NULL;
 size_t numChildren;

	if(!m_Children.empty()) {

		numChildren = m_Children.size();
		for(size_t i = 0; i < numChildren; i++) {

			if(strcmp(m_Children[i]->m_Name_p, searchName) == 0) {
				retNode = m_Children[i];
				break;					// break the for loop
			}
		}
	}

	return retNode;
} // end of GetChildNodeByName()

