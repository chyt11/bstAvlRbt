
#include <iostream>
#include <stdio.h>
//#include <time.h>
#include <cstdlib>
#include <map>
#include "rbt.h"


Rbt::Rbt()
{
}

Rbt::~Rbt()
{
}

void Rbt::deleteTree(S_RbNode* pRoot)
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
S_RbNode* Rbt::insertNode(const S_Data& sData, S_RbNode* pRoot)
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
	pNode->parent = pParent;
	pNode->value  = sData;
	pNode->colour = RED;
//	bool insertToLChild = false;

	/* 1、如果根节点为空，插入黑点 */
	if(NULL == pParent)
	{
		pNode->colour = BLACK;
		return pNode;
	}
	else
	{
		if(sData.id < pParent->value.id)
		{
		//	insertToLChild = true;
			pParent->lChild = pNode;
		}
		else
		{
		//	insertToLChild = false;
			pParent->rChild = pNode;
		}
	}
	/* 非根结点，插入的平衡调整 */
	return insertNodeRebalance(pNode, pRoot);

}

/* 返回的是根结点 */
/* 先删除再调整 */
S_RbNode* Rbt::deleteNode(const int id, S_RbNode* pRoot)
{
	S_RbNode* pNode = pRoot;
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
	if(NULL == pNode)	return pRoot;

	S_RbNode* pDeleteNode    = pNode;
	S_RbNode* pSubstitueNode = NULL;				/* 被删节点的替补结点 S */
	S_RbNode* pXNode         = NULL;				/* 需调整的结点X */
	S_RbNode* pXparentNode   = pDeleteNode->parent;	/* 需调整的结点X的父结点 */

	int deleteColour = RED;
	if(NULL == pDeleteNode->rChild)
	{
		pSubstitueNode = pDeleteNode->lChild;
		pXNode = pSubstitueNode;
		deleteColour = pDeleteNode->colour;
	}
	else if(NULL == pDeleteNode->lChild)
	{
		pSubstitueNode = pDeleteNode->rChild;
		pXNode = pSubstitueNode;
		deleteColour = pDeleteNode->colour;
	}
	else
	{
		pSubstitueNode = pDeleteNode->lChild;
		if(NULL != pSubstitueNode->rChild)	/* 不进入这个条件找的不是前趋，是一个最方面调整的结点 */
		{
			do								/* 找前趋 */
			{
				pSubstitueNode = pSubstitueNode->rChild;
			}while (NULL != pSubstitueNode->rChild);
			pXNode = pSubstitueNode->lChild;				/* 调整的结点X为S的左子树 */
			pXparentNode = pSubstitueNode->parent;			/* 调整的结点的父结点xParent依然为S的原来父结点 */
			pSubstitueNode->parent->rChild = pSubstitueNode->lChild;
			if(NULL != pSubstitueNode->lChild)	pSubstitueNode->lChild->parent = pSubstitueNode->parent;

			pSubstitueNode->lChild = pDeleteNode->lChild;
			pDeleteNode->lChild->parent = pSubstitueNode;	/* 这里 1 把S的左子树放到S原来的位置，2 把S放到D的位置 */
		}
		else
		{
			pXNode = pSubstitueNode->lChild;				/* 调整的结点X为S的左子树 */
			pXparentNode = pSubstitueNode;					/* 调整的结点的父结点xParent依然为其原来父结点 */
		}

		deleteColour = pSubstitueNode->colour;				/* 被删掉的颜色为替换结点S原来的颜色 */
		pSubstitueNode->rChild = pDeleteNode->rChild;
		pDeleteNode->rChild->parent = pSubstitueNode;		/* 把原来D的右子树接到S的右子树位置 */
		pSubstitueNode->colour = pDeleteNode->colour;		/* S颜色替换为删除结点D的颜色 */
	}
	if(NULL != pSubstitueNode)
	{
		pSubstitueNode->parent = pDeleteNode->parent;		/* S的父节点更新 */
	}

	if(NULL == pDeleteNode->parent)		/* root节点 */
	{
		pRoot = pSubstitueNode;
	}
	else
	{
		if(pDeleteNode == pDeleteNode->parent->lChild)		/* D的父节点的子节点与S衔接 */
			pDeleteNode->parent->lChild = pSubstitueNode;
		else pDeleteNode->parent->rChild = pSubstitueNode;
	}

	/* 1、如果Y指向的节点是个红色节点，那么直接删除掉Y以后，红黑性质不会被破坏。操作结束。*/
	if(BLACK == deleteColour)	/* 指向是黑结点才要调整 */
	{
		pRoot = deleteNodeRebalance(pXNode, pXparentNode, pRoot);
//		printf("~~~~ deleteNodeRebalance pXNode(%p), finish\n", pXNode); fflush(stdout);
	}

	delete pDeleteNode;
	return pRoot;
}

