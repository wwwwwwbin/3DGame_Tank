#ifndef BINBIN_PUBLISH_H
#define BINBIN_PUBLISH_H

#define CHECK_FAILD_JUMP(Condition) 			\
	do											\
	{											\
		if (!(Condition))						\
		goto Exit0;								\
	} while (false)


#define CHECK_SUCCEED_JUMP(Condition)			\
	do											\
	{											\
		if (Condition)							\
		goto Exit1;								\
	} while (false)

#define SafeRelease(pObject)					\
	if(pObject!=NULL)							\
	{											\
		pObject->Release();						\
		pObject = NULL;							\
	}										


#include <stdio.h>
#include <tchar.h>

#endif //BINBIN_PUBLISH_H