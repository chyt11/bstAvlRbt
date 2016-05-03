
#include <iostream>
#include <stdio.h>
#include "bst.h"



#define LOG_DEBUG(format,...)  	printf("line %d : "format, __LINE__, ##__VA_ARGS__)

Bst::Bst() {
	// TODO Auto-generated constructor stub

}

Bst::~Bst() {
	// TODO Auto-generated destructor stub
}

void Bst::deleteTree(S_RbNode* pRoot)
{
	if(NULL != pRoot)
	{
		deleteTree(pRoot->lChild);
		deleteTree(pRoot->rChild);
		delete pRoot;
	}
}

/* 返回的是根结点 */
/* 先插入再调整 */
S_RbNode* Bst::insertNode(const S_Data& sData, S_RbNode* pRoot)
{
	/* 查找到插入点，查找到就更新 */
	S_RbNode* pParent = NULL;
	S_RbNode* pNode = pRoot;
	while(NULL != pNode)
	{
		if(sData.id < pNode->value.id)
		{
			pParent     = pNode;
			pNode = pNode->lChild;
		}
		else if (sData.id > pNode->value.id)
		{
			pParent     = pNode;
			pNode = pNode->rChild;
		}
		else	//	(sData.id == pInsertNode->value.id)
		{
			pNode->value = sData;
			return pRoot;
		}
	}
	/* 生成一个红结点，根据下列情况再作决定 */
	pNode = new S_RbNode();
//	pNode->parent = pParent;
	pNode->value  = sData;
//	pNode->colour = RED;

	/* 1、如果根节点为空，插入黑点 */
	if(NULL == pParent)
	{
//		pNode->colour = BLACK;
		return pNode;
	}
	else
	{
		if(sData.id < pParent->value.id)
		{
			pParent->lChild = pNode;
		}
		else
		{
			pParent->rChild = pNode;
		}
	}
	/* 非根结点，插入的平衡调整 */
	return pRoot;

}

/* 返回的是根结点 */
/* 先删除再调整 */
S_RbNode* Bst::deleteNode(const int id, S_RbNode* pRoot)
{
	S_RbNode* pDeleteNode = pRoot;
	S_RbNode* pParent     = NULL;
	while(NULL != pDeleteNode)
	{
		if(id < pDeleteNode->value.id)
		{
			pParent     = pDeleteNode;
			pDeleteNode = pDeleteNode->lChild;
		}
		else if (id > pDeleteNode->value.id)
		{
			pParent     = pDeleteNode;
			pDeleteNode = pDeleteNode->rChild;
		}
		else	//	(sData.id == pNode->value.id)
			break;
		//	return pNode;
	}
	if(NULL == pDeleteNode)	return pRoot;			/* 找不到的情况 */

	S_RbNode* pSubstitueNode = NULL;				/* 被删节点的替补结点 S */

	if(NULL == pDeleteNode->rChild)
	{
		pSubstitueNode = pDeleteNode->lChild;

	}
	else if(NULL == pDeleteNode->lChild)
	{
		pSubstitueNode = pDeleteNode->rChild;

	}
	else
	{
		S_RbNode* pSubstitueParent = pSubstitueNode;
		pSubstitueNode = pDeleteNode->lChild;
		if(NULL != pSubstitueNode->rChild)	/* 不进入这个条件找的不是前趋，是一个最方面调整的结点 */
		{
			do								/* 找前趋 */
			{
				pSubstitueParent = pSubstitueNode;
				pSubstitueNode = pSubstitueNode->rChild;
			}while (NULL != pSubstitueNode->rChild);
			pSubstitueParent->rChild = pSubstitueNode->lChild;
			pSubstitueNode->lChild = pDeleteNode->lChild;
		}
		pSubstitueNode->rChild = pDeleteNode->rChild;		/* 把原来D的右子树接到S的右子树位置 */

	}

	if(NULL == pParent)										/* root节点 */	//pDeleteNode->parent)
	{
		pRoot = pSubstitueNode;
	}
	else
	{
		if(pDeleteNode == pParent->lChild)		/* D的父节点的子节点与S衔接 */
			pParent->lChild = pSubstitueNode;
		else pParent->rChild = pSubstitueNode;
	}

	delete pDeleteNode;
	return pRoot;
}

/* 返回的是查找到的结点 */
S_RbNode* Bst::searchNode(const int id, const S_RbNode* pRoot)
{
	/* 查找到插入点，查找到就不插入 */
	S_RbNode* pNode = const_cast<S_RbNode*>(pRoot);
	while(NULL != pNode)
	{
		if(id < pNode->value.id)
			pNode = pNode->lChild;
		else if (id > pNode->value.id)
			pNode = pNode->rChild;
		else	//	(sData.id == pNode->value.id)
			break;
		//	return pNode;
	}
	return pNode;
}

void Bst::test(void)
{

}

void Bst::printTree(S_RbNode* pRoot, int order)
{
	if(NULL != pRoot)
	{
		switch(order)
		{
	//	case 0: 	/* 前序 */
	//		break;
		case 1: 	/* 中序 */
			printTree(pRoot->lChild, 1);
			printf("root(%p) ", pRoot);
			printf("id(%d), colour(%d) parent(%p), lChild(%p), rChild(%p);\n",
					pRoot->value.id, pRoot->colour, pRoot->parent, pRoot->lChild, pRoot->rChild);
			printTree(pRoot->rChild, 1);
			break;
		case 2: 	/* 后序 */
			printTree(pRoot->lChild, 2);
			printTree(pRoot->rChild, 2);
			printf("root(%p) ", pRoot);
			printf("id(%d), colour(%d) parent(%p), lChild(%p), rChild(%p);\n",
					pRoot->value.id, pRoot->colour, pRoot->parent, pRoot->lChild, pRoot->rChild);
			break;
		default:	/* 前序 */
			printf("root(%p) ", pRoot);
			printf("id(%d), colour(%d) parent(%p), lChild(%p), rChild(%p);\n",
					pRoot->value.id, pRoot->colour, pRoot->parent, pRoot->lChild, pRoot->rChild);
			printTree(pRoot->lChild);
			printTree(pRoot->rChild);
			break;
		}
	}
}

int Bst::getHeight(S_RbNode* pRoot)
{
	if(NULL == pRoot)	return 0;

	int lHeight = getHeight(pRoot->lChild) + 1;
	int rHeight = getHeight(pRoot->rChild) + 1;

	return (lHeight > rHeight ? lHeight : rHeight);
}

/* end of file */
