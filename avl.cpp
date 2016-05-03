
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cstdlib>
#include <map>
#include "avl.h"


Avl::Avl()
{
}

Avl::~Avl()
{
}

void Avl::deleteTree(S_RbNode* pRoot)
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
S_RbNode* Avl::insertNode(const S_Data& sData, S_RbNode* pRoot)
{
	insertAvl(sData, &pRoot);
	return pRoot;
}

/* ���ص��Ǹ���� */
/* ��ɾ���ٵ��� */
S_RbNode* Avl::deleteNode(const int id, S_RbNode* pRoot)
{
	deleteAvl(id, &pRoot);
	return pRoot;
}

/* ���ص��ǲ��ҵ��Ľ�� */
S_RbNode* Avl::searchNode(const int id, const S_RbNode* pRoot)
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

void Avl::test(void)
{

}

void Avl::printTree(S_RbNode* pRoot, int order)
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

int Avl::getHeight(S_RbNode* pRoot)
{
	if(NULL == pRoot)	return 0;

	int lHeight = getHeight(pRoot->lChild) + 1;
	int rHeight = getHeight(pRoot->rChild) + 1;

	return (lHeight > rHeight ? lHeight : rHeight);
}

/*   A             B
 *       B  >  A
 *     x        x             */
void Avl::leftRotate(S_RbNode** ppNodeA)
{
	S_RbNode* pNodeB = (*ppNodeA)->rChild;

	/* x��ΪA�������� */
	(*ppNodeA)->rChild = pNodeB->lChild;

	/* A��ΪB�������� */
	pNodeB->lChild = (*ppNodeA);
	/* B��Ϊ�µ���������㣬��������ɺ󣬵�����ʹ�ã� */
	(*ppNodeA) = pNodeB;
}

void Avl::rightRotate(S_RbNode** ppNodeA)
{
	S_RbNode* pNodeB = (*ppNodeA)->lChild;

	/* x��ΪA�������� */
	(*ppNodeA)->lChild = pNodeB->rChild;

	/* A��ΪB�������� */
	pNodeB->rChild = (*ppNodeA);
	/* B��Ϊ�µ���������� ��������ɺ󣬵�����ʹ�ã� */
	(*ppNodeA) = pNodeB;
}


void Avl::leftBalance(S_RbNode** ppNode)
{
	S_RbNode* pChildNode = (*ppNode)->lChild;
	switch(pChildNode->colour)
	{
		case BF_LH:	/* ������Ҳ�����أ������������� */
			(*ppNode)->colour = pChildNode->colour = BF_EH;
			rightRotate(ppNode);

			break;
		case BF_RH:	/* �����������أ���������Ȼ�󣬵�ǰ������ */
		{
			S_RbNode* pGrandChildNode = pChildNode->rChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* ���� */
					(*ppNode)->colour = BF_RH;
					pChildNode->colour = BF_EH;
					break;
				case BF_EH:	/* ƽ�⣬����������������ƽ�� */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* ���� */
					(*ppNode)->colour = BF_EH;
					pChildNode->colour = BF_LH;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
			pGrandChildNode->colour = BF_EH;
			leftRotate(&((*ppNode)->lChild));
			rightRotate(ppNode);

			break;
		}
		default:	LOG_DEBUG("childNode bf(%d) ERR\n", pChildNode->colour); break;
	}
}

void Avl::rightBalance(S_RbNode** ppNode)
{
	S_RbNode* pChildNode = (*ppNode)->rChild;
	switch(pChildNode->colour)
	{
		case BF_LH:	/* �����������أ���������Ȼ�󣬵�ǰ������ */
		{
			S_RbNode* pGrandChildNode = pChildNode->lChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* ���� */
					(*ppNode)->colour = BF_EH;
					pChildNode->colour = BF_RH;
					break;
				case BF_EH:	/* ƽ�⣬����������������ƽ�� */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* ���� */
					(*ppNode)->colour = BF_LH;
					pChildNode->colour = BF_EH;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
			pGrandChildNode->colour = BF_EH;
			rightRotate(&((*ppNode)->rChild));
			leftRotate(ppNode);
			break;
		}
		case BF_RH:	/* �����������أ� ������������*/
			(*ppNode)->colour = pChildNode->colour = BF_EH;
			leftRotate(ppNode);
			break;
		default:	LOG_DEBUG("childNode bf(%d) ERR\n", pChildNode->colour); break;
	}
}

