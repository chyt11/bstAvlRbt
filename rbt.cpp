
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

/* ���ص��Ǹ���� */
/* �Ȳ����ٵ��� */
S_RbNode* Rbt::insertNode(const S_Data& sData, S_RbNode* pRoot)
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
	pNode->parent = pParent;
	pNode->value  = sData;
	pNode->colour = RED;
//	bool insertToLChild = false;

	/* 1��������ڵ�Ϊ�գ�����ڵ� */
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
	/* �Ǹ���㣬�����ƽ����� */
	return insertNodeRebalance(pNode, pRoot);

}

/* ���ص��Ǹ���� */
/* ��ɾ���ٵ��� */
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
	S_RbNode* pSubstitueNode = NULL;				/* ��ɾ�ڵ���油��� S */
	S_RbNode* pXNode         = NULL;				/* ������Ľ��X */
	S_RbNode* pXparentNode   = pDeleteNode->parent;	/* ������Ľ��X�ĸ���� */

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
		if(NULL != pSubstitueNode->rChild)	/* ��������������ҵĲ���ǰ������һ���������Ľ�� */
		{
			do								/* ��ǰ�� */
			{
				pSubstitueNode = pSubstitueNode->rChild;
			}while (NULL != pSubstitueNode->rChild);
			pXNode = pSubstitueNode->lChild;				/* �����Ľ��XΪS�������� */
			pXparentNode = pSubstitueNode->parent;			/* �����Ľ��ĸ����xParent��ȻΪS��ԭ������� */
			pSubstitueNode->parent->rChild = pSubstitueNode->lChild;
			if(NULL != pSubstitueNode->lChild)	pSubstitueNode->lChild->parent = pSubstitueNode->parent;

			pSubstitueNode->lChild = pDeleteNode->lChild;
			pDeleteNode->lChild->parent = pSubstitueNode;	/* ���� 1 ��S���������ŵ�Sԭ����λ�ã�2 ��S�ŵ�D��λ�� */
		}
		else
		{
			pXNode = pSubstitueNode->lChild;				/* �����Ľ��XΪS�������� */
			pXparentNode = pSubstitueNode;					/* �����Ľ��ĸ����xParent��ȻΪ��ԭ������� */
		}

		deleteColour = pSubstitueNode->colour;				/* ��ɾ������ɫΪ�滻���Sԭ������ɫ */
		pSubstitueNode->rChild = pDeleteNode->rChild;
		pDeleteNode->rChild->parent = pSubstitueNode;		/* ��ԭ��D���������ӵ�S��������λ�� */
		pSubstitueNode->colour = pDeleteNode->colour;		/* S��ɫ�滻Ϊɾ�����D����ɫ */
	}
	if(NULL != pSubstitueNode)
	{
		pSubstitueNode->parent = pDeleteNode->parent;		/* S�ĸ��ڵ���� */
	}

	if(NULL == pDeleteNode->parent)		/* root�ڵ� */
	{
		pRoot = pSubstitueNode;
	}
	else
	{
		if(pDeleteNode == pDeleteNode->parent->lChild)		/* D�ĸ��ڵ���ӽڵ���S�ν� */
			pDeleteNode->parent->lChild = pSubstitueNode;
		else pDeleteNode->parent->rChild = pSubstitueNode;
	}

	/* 1�����Yָ��Ľڵ��Ǹ���ɫ�ڵ㣬��ôֱ��ɾ����Y�Ժ󣬺�����ʲ��ᱻ�ƻ�������������*/
	if(BLACK == deleteColour)	/* ָ���Ǻڽ���Ҫ���� */
	{
		pRoot = deleteNodeRebalance(pXNode, pXparentNode, pRoot);
//		printf("~~~~ deleteNodeRebalance pXNode(%p), finish\n", pXNode); fflush(stdout);
	}

	delete pDeleteNode;
	return pRoot;
}

/* ���ص��ǲ��ҵ��Ľ�� */
S_RbNode* Rbt::searchNode(const int id, const S_RbNode* pRoot)
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

void Rbt::test(void)
{

}

void Rbt::printTree(S_RbNode* pRoot, int order)
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

int Rbt::getHeight(S_RbNode* pRoot)
{
	if(NULL == pRoot)	return 0;

	int lHeight = getHeight(pRoot->lChild) + 1;
	int rHeight = getHeight(pRoot->rChild) + 1;

	return (lHeight > rHeight ? lHeight : rHeight);
}

