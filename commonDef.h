/*
 * commnonDef.h
 *
 *  Created on: 2015Äê10ÔÂ19ÈÕ
 */

#ifndef COMMONDEF_H_
#define COMMONDEF_H_

#include <string.h>

#define LOG_DEBUG(format,...)  	printf("line %d : "format, __LINE__, ##__VA_ARGS__)

#define BLACK	0
#define RED		1

typedef struct _S_Data
{
	int  id;
	int  value;
} S_Data;

typedef struct _S_RbNode
{
	struct _S_RbNode*	parent;
	struct _S_RbNode*	lChild;
	struct _S_RbNode*	rChild;
	S_Data				value;
	int 				colour;
	_S_RbNode()
	{
		memset(this, 0, sizeof(_S_RbNode));
	}
} S_RbNode;




#endif /* COMMONDEF_H_ */
