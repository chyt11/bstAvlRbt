
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

/* ���ص��Ǹ���� */
/* �Ȳ����ٵ��� */
S_RbNode* Bst::insertNode(const S_Data& sData, S_RbNode* pRoot)
{
	/* ���ҵ�����㣬���ҵ��͸��� */
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
	/* ����һ�����㣬������������������� */
	pNode = new S_RbNode();
//	pNode->parent = pParent;
	pNode->value  = sData;
//	pNode->colour = RED;

	/* 1��������ڵ�Ϊ�գ�����ڵ� */
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
	/* �Ǹ���㣬�����ƽ����� */
	return pRoot;

}

/* ���ص��Ǹ���� */
/* ��ɾ���ٵ��� */
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
	if(NULL == pDeleteNode)	return pRoot;			/* �Ҳ�������� */

	S_RbNode* pSubstitueNode = NULL;				/* ��ɾ�ڵ���油��� S */

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
		if(NULL != pSubstitueNode->rChild)	/* ��������������ҵĲ���ǰ������һ���������Ľ�� */
		{
			do								/* ��ǰ�� */
			{
				pSubstitueParent = pSubstitueNode;
				pSubstitueNode = pSubstitueNode->rChild;
			}while (NULL != pSubstitueNode->rChild);
			pSubstitueParent->rChild = pSubstitueNode->lChild;
			pSubstitueNode->lChild = pDeleteNode->lChild;
		}
		pSubstitueNode->rChild = pDeleteNode->rChild;		/* ��ԭ��D���������ӵ�S��������λ�� */

	}

	if(NULL == pParent)										/* root�ڵ� */	//pDeleteNode->parent)
	{
		pRoot = pSubstitueNode;
	}
	else
	{
		if(pDeleteNode == pParent->lChild)		/* D�ĸ��ڵ���ӽڵ���S�ν� */
			pParent->lChild = pSubstitueNode;
		else pParent->rChild = pSubstitueNode;
	}

	delete pDeleteNode;
	return pRoot;
}

/* ���ص��ǲ��ҵ��Ľ�� */
S_RbNode* Bst::searchNode(const int id, const S_RbNode* pRoot)
{
	/* ���ҵ�����㣬���ҵ��Ͳ����� */
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
	//	case 0: 	/* ǰ�� */
	//		break;
		case 1: 	/* ���� */
			printTree(pRoot->lChild, 1);
			printf("root(%p) ", pRoot);
			printf("id(%d), colour(%d) parent(%p), lChild(%p), rChild(%p);\n",
					pRoot->value.id, pRoot->colour, pRoot->parent, pRoot->lChild, pRoot->rChild);
			printTree(pRoot->rChild, 1);
			break;
		case 2: 	/* ���� */
			printTree(pRoot->lChild, 2);
			printTree(pRoot->rChild, 2);
			printf("root(%p) ", pRoot);
			printf("id(%d), colour(%d) parent(%p), lChild(%p), rChild(%p);\n",
					pRoot->value.id, pRoot->colour, pRoot->parent, pRoot->lChild, pRoot->rChild);
			break;
		default:	/* ǰ�� */
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