/*
 * 1�����ڵ�
ֱ�ӰѴ˽��ͿΪ��ɫ

1���ڸ�
�������κε���

2���츸
2.1 ����	�������棩
���常���Ϊ��ɫʱ������ͼ��ʾ�����������ת������ֻҪ�����������Ϊ��ɫ�����游����Ϊ��ɫ���ɡ��������游���ĸ�����п���Ϊ��ɫ���Ӷ�Υ����������ʡ���ʱ���뽫�游�����Ϊ�µ��ж���������ϣ�����������ƽ���������ע����������Ҫ�����ģ��п��ܻ���������ڵ㣩����ʵ�����������4�֣�ֻ�������ɫ������Ҫ��ת���Σ�

2.2 ����
���常���Ϊ��ɫʱ����Ҫ������ת��Ҳ��Ҫ����ɫ��
 */
S_RbNode* Rbt::insertNodeRebalance(S_RbNode* pNode, S_RbNode* pRoot)
{
	S_RbNode* pParent = pNode->parent;
	/* 2���ڸ�	�����κε��� */

	/* 2���츸	����� */
	S_RbNode* pGrandparent = NULL;			/* �游��� */
	S_RbNode* pUncle       = NULL;			/* ������ */
	S_RbNode* pTmpNode     = NULL;			/* �����õ���ʱ��� */
	while((NULL != (pParent = pNode->parent)) && (RED == pParent->colour))
	{										/* ����㲻Ϊ�죬���Ժ츸������ǿ�˫�׽�� */
		pGrandparent = pParent->parent;
		if(pParent == pGrandparent->lChild)		/* �������Ϊ�游��������ʱ */
		{
			pUncle = pGrandparent->rChild;
			if((NULL != pUncle) && (RED == pUncle->colour))
												/* 2.1�����壺 ����������Ϳ�ڣ����游���Ϳ�� */
			{									/* ����ǰ���ָ���游��㣬���ϵ��� */
				pParent->colour      = BLACK;
				pUncle->colour       = BLACK;
				pGrandparent->colour = RED;
				pNode = pGrandparent;
			}
			else								/* 2.2�����壺��Ҫ��ת�͸���ɫ*/
			{
				if(pNode == pParent->rChild)	/* ��ǰ���Ϊ�丸�����Һ���ʱ */
				{
					/* ����*/
					leftRotate(pParent, &pRoot);
					pTmpNode = pParent;
					pParent  = pNode;
					pNode    = pTmpNode;
				}
				pParent->colour      = BLACK;
				pGrandparent->colour = RED;
				/* ����*/
				rightRotate(pGrandparent, &pRoot);
			}
		}
		else
		{
			pUncle = pGrandparent->lChild;
			if((NULL != pUncle) && (RED == pUncle->colour))
												/* 2.1�����壺 ����������Ϳ�ڣ����游���Ϳ�� */
			{									/* ����ǰ���ָ���游��㣬���ϵ��� */
				pParent->colour      = BLACK;
				pUncle->colour       = BLACK;
				pGrandparent->colour = RED;
				pNode = pGrandparent;
			}
			else								/* 2.2�����壺��Ҫ��ת�͸���ɫ */
			{
				if(pNode == pParent->lChild)	/* ��ǰ���Ϊ�丸�����Һ���ʱ */
				{
					/* ����*/
					rightRotate(pParent, &pRoot);
					pTmpNode = pParent;
					pParent  = pNode;
					pNode    = pTmpNode;
				}
				pParent->colour      = BLACK;
				pGrandparent->colour = RED;
				/* ���� */
				leftRotate(pGrandparent, &pRoot);
			}
		}
	}

	pRoot->colour = BLACK;			/* �����������ζ���֮Ϳ�� */
	return pRoot;
}

