
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

/* 返回的是根结点 */
/* 先插入再调整 */
S_RbNode* Avl::insertNode(const S_Data& sData, S_RbNode* pRoot)
{
	insertAvl(sData, &pRoot);
	return pRoot;
}

/* 返回的是根结点 */
/* 先删除再调整 */
S_RbNode* Avl::deleteNode(const int id, S_RbNode* pRoot)
{
	deleteAvl(id, &pRoot);
	return pRoot;
}

/* 返回的是查找到的结点 */
S_RbNode* Avl::searchNode(const int id, const S_RbNode* pRoot)
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

void Avl::test(void)
{

}

void Avl::printTree(S_RbNode* pRoot, int order)
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

	/* x成为A的右子树 */
	(*ppNodeA)->rChild = pNodeB->lChild;

	/* A成为B的左子树 */
	pNodeB->lChild = (*ppNodeA);
	/* B成为新的子树跟结点，（函数完成后，调用者使用） */
	(*ppNodeA) = pNodeB;
}

void Avl::rightRotate(S_RbNode** ppNodeA)
{
	S_RbNode* pNodeB = (*ppNodeA)->lChild;

	/* x成为A的左子树 */
	(*ppNodeA)->lChild = pNodeB->rChild;

	/* A成为B的右子树 */
	pNodeB->rChild = (*ppNodeA);
	/* B成为新的子树跟结点 （函数完成后，调用者使用） */
	(*ppNodeA) = pNodeB;
}


