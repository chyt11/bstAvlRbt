
#ifndef _AVL_H_
#define _AVL_H_

//#include <string.h>
#include "commonDef.h"

/* 使用colour成员记录平衡因子， */
#define BF_LH	1
#define BF_EH	0
#define BF_RH	(-1)

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



class Avl {
public:
	Avl();
	virtual ~Avl();
	void deleteTree(S_RbNode* pRoot);

	/* 插入结点 */
	S_RbNode* insertNode(const S_Data& sData, S_RbNode* pRoot);
	/* 删除结点 */
	S_RbNode* deleteNode(const int id, S_RbNode* pRoot);
	/* 查找 */
	S_RbNode* searchNode(const int id, const S_RbNode* pRoot);


	void test(void);

	void printTree(S_RbNode* pRoot, int order = 0);
	int  getHeight(S_RbNode* pRoot);

private:
	/* 左旋 */
	void leftRotate(S_RbNode** ppNodeA);
	/* 右旋 */
	void rightRotate(S_RbNode** ppNodeA);

	void leftBalance(S_RbNode** ppNode);
	void rightBalance(S_RbNode** ppNode);
	void leftBalanceDelete(S_RbNode** ppNode);
	void rightBalanceDelete(S_RbNode** ppNode);

	/* AVL结点的插入和删除 */
	int  insertAvl(const S_Data& sData, S_RbNode** ppNode);
	int  deleteAvl(const int id, S_RbNode** ppNode);

	int  deleteFindPreNode(S_RbNode** ppNode, S_RbNode** ppPreNode);
//	S_RbNode* deleteFindPreNode(S_RbNode** ppNode, int& treeHeightChangeCount);
};

#endif /* _AVL_H_ */

/* end of file */