void Avl::leftBalanceDelete(S_RbNode** ppNode)
{
	S_RbNode* pChildNode = (*ppNode)->lChild;
	switch(pChildNode->colour)
	{
		case BF_LH:	/* ������Ҳ�����أ������������� */
			(*ppNode)->colour = BF_EH;
		case BF_EH:	/* ������ƽ�⣬������������ */
		//	(*ppNode)->colour = BF_LH;
			pChildNode->colour--;
			rightRotate(ppNode);

			break;
		case BF_RH:	/* �����������أ���������Ȼ�󣬵�ǰ������ */
		{
			S_RbNode* pGrandChildNode = pChildNode->rChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* ���� */
					(*ppNode)->colour = BF_RH;
					pChildNode->colour = BF_EH;
					break;
				case BF_EH:	/* ƽ�⣬����������������ƽ�� */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* ���� */
					(*ppNode)->colour = BF_EH;
					pChildNode->colour = BF_LH;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
			pGrandChildNode->colour = BF_EH;
			leftRotate(&((*ppNode)->lChild));
			rightRotate(ppNode);
			break;
		}
		default:	LOG_DEBUG("childNode bf(%d) ERR\n", pChildNode->colour); break;
	}
}

void Avl::rightBalanceDelete(S_RbNode** ppNode)
{
	S_RbNode* pChildNode = (*ppNode)->rChild;
	switch(pChildNode->colour)
	{
		case BF_LH:	/* �����������أ���������Ȼ�󣬵�ǰ������ */
		{
			S_RbNode* pGrandChildNode = pChildNode->lChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* ���� */
					(*ppNode)->colour = BF_EH;
					pChildNode->colour = BF_RH;
					break;
				case BF_EH:	/* ƽ�⣬����������������ƽ�� */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* ���� */
					(*ppNode)->colour = BF_LH;
					pChildNode->colour = BF_EH;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
			pGrandChildNode->colour = BF_EH;
			rightRotate(&((*ppNode)->rChild));
			leftRotate(ppNode);
			break;
		}
		case BF_RH:	/* �����������أ� ������������*/
			(*ppNode)->colour = BF_EH;
		//	pChildNode->colour = BF_LH;
		//	leftRotate(ppNode);
		//	break;
		case BF_EH:	/* ������ƽ�⣬������������ */
		//	(*ppNode)->colour = BF_RH;
			pChildNode->colour++;
			leftRotate(ppNode);
			break;
		default:	LOG_DEBUG("childNode bf(%d) ERR\n", pChildNode->colour); break;
	}
}

/* ����ֵ��ʾ�����Ƿ񳤸� */
int Avl::insertAvl(const S_Data& sData, S_RbNode** ppNode)
{
	int ret = 0;
	/* ppNodeָ�����ȷ�Բ�У�飬ֻ��*ppNodeָ�� */
	if(NULL == *ppNode)	/* ���Ϊ������������ */
	{
		*ppNode = new S_RbNode;
		(*ppNode)->colour = BF_EH;
		(*ppNode)->value  = sData;
		/* �ӽڵ�ָ�벻��ֵ�ˣ��ṹ�幹�캯���Ѿ������� */
		return 1;
	}

	if(sData.id < (*ppNode)->value.id)	// lchild
	{
		if(0 < insertAvl(sData, &((*ppNode)->lChild)))
		{
			/* ���ݵ�ǰƽ�����Ӿ����Ƿ���� */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* ԭ���������أ���Ҫ���� */
					leftBalance(ppNode);
					ret = 0;
					break;
				case BF_EH:	/* ԭ��ƽ�⣬��BF+1 */
					(*ppNode)->colour = BF_LH;
					ret = 1;
					break;
				case BF_RH:	/* ԭ���������أ���ƽ���� */
					(*ppNode)->colour = BF_EH;
					ret = 0;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
		}
	}
	else if (sData.id > (*ppNode)->value.id)	// rchild
	{
		if(0 < insertAvl(sData, &((*ppNode)->rChild)))
		{
			/* ���ݵ�ǰƽ�����Ӿ����Ƿ���� */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* ԭ���������أ���ƽ���� */
					(*ppNode)->colour = BF_EH;
					ret = 0;
					break;
				case BF_EH:	/* ԭ��ƽ�⣬��BF-1 */
					(*ppNode)->colour = BF_RH;
					ret = 1;
					break;
				case BF_RH:	/* ԭ���������أ���Ҫ���� */
					rightBalance(ppNode);
					ret = 0;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
		}
	}
	else	//	(sData.id == (*ppNode)->value.id)
	{
		(*ppNode)->value  = sData;	/* ���ʱ�򸲸ǵ�ǰ���� */
		return 0;
	}

	return ret;
}