void Avl::leftBalance(S_RbNode** ppNode)
{
	S_RbNode* pChildNode = (*ppNode)->lChild;
	switch(pChildNode->colour)
	{
		case BF_LH:	/* 左子树也是左重，单右旋可以了 */
			(*ppNode)->colour = pChildNode->colour = BF_EH;
			rightRotate(ppNode);

			break;
		case BF_RH:	/* 左子树是右重，子树左旋然后，当前树右旋 */
		{
			S_RbNode* pGrandChildNode = pChildNode->rChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* 左重 */
					(*ppNode)->colour = BF_RH;
					pChildNode->colour = BF_EH;
					break;
				case BF_EH:	/* 平衡，新树的两个子树都平衡 */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* 右重 */
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
		case BF_LH:	/* 右子树是左重，子树右旋然后，当前树左旋 */
		{
			S_RbNode* pGrandChildNode = pChildNode->lChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* 左重 */
					(*ppNode)->colour = BF_EH;
					pChildNode->colour = BF_RH;
					break;
				case BF_EH:	/* 平衡，新树的两个子树都平衡 */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* 右重 */
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
		case BF_RH:	/* 右子树是右重， 单左旋可以了*/
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
		case BF_LH:	/* 左子树也是左重，单右旋可以了 */
			(*ppNode)->colour = BF_EH;
		case BF_EH:	/* 左子树平衡，单右旋可以了 */
		//	(*ppNode)->colour = BF_LH;
			pChildNode->colour--;
			rightRotate(ppNode);

			break;
		case BF_RH:	/* 左子树是右重，子树左旋然后，当前树右旋 */
		{
			S_RbNode* pGrandChildNode = pChildNode->rChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* 左重 */
					(*ppNode)->colour = BF_RH;
					pChildNode->colour = BF_EH;
					break;
				case BF_EH:	/* 平衡，新树的两个子树都平衡 */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* 右重 */
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
		case BF_LH:	/* 右子树是左重，子树右旋然后，当前树左旋 */
		{
			S_RbNode* pGrandChildNode = pChildNode->lChild;
			switch(pGrandChildNode->colour)
			{
				case BF_LH:	/* 左重 */
					(*ppNode)->colour = BF_EH;
					pChildNode->colour = BF_RH;
					break;
				case BF_EH:	/* 平衡，新树的两个子树都平衡 */
					(*ppNode)->colour = pChildNode->colour = BF_EH;
					break;
				case BF_RH:	/* 右重 */
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
		case BF_RH:	/* 右子树是右重， 单左旋可以了*/
			(*ppNode)->colour = BF_EH;
		//	pChildNode->colour = BF_LH;
		//	leftRotate(ppNode);
		//	break;
		case BF_EH:	/* 右子树平衡，单左旋可以了 */
		//	(*ppNode)->colour = BF_RH;
			pChildNode->colour++;
			leftRotate(ppNode);
			break;
		default:	LOG_DEBUG("childNode bf(%d) ERR\n", pChildNode->colour); break;
	}
}

/* 返回值表示子树是否长高 */
int Avl::insertAvl(const S_Data& sData, S_RbNode** ppNode)
{
	int ret = 0;
	/* ppNode指针的正确性不校验，只管*ppNode指针 */
	if(NULL == *ppNode)	/* 结点为空则可以添加了 */
	{
		*ppNode = new S_RbNode;
		(*ppNode)->colour = BF_EH;
		(*ppNode)->value  = sData;
		/* 子节点指针不赋值了，结构体构造函数已经清零了 */
		return 1;
	}

	if(sData.id < (*ppNode)->value.id)	// lchild
	{
		if(0 < insertAvl(sData, &((*ppNode)->lChild)))
		{
			/* 根据当前平衡因子决定是否调整 */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* 原本就是左重，需要调整 */
					leftBalance(ppNode);
					ret = 0;
					break;
				case BF_EH:	/* 原本平衡，则BF+1 */
					(*ppNode)->colour = BF_LH;
					ret = 1;
					break;
				case BF_RH:	/* 原本就是右重，变平衡了 */
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
			/* 根据当前平衡因子决定是否调整 */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* 原本就是左重，变平衡了 */
					(*ppNode)->colour = BF_EH;
					ret = 0;
					break;
				case BF_EH:	/* 原本平衡，则BF-1 */
					(*ppNode)->colour = BF_RH;
					ret = 1;
					break;
				case BF_RH:	/* 原本就是右重，需要调整 */
					rightBalance(ppNode);
					ret = 0;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
		}
	}
	else	//	(sData.id == (*ppNode)->value.id)
	{
		(*ppNode)->value  = sData;	/* 相等时候覆盖当前内容 */
		return 0;
	}

	return ret;
}

int Avl::deleteAvl(const int id, S_RbNode** ppNode)
{
	int ret = 0;
	if(NULL == *ppNode)	/* 没有找到结点则树没有任何改变 */
	{
		return 0;
	}

//	LOG_DEBUG(" *ppNode id(%d)\n", (*ppNode)->value.id); fflush(stdout);
	if(id < (*ppNode)->value.id)		// lchild
	{
		/*  */
		if(0 > deleteAvl(id, &((*ppNode)->lChild)))
		{
			/* 根据当前平衡因子决定是否调整 */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* 原本就是左重，变平衡了 */
					(*ppNode)->colour = BF_EH;
					ret = -1;
					break;
				case BF_EH:	/* 原本平衡，则BF-1 */
					(*ppNode)->colour = BF_RH;
					ret = 0;
					break;
				case BF_RH:	/* 原本就是右重，需要调整 */
					rightBalanceDelete(ppNode);
					if(BF_EH == (*ppNode)->colour) ret = -1;	/* 如果变平衡了，说明树矮了，否则，树还是那么高 */
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
			/* 根据当前平衡因子决定是否调整 */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* 原本就是左重，需要调整 */
					leftBalanceDelete(ppNode);
					if(BF_EH == (*ppNode)->colour) ret = -1;
					else ret = 0;
					break;
				case BF_EH:	/* 原本平衡，则BF+1 */
					(*ppNode)->colour = BF_LH;
					ret = 0;
					break;
				case BF_RH:	/* 原本就是右重，变平衡了 */
					(*ppNode)->colour = BF_EH;
					ret = -1;
					break;
				default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
			}
		}
	}
	else // ==
	{
		S_RbNode* pSubstitueNode = NULL;				/* 被删节点的替补结点 S */

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
			if(NULL != pSubstitueNode->rChild)	/* 不进入这个条件找的不是前趋，是一个最方面调整的结点 */
			{
				S_RbNode* pAjustParentNode = pSubstitueNode;	/* 调整位置的父结点（即结构上被删除的结点） */
				ret = deleteFindPreNode(&pSubstitueNode, &pAjustParentNode);
				pAjustParentNode->lChild = pSubstitueNode;		/* 把可能调整后D的左子树接到调整结点A的左子树位置 */
				pSubstitueNode           = pAjustParentNode;	/* 方便函数的调用，把S指向调整结点A */
			}
			else
			{
				ret = -1;
			}
			pSubstitueNode->rChild = (*ppNode)->rChild;	/* 把删除结点D的右子树接到S的右子树 */
			pSubstitueNode->colour = (*ppNode)->colour;
//			if(NULL == pSubstitueNode)
//			{
//				LOG_DEBUG(" ppNode(%p) ERR\n", pSubstitueNode); fflush(stdout);
//			}
			/* 如果左树变矮，根据当前平衡因子决定是否调整 */
			if(-1 == ret)
			{
				switch(pSubstitueNode->colour)
				{
					case BF_LH:	/* 原本就是左重，变平衡了 */
						pSubstitueNode->colour = BF_EH;
						ret = -1;
						break;
					case BF_EH:	/* 原本平衡，则BF-1 */
						pSubstitueNode->colour = BF_RH;
						ret = 0;
						break;
					case BF_RH:	/* 原本就是右重，需要调整 */
						rightBalanceDelete(&pSubstitueNode);
						if(BF_EH == pSubstitueNode->colour) ret = -1;
						else ret = 0;
						break;
					default:	LOG_DEBUG(" bf(%d) ERR\n", (*ppNode)->colour); break;
				}
			}
		}

		delete (*ppNode);
		(*ppNode) = pSubstitueNode;			/* 换成替代结点 */
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
	/* 为了效率，ppNode的有效性不检查，由调用者保证 */
	if(NULL != (*ppNode)->rChild)
	{
		if(0 > deleteFindPreNode(&((*ppNode)->rChild), ppPreNode))
		{
			int ret = 0;
			/* 根据当前平衡因子决定是否调整 */
			switch((*ppNode)->colour)
			{
				case BF_LH:	/* 原本就是左重，需要调整 */
					leftBalanceDelete(ppNode);
					if(BF_EH == (*ppNode)->colour) ret = -1;
					else ret = 0;
					break;
				case BF_EH:	/* 原本平衡，则BF+1 */
					(*ppNode)->colour = BF_LH;
					ret = 0;
					break;
				case BF_RH:	/* 原本就是右重，变平衡了 */
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
		*ppPreNode = (*ppNode);			/* 返回前驱结点 */
		*ppNode    = (*ppNode)->lChild;	/* 把前驱结点在结构上换为其左结点 */
		return -1;
	}

	return 0;
}
/* end of file */
