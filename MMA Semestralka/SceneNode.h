// Simple Scene Graph in C++ by E. Luten 2009

#ifndef __SCENENODE_H
#define __SCENENODE_H

#include <vector>
#include <cstring>


class CSceneNode {

public:

	CSceneNode(CSceneNode* parentNode_p = NULL, const char* nodeName_p = NULL);

	virtual ~CSceneNode(void);

	// this method goes through the children-vector and calls the Update method on each child that does not equal NULL. 
	virtual void Update(void);

	CSceneNode* GetParentNode(void) const;
	void SetParentNode(CSceneNode * newParent_p);

	void AddChildNode(CSceneNode* childNode_p);
	void RemoveChildNode(CSceneNode* childNode_p);

	const char* GetNodeName(void) const;

	CSceneNode* GetChildNodeByName(const char *searchName);

protected:
	// pointer to the parent node
	CSceneNode* m_Parent_p;

	// node name
	const char* m_Name_p;

	// vector of children
	std::vector<CSceneNode *> m_Children;
};

#endif // of __SCENENODE_H
