
#ifndef BST_H_
#define BST_H_

//#include <string.h>
#include "commonDef.h"


class Bst
{
public:
	Bst();
	virtual ~Bst();
	virtual void deleteTree(S_RbNode* pRoot);

	/* 插入结点 */
	virtual S_RbNode* insertNode(const S_Data& sData, S_RbNode* pRoot);
	/* 删除结点 */
	virtual S_RbNode* deleteNode(const int id, S_RbNode* pRoot);
	/* 查找 */
	virtual S_RbNode* searchNode(const int id, const S_RbNode* pRoot);

	virtual void test(void);

	virtual void printTree(S_RbNode* pRoot, int order = 0);
	virtual int  getHeight(S_RbNode* pRoot);

};

#endif /* BST_H_ */

/* end of file */
