
#ifndef RBT_H_
#define RBT_H_

//#include <string.h>
#include "commonDef.h"

//#define BLACK	0
//#define RED		1
//
//typedef struct _S_Data
//{
//	int  id;
//	int  value;
//} S_Data;
//
//typedef struct _S_RbNode
//{
//	struct _S_RbNode*	parent;
//	struct _S_RbNode*	lChild;
//	struct _S_RbNode*	rChild;
//	S_Data				value;
//	int 				colour;
//	_S_RbNode()
//	{
//		memset(this, 0, sizeof(_S_RbNode));
//	}
//} S_RbNode;


class Rbt {
public:
	Rbt();
	virtual ~Rbt();
	void deleteTree(S_RbNode* pRoot);

	/* ������ */
	S_RbNode* insertNode(const S_Data& sData, S_RbNode* pRoot);
	/* ɾ����� */
	S_RbNode* deleteNode(const int id, S_RbNode* pRoot);
	/* ���� */
	S_RbNode* searchNode(const int id, const S_RbNode* pRoot);

	void test(void);

	void printTree(S_RbNode* pRoot, int order = 0);
	int  getHeight(S_RbNode* pRoot);

private:
	/* ����ĵ��� */
	S_RbNode* insertNodeRebalance(S_RbNode* pNode, S_RbNode* pRoot);
	/* ɾ���ĵ��� */
	S_RbNode* deleteNodeRebalance(S_RbNode* pXNode, S_RbNode* pParentNode, S_RbNode* pRoot);

	/* ���� */
	void leftRotate(S_RbNode* pNodeA, S_RbNode** ppRoot);
	/* ���� */
	void rightRotate(S_RbNode* pNodeA, S_RbNode** ppRoot);


};

#endif /* RBT_H_ */

/* end of file */