/* 返回的是查找到的结点 */
S_RbNode* Rbt::searchNode(const int id, const S_RbNode* pRoot)
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

void Rbt::test(void)
{

}

void Rbt::printTree(S_RbNode* pRoot, int order)
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

int Rbt::getHeight(S_RbNode* pRoot)
{
	if(NULL == pRoot)	return 0;

	int lHeight = getHeight(pRoot->lChild) + 1;
	int rHeight = getHeight(pRoot->rChild) + 1;

	return (lHeight > rHeight ? lHeight : rHeight);
}

/*
 * 1、根节点
直接把此结点涂为黑色

1、黑父
不用做任何调整

2、红父
2.1 红叔	（黑曾祖）
当叔父结点为红色时，如下图所示，无需进行旋转操作，只要将父和叔结点变为黑色，将祖父结点变为红色即可。但由于祖父结点的父结点有可能为红色，从而违反红黑树性质。此时必须将祖父结点作为新的判定点继续向上（迭代）进行平衡操作。（注意这里是需要迭代的，有可能会调整到根节点）（其实这种情况包括4种，只需调整颜色，不需要旋转树形）

2.2 黑叔
当叔父结点为黑色时，需要进行旋转，也需要改颜色。
 */
S_RbNode* Rbt::insertNodeRebalance(S_RbNode* pNode, S_RbNode* pRoot)
{
	S_RbNode* pParent = pNode->parent;
	/* 2、黑父	不用任何调整 */

	/* 2、红父	分情况 */
	S_RbNode* pGrandparent = NULL;			/* 祖父结点 */
	S_RbNode* pUncle       = NULL;			/* 叔叔结点 */
	S_RbNode* pTmpNode     = NULL;			/* 交换用的临时结点 */
	while((NULL != (pParent = pNode->parent)) && (RED == pParent->colour))
	{										/* 根结点不为红，所以红父必有其非空双亲结点 */
		pGrandparent = pParent->parent;
		if(pParent == pGrandparent->lChild)		/* 当父结点为祖父结点的左孩子时 */
		{
			pUncle = pGrandparent->rChild;
			if((NULL != pUncle) && (RED == pUncle->colour))
												/* 2.1、红叔： 将父和叔结点涂黑，将祖父结点涂红 */
			{									/* ，当前结点指向祖父结点，往上迭代 */
				pParent->colour      = BLACK;
				pUncle->colour       = BLACK;
				pGrandparent->colour = RED;
				pNode = pGrandparent;
			}
			else								/* 2.2、黑叔：需要旋转和改颜色*/
			{
				if(pNode == pParent->rChild)	/* 当前结点为其父结点的右孩子时 */
				{
					/* 左旋*/
					leftRotate(pParent, &pRoot);
					pTmpNode = pParent;
					pParent  = pNode;
					pNode    = pTmpNode;
				}
				pParent->colour      = BLACK;
				pGrandparent->colour = RED;
				/* 右旋*/
				rightRotate(pGrandparent, &pRoot);
			}
		}
		else
		{
			pUncle = pGrandparent->lChild;
			if((NULL != pUncle) && (RED == pUncle->colour))
												/* 2.1、红叔： 将父和叔结点涂黑，将祖父结点涂红 */
			{									/* ，当前结点指向祖父结点，往上迭代 */
				pParent->colour      = BLACK;
				pUncle->colour       = BLACK;
				pGrandparent->colour = RED;
				pNode = pGrandparent;
			}
			else								/* 2.2、黑叔：需要旋转和改颜色 */
			{
				if(pNode == pParent->lChild)	/* 当前结点为其父结点的右孩子时 */
				{
					/* 右旋*/
					rightRotate(pParent, &pRoot);
					pTmpNode = pParent;
					pParent  = pNode;
					pNode    = pTmpNode;
				}
				pParent->colour      = BLACK;
				pGrandparent->colour = RED;
				/* 左旋 */
				leftRotate(pGrandparent, &pRoot);
			}
		}
	}

	pRoot->colour = BLACK;			/* 根结点无论如何都将之涂黑 */
	return pRoot;
}