int Avl::deleteAvl(const int id, S_RbNode** ppNode)
{
	int ret = 0;
	if(NULL == *ppNode)	/* û���ҵ��������û���κθı� */
	{
		return 0;
	}

//	LOG_DEBUG(" *ppNode id(%d)\n", (*ppNode)->value.id); fflush(stdout);
	if(id < (*ppNode)->value.id)		// lchild
	{
		/*  */
		if(0 > deleteAvl(id, &((*ppNode)->lChild)))
		{
			/* ���ݵ�ǰƽ�����Ӿ����Ƿ���� */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* ԭ���������أ���ƽ���� */
					(*ppNode)->colour = BF_EH;
					ret = -1;
					break;
				case BF_EH:	/* ԭ��ƽ�⣬��BF-1 */
					(*ppNode)->colour = BF_RH;
					ret = 0;
					break;
				case BF_RH:	/* ԭ���������أ���Ҫ���� */
					rightBalanceDelete(ppNode);
					if(BF_EH == (*ppNode)->colour) ret = -1;	/* �����ƽ���ˣ�˵�������ˣ�������������ô�� */
					else ret = 0;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
		}
	}
	else if (id > (*ppNode)->value.id)	// rchild
	{
		if(0 > deleteAvl(id, &((*ppNode)->rChild)))
		{
			/* ���ݵ�ǰƽ�����Ӿ����Ƿ���� */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* ԭ���������أ���Ҫ���� */
					leftBalanceDelete(ppNode);
					if(BF_EH == (*ppNode)->colour) ret = -1;
					else ret = 0;
					break;
				case BF_EH:	/* ԭ��ƽ�⣬��BF+1 */
					(*ppNode)->colour = BF_LH;
					ret = 0;
					break;
				case BF_RH:	/* ԭ���������أ���ƽ���� */
					(*ppNode)->colour = BF_EH;
					ret = -1;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
		}
	}
	else // ==
	{
		S_RbNode* pSubstitueNode = NULL;				/* ��ɾ�ڵ���油��� S */

		if(NULL == (*ppNode)->rChild)
		{
			pSubstitueNode = (*ppNode)->lChild;
			ret = -1;
		}
		else if(NULL == (*ppNode)->lChild)
		{
			pSubstitueNode = (*ppNode)->rChild;
			ret = -1;
		}
		else
		{
			pSubstitueNode = (*ppNode)->lChild;
			if(NULL != pSubstitueNode->rChild)	/* ��������������ҵĲ���ǰ������һ���������Ľ�� */
			{
				S_RbNode* pAjustParentNode = pSubstitueNode;	/* ����λ�õĸ���㣨���ṹ�ϱ�ɾ���Ľ�㣩 */
				ret = deleteFindPreNode(&pSubstitueNode, &pAjustParentNode);
				pAjustParentNode->lChild = pSubstitueNode;		/* �ѿ��ܵ�����D���������ӵ��������A��������λ�� */
				pSubstitueNode           = pAjustParentNode;	/* ���㺯���ĵ��ã���Sָ��������A */
			}
			else
			{
				ret = -1;
			}
			pSubstitueNode->rChild = (*ppNode)->rChild;	/* ��ɾ�����D���������ӵ�S�������� */
			pSubstitueNode->colour = (*ppNode)->colour;
//			if(NULL == pSubstitueNode)
//			{
//				LOG_DEBUG(" ppNode(%p) ERR\n", pSubstitueNode); fflush(stdout);
//			}
			/* ��������䰫�����ݵ�ǰƽ�����Ӿ����Ƿ���� */
			if(-1 == ret)
			{
				switch(pSubstitueNode->colour)
				{
					case BF_LH:	/* ԭ���������أ���ƽ���� */
						pSubstitueNode->colour = BF_EH;
						ret = -1;
						break;
					case BF_EH:	/* ԭ��ƽ�⣬��BF-1 */
						pSubstitueNode->colour = BF_RH;
						ret = 0;
						break;
					case BF_RH:	/* ԭ���������أ���Ҫ���� */
						rightBalanceDelete(&pSubstitueNode);
						if(BF_EH == pSubstitueNode->colour) ret = -1;
						else ret = 0;
						break;
					default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
				}
			}
		}

		delete (*ppNode);
		(*ppNode) = pSubstitueNode;			/* ���������� */
	}

	return ret;
}

//S_RbNode* Avl::deleteFindPreNode(S_RbNode** ppNode, int& treeHeightChangeCount)
int Avl::deleteFindPreNode(S_RbNode** ppNode, S_RbNode** ppPreNode)
{
//	if(NULL == ppNode)
//	{
//		LOG_DEBUG(" ppNode(%p) ERR\n", ppNode); fflush(stdout);
//	}
	/* Ϊ��Ч�ʣ�ppNode����Ч�Բ���飬�ɵ����߱�֤ */
	if(NULL != (*ppNode)->rChild)
	{
		if(0 > deleteFindPreNode(&((*ppNode)->rChild), ppPreNode))
		{
			int ret = 0;
			/* ���ݵ�ǰƽ�����Ӿ����Ƿ���� */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* ԭ���������أ���Ҫ���� */
					leftBalanceDelete(ppNode);
					if(BF_EH == (*ppNode)->colour) ret = -1;
					else ret = 0;
					break;
				case BF_EH:	/* ԭ��ƽ�⣬��BF+1 */
					(*ppNode)->colour = BF_LH;
					ret = 0;
					break;
				case BF_RH:	/* ԭ���������أ���ƽ���� */
					(*ppNode)->colour = BF_EH;
					ret = -1;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
			return ret;
		}
	}
	else
	{
		*ppPreNode = (*ppNode);			/* ����ǰ����� */
		*ppNode    = (*ppNode)->lChild;	/* ��ǰ������ڽṹ�ϻ�Ϊ������ */
		return -1;
	}

	return 0;
}
/* end of file */