/* ��XΪ�丸������Ϊ��
 * 2�����Yָ��Ľڵ��Ǹ���ɫ�ڵ�
2.1��X���ֵ�W�Ǻ�ɫ����취�����Ϊ��ɫ��
 ������W�Ǻ�ɫ�ģ��������ӽڵ�͸��ڵ��Ϊ��ɫ����ֻҪ��W���丸�ڵ����ɫ�Ի���
    �ٶԸ��ڵ����һ������ת���㽫W�����ӽڵ�ŵ���X���ֵܽڵ��ϣ�X���ֵܽڵ����˺�ɫ���Һ�����ʲ��䡣
    ���������ֻ꣬����ʱ�����1ת�������������2��3��4��

2.2��X���ֵܽڵ�W�Ǻ�ɫ�ģ�����W�������ӽڵ㶼�Ǻ�ɫ�ġ�
��ʱ���Խ�X��һ�غ�ɫ��W�ĺ�ɫͬʱȥ������ת�Ӹ����ǵĸ��ڵ��ϣ�����X��ָ�����ĸ��ڵ��ˣ�
��˴�ʱ���ڵ����˫����ɫ�ˡ���һ�غ�ɫ�ڵ����ơ���w�ĺڸ�ȥ������Ϊ��ɫ��
2.2.1��������ڵ�ԭ���Ǻ�ɫ�ģ������ּ�һ���ɫ����ôX����ָ�������ڵ���Ǻ����ɫ�ģ�ֱ�Ӱ�X��Ҳ���Ǹ��ڵ㣩��Ϊ��ɫ��������Ѿ���������ˡ�
2.2.2��������ڵ�������˫���ɫ���Ǿ��Ը��ڵ�Ϊ�µ�X�������ϵ���һ�εĵݹ顣

2.3��X���ֵܽڵ�W�Ǻ�ɫ�ģ�����W�����ӽڵ��Ǻ�ɫ�ģ����ӽڵ��Ǻ�ɫ�ġ�
��ʱͨ������W�������ӽڵ����ɫ������һ��������ת�Ϳ�ת��������ĵ����������ע�⣬ԭ��L�Ǻ�ɫ�ģ�����L���ӽڵ�һ���Ǻ�ɫ�ģ�
    ������ת��L�ڵ��һ�������ҵ�֮����Ϊ��ɫ��W�ڵ��ϲ����ƻ�������ʡ����κ��ɫ�߶Ȳ��䡣Ȼ��ת��2.4����

2.4��X���ֵܽڵ�W�Ǻ�ɫ�ģ�����W�����ӽڵ��Ǻ�ɫ�ġ�
��������£��Ը�������һ��������W�ʹ��ڸ���λ�ã���W����Ϊԭ���ĸ���λ�õ���ɫ��ͬʱ��W�������µĶ��ӽڵ����ɫ��Ϊ��ɫ��ȥ��X��һ�غ�ɫ��
������������Ҳ�͵õ��˽�����ݹ���������ڴ����ϣ�Ϊ�˱�ʶ�ݹ���������ǰ�Xָ����ڵ㣩
*/
S_RbNode* Rbt::deleteNodeRebalance(S_RbNode* pXNode, S_RbNode* pParentNode, S_RbNode* pRoot)
{
//	printf("~~~~ deleteNodeRebalance pXNode(%p), pParentNode(%p), pRoot(%p)\n",
//			pXNode, pParentNode, pRoot); fflush(stdout);
	/* ��ɾλ�ý��Ϊ��ɫ��XΪ����X���ֵ�WҲΪ�յ���������� */
	S_RbNode* pWNode = NULL;
	while((pXNode != pRoot) && ((NULL == pXNode) || (BLACK == pXNode->colour)))
	{
		if(pXNode == pParentNode->lChild)			/* X���丸������ */
		{
			pWNode = pParentNode->rChild;
			if(RED == pWNode->colour)				/* 2.1 */
			{
				pWNode->colour = BLACK;
				pParentNode->colour = RED;
				leftRotate(pParentNode, &pRoot);
				pWNode = pParentNode->rChild;		/* ���������ת��2.2~2.4 */
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
					pWNode = pParentNode->rChild;	/* ת��2.4 */
				}
													/* 2.4 */
				pWNode->colour = pParentNode->colour;
				pParentNode->colour = BLACK;
				pWNode->rChild->colour = BLACK;
				leftRotate(pParentNode, &pRoot);
				pXNode = pRoot;						/* ��ʶѭ������ */
			}
		}
		else										/* X���丸���Һ��� */
		{
			pWNode = pParentNode->lChild;
			if(RED == pWNode->colour)				/* 2.1 */
			{
				pWNode->colour = BLACK;
				pParentNode->colour = RED;
				rightRotate(pParentNode, &pRoot);
				pWNode = pParentNode->lChild;		/* ���������ת��2.2~2.4 */
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
					pWNode = pParentNode->lChild;	/* ת��2.4 */
				}
													/* 2.4 */
				pWNode->colour = pParentNode->colour;
				pParentNode->colour = BLACK;
				pWNode->lChild->colour = BLACK;
				rightRotate(pParentNode, &pRoot);
				pXNode = pRoot;						/* ��ʶѭ������ */
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

	/* x��ΪA�������� */
	pNodeA->rChild = pNodeB->lChild;
	if(NULL != pNodeB->lChild)
		pNodeB->lChild->parent = pNodeA;
	/* B��A�ĸ����� */
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
	/* A��ΪB�������� */
	pNodeB->lChild = pNodeA;
	pNodeA->parent = pNodeB;
//	return pRoot;
}

void Rbt::rightRotate(S_RbNode* pNodeA, S_RbNode** ppRoot)
{
	S_RbNode* pNodeB = pNodeA->lChild;

	/* x��ΪA�������� */
	pNodeA->lChild = pNodeB->rChild;
	if(NULL != pNodeB->rChild)
		pNodeB->rChild->parent = pNodeA;
	/* B��A�ĸ����� */
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
	/* A��ΪB�������� */
	pNodeB->rChild = pNodeA;
	pNodeA->parent = pNodeB;
//	return pRoot;
}

/* end of file */