/* 以X为其父的左孩子为例
 * 2、如果Y指向的节点是个黑色节点
2.1：X的兄弟W是红色（想办法将其变为黑色）
 （由于W是红色的，因此其儿子节点和父节点必为黑色，）只要将W和其父节点的颜色对换，
    再对父节点进行一次左旋转，便将W的左子节点放到了X的兄弟节点上，X的兄弟节点变成了黑色，且红黑性质不变。
    但还不算完，只是暂时将情况1转变成了下面的情况2或3或4。

2.2、X的兄弟节点W是黑色的，而且W的两个子节点都是黑色的。
此时可以将X的一重黑色和W的黑色同时去掉，而转加给他们的父节点上，这是X就指向它的父节点了，
因此此时父节点具有双重颜色了。这一重黑色节点上移。把w的黑给去掉，成为红色。
2.2.1、如果父节点原来是红色的，现在又加一层黑色，那么X现在指向的这个节点就是红黑两色的，直接把X（也就是父节点）着为黑色。问题就已经完整解决了。
2.2.2、如果父节点现在是双层黑色，那就以父节点为新的X进行向上的下一次的递归。

2.3、X的兄弟节点W是黑色的，而且W的左子节点是红色的，右子节点是黑色的。
此时通过交换W和其左子节点的颜色并进行一次向右旋转就可转换成下面的第四种情况。注意，原来L是红色的，所以L的子节点一定是黑色的，
    所以旋转中L节点的一个子树挂到之后着为红色的W节点上不会破坏红黑性质。变形后黑色高度不变。然后转向2.4处理。

2.4、X的兄弟节点W是黑色的，而且W的右子节点是红色的。
这种情况下，对父子树做一次左旋，W就处于根的位置，将W保持为原来的根的位置的颜色，同时将W的两个新的儿子节点的颜色变为黑色，去掉X的一重黑色。
这样整个问题也就得到了解决。递归结束。（在代码上，为了标识递归结束，我们把X指向根节点）
*/
S_RbNode* Rbt::deleteNodeRebalance(S_RbNode* pXNode, S_RbNode* pParentNode, S_RbNode* pRoot)
{
//	printf("~~~~ deleteNodeRebalance pXNode(%p), pParentNode(%p), pRoot(%p)\n",
//			pXNode, pParentNode, pRoot); fflush(stdout);
	/* 被删位置结点为黑色，X为空且X的兄弟W也为空的情况不存在 */
	S_RbNode* pWNode = NULL;
	while((pXNode != pRoot) && ((NULL == pXNode) || (BLACK == pXNode->colour)))
	{
		if(pXNode == pParentNode->lChild)			/* X是其父的左孩子 */
		{
			pWNode = pParentNode->rChild;
			if(RED == pWNode->colour)				/* 2.1 */
			{
				pWNode->colour = BLACK;
				pParentNode->colour = RED;
				leftRotate(pParentNode, &pRoot);
				pWNode = pParentNode->rChild;		/* 根据情况跳转至2.2~2.4 */
			}

			if(((NULL == pWNode->lChild) ||(BLACK == pWNode->lChild->colour))
					&&((NULL == pWNode->rChild) ||(BLACK == pWNode->rChild->colour)))
			{										/* 2.2 */
				pWNode->colour = RED;
				pXNode = pParentNode;
				pParentNode = pParentNode->parent;
			}
			else
			{
				if((NULL == pWNode->rChild) ||(BLACK == pWNode->rChild->colour))
				{									/* 2.3 */
					pWNode->colour = RED;
					pWNode->lChild->colour = BLACK;
					rightRotate(pWNode, &pRoot);
					pWNode = pParentNode->rChild;	/* 转至2.4 */
				}
													/* 2.4 */
				pWNode->colour = pParentNode->colour;
				pParentNode->colour = BLACK;
				pWNode->rChild->colour = BLACK;
				leftRotate(pParentNode, &pRoot);
				pXNode = pRoot;						/* 标识循环结束 */
			}
		}
		else										/* X是其父的右孩子 */
		{
			pWNode = pParentNode->lChild;
			if(RED == pWNode->colour)				/* 2.1 */
			{
				pWNode->colour = BLACK;
				pParentNode->colour = RED;
				rightRotate(pParentNode, &pRoot);
				pWNode = pParentNode->lChild;		/* 根据情况跳转至2.2~2.4 */
			}

			if(((NULL == pWNode->lChild) ||(BLACK == pWNode->lChild->colour))
					&& ((NULL == pWNode->rChild) ||(BLACK == pWNode->rChild->colour)))
			{										/* 2.2 */
				pWNode->colour = RED;
				pXNode = pParentNode;
				pParentNode = pParentNode->parent;
			}
			else
			{
				if((NULL == pWNode->lChild) ||(BLACK == pWNode->lChild->colour))
				{									/* 2.3 */
					pWNode->colour = RED;
					pWNode->rChild->colour = BLACK;
					leftRotate(pWNode, &pRoot);
					pWNode = pParentNode->lChild;	/* 转至2.4 */
				}
													/* 2.4 */
				pWNode->colour = pParentNode->colour;
				pParentNode->colour = BLACK;
				pWNode->lChild->colour = BLACK;
				rightRotate(pParentNode, &pRoot);
				pXNode = pRoot;						/* 标识循环结束 */
			}
		}
	}
	if(NULL != pXNode)	pXNode->colour = BLACK;

	return pRoot;
}
/*   A             B
 *       B  >  A
 *     x        x             */
