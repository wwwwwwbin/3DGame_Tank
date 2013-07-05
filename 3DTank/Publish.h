#ifndef BINBIN_PUBLISH_H
#define BINBIN_PUBLISH_H

#include <stdio.h>
#include <tchar.h>
#include "LogSystem.h"

#define CHECK_FAILD_JUMP(Condition) 														\
	do																						\
	{																						\
		if (!(Condition))																	\
		goto Exit0;																			\
	} while (false)

#define LOG_FAILD_JUMP(Condition)															\
	do																					 	\
	{																					 	\
	if (!(Condition))																	 	\
	{																				 		\
		CLogSystem::LogPrint(																\
		LOG_TYPE_ERROR,																		\
		"LOG_FAILD_JUMP(%s) at line %d in %s", #Condition, __LINE__, __FUNCTION__			\
		);																				 	\
		goto Exit0;																		 	\
	}																				 		\
	} while (false)

#define CHECK_SUCCEED_JUMP(Condition)														\
	do																						\
	{																						\
		if (Condition)																		\
		goto Exit1;																			\
	} while (false)

#define SafeRelease(pObject)																\
	if(pObject!=NULL)																		\
	{																						\
		pObject->Release();																	\
		pObject = NULL;																		\
	}			

#define CHECK_HRESULT_FAILD_JUMP(hResult)													\
	do																						\
	{																						\
	if (FAILED(hResult))																	\
	goto Exit0;																				\
	} while (false)

#define LOG_HRESULT_FAILD_JUMP(hResult)														\
	do																						\
	{																						\
	if (FAILED(hResult))																	\
	{																						\
		CLogSystem::LogPrint(																\
		LOG_TYPE_ERROR,																		\
		"LOG_FAILD_JUMP(%s) at line %d in %s", #hResult, __LINE__, __FUNCTION__				\
		);																					\
		goto Exit0;																			\
	}																						\
	} while (false)


#endif //BINBIN_PUBLISH_H