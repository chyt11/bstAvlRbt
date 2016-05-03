
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <map>
#include "rbt.h"
#include "avl.h"
#include "bst.h"

using namespace std;

template<class TreeClass, typename S_NodeType>
void testOneBinTree(TreeClass& treeClass, S_NodeType* pRoot, S_Data* pData, int loopCount)
{
	pRoot = NULL;
	clock_t tStart = clock();
	for(int i = 0; i < loopCount; i++)
	{
		pRoot = treeClass.insertNode(pData[i], pRoot);
	}
	printf("insertNode finish! cost %lu ms\n", clock()- tStart); fflush(stdout);
//	treeClass.printTree(pRoot, 0);
//	printf(" %p == pRoot \n", pRoot);	fflush(stdout);
	if(NULL != pRoot)
	{
		printf(" LHeight(%d)  RHeight(%d) \n",
			treeClass.getHeight(pRoot->lChild), treeClass.getHeight(pRoot->rChild)); fflush(stdout);
	}
	else printf(" NULL == pRoot \n");
	fflush(stdout);

	S_NodeType* pFoundNode = NULL;
	tStart = clock();
	for(int i = 0; i < loopCount/* loopCount */; i++)
	{
		if(0x80 == (loopCount % 0x8000))
		{
			printf("search id(%d)\n", pData[i].id); fflush(stdout);
		}
		pFoundNode = treeClass.searchNode(pData[i].id, pRoot);
//		printf("search id(%d):", pData[i].id);
//		if(NULL != pFoundNode)
//			printf(" found value(%d)\n", pFoundNode->value.value);
//		else printf(" found NULL pointer\n");
	}
	printf("searchNode finish cost %lu ms\n", clock()- tStart); fflush(stdout);

	tStart = clock();
	for(int i = loopCount - 1; i >= 0; i--)	//	for(unsigned int i = 0; i < loopCount; i++)
	{
//		treeClass.printTree(pRoot, 0);
//		printf("delete id(%d):\n", pData[i].id); fflush(stdout);
		pRoot = treeClass.deleteNode(pData[i].id, pRoot);
	}
	printf("deleteNode finish cost %lu ms pRoot(%p)\n\n", clock()- tStart, pRoot); fflush(stdout);
	treeClass.deleteTree(pRoot);
	pRoot = NULL;
}

/* randomNumCountÉè¶¨²úÉúµÄËæ»úÊýÁ¿ */
void testBinTree(int randomNumCount, unsigned int mask)
{
	S_Data* pData = new S_Data[randomNumCount];

	int loopCount = randomNumCount;
	Rbt rbt;
	Avl avl;
	Bst bst;


	S_RbNode* pRoot = NULL;
//	S_RbNode* pFoundNode = NULL;
	int tmp = 0;
//	clock_t tStart;

	printf("**** Random Test: cout[%#x]****\n", loopCount);
	/* Ëæ»úÔªËØ²âÊÔ */
	for(int i = 0; i < loopCount; i++)
	{
		tmp = rand() & mask;	//FFFFF;
		pData[i].id = tmp;
		pData[i].value = tmp;// * 10;
	}
	/* ºìºÚÊ÷²âÊÔ */
	printf("Red black tree Test:\n");
	testOneBinTree(rbt, pRoot, pData, loopCount);
	/* AVLÊ÷²âÊÔ */
	printf("AVL tree Test:\n");
	testOneBinTree(avl, pRoot, pData, loopCount);
	/* ¶þ²æÅÅÐòÊ÷²âÊÔ */
	printf("Binary search tree Test:\n");
	testOneBinTree(bst, pRoot, pData, loopCount);

	printf("**** 1 ... n Test: cout[%#x]****\n", loopCount);
	/* ÓÐÐòÔªËØ²âÊÔ 1 */
	loopCount = randomNumCount;
	for(int i = 0; i < loopCount; i++)
	{
		tmp = i;
		pData[i].id = tmp;
		pData[i].value = tmp;// * 10;
	}
	/* ºìºÚÊ÷²âÊÔ */
	printf("Red black tree Test:\n");
	testOneBinTree(rbt, pRoot, pData, loopCount);
	/* AVLÊ÷²âÊÔ */
	printf("AVL tree Test:\n");
	testOneBinTree(avl, pRoot, pData, loopCount);
	/* ¶þ²æÅÅÐòÊ÷²âÊÔ */
//	printf("Binary search tree Test:\n");
//	testOneBinTree(bst, pRoot, pData, loopCount);

	printf("**** n ... 1 Test: cout[%#x]****\n", loopCount);
	/* ÓÐÐòÔªËØ²âÊÔ 2 */
	loopCount = randomNumCount;
	for(int i = 0; i < loopCount; i++)
	{
		tmp = loopCount - i;
		pData[i].id = tmp;
		pData[i].value = tmp;// * 10;
	}
	/* ºìºÚÊ÷²âÊÔ */
	printf("Red black tree Test:\n");
	testOneBinTree(rbt, pRoot, pData, loopCount);
	/* AVLÊ÷²âÊÔ */
	printf("AVL tree Test:\n");
	testOneBinTree(avl, pRoot, pData, loopCount);
	/* ¶þ²æÅÅÐòÊ÷²âÊÔ */
//	printf("Binary search tree Test:\n");
//	testOneBinTree(bst, pRoot, pData, loopCount);

	printf("**** 1 x 2 y 3 z.. n Test: cout[%#x]****\n", loopCount);
	/* ÓÐÐòËæ»ú½»ÌæÔªËØ²âÊÔ */
	loopCount = randomNumCount;
	for(int i = 0; i < loopCount; i++)
	{
		tmp = rand() & mask;	//FFFFF;
		pData[i].id = tmp;
		pData[i].value = tmp;// * 10;
	}
	for(int i = 0; i < loopCount; i += 2)
	{
		tmp = i;
		pData[i].id = tmp;
		pData[i].value = tmp;// * 10;
	}
	/* ºìºÚÊ÷²âÊÔ */
	printf("Red black tree Test:\n");
	testOneBinTree(rbt, pRoot, pData, loopCount);
	/* AVLÊ÷²âÊÔ */
	printf("AVL tree Test:\n");
	testOneBinTree(avl, pRoot, pData, loopCount);
	/* ¶þ²æÅÅÐòÊ÷²âÊÔ */
//	printf("Binary search tree Test:\n");
//	testOneBinTree(bst, pRoot, pData, loopCount);

	if(NULL != pData)	delete[] pData;
	pData = NULL;
	printf("testBinTree finish!\n");
}


int main(int argc, char* argv[])
{
    srand(time(0));

    testBinTree(0x80000, 0x7FFFFFFF);

    return 0;

}


/* end of file */