void Rbt::leftRotate(S_RbNode* pNodeA, S_RbNode** ppRoot)
{
	S_RbNode* pNodeB = pNodeA->rChild;

	/* x成为A的右子树 */
	pNodeA->rChild = pNodeB->lChild;
	if(NULL != pNodeB->lChild)
		pNodeB->lChild->parent = pNodeA;
	/* B和A的父相连 */
	pNodeB->parent = pNodeA->parent;
	if(NULL != pNodeA->parent)
	{
		if(pNodeA == pNodeA->parent->lChild)
			pNodeA->parent->lChild = pNodeB;
		else pNodeA->parent->rChild = pNodeB;
	}
	else
	{
		*ppRoot = pNodeB;
	}
	/* A成为B的左子树 */
	pNodeB->lChild = pNodeA;
	pNodeA->parent = pNodeB;
//	return pRoot;
}

void Rbt::rightRotate(S_RbNode* pNodeA, S_RbNode** ppRoot)
{
	S_RbNode* pNodeB = pNodeA->lChild;

	/* x成为A的左子树 */
	pNodeA->lChild = pNodeB->rChild;
	if(NULL != pNodeB->rChild)
		pNodeB->rChild->parent = pNodeA;
	/* B和A的父相连 */
	pNodeB->parent = pNodeA->parent;
	if(NULL != pNodeA->parent)
	{
		if(pNodeA == pNodeA->parent->rChild)
			pNodeA->parent->rChild = pNodeB;
		else pNodeA->parent->lChild = pNodeB;
	}
	else
	{
		*ppRoot = pNodeB;
	}
	/* A成为B的右子树 */
	pNodeB->rChild = pNodeA;
	pNodeA->parent = pNodeB;
//	return pRoot;
}

/* end of file */
