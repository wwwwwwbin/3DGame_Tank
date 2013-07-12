//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   LogSystem.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-04 19:31
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogSystem.h"
#include <share.h>
#include <time.h>

FILE* CLogSystem::m_pLogFile = NULL;

CLogSystem::CLogSystem( void )
{

}

CLogSystem::~CLogSystem( void )
{
	CLogSystem::UnInit();
}

int CLogSystem::Init( void )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	struct tm sLocalTime; 
	
	__time64_t llTime;

	CHECK_SUCCEED_JUMP(m_pLogFile);

	m_pLogFile = _fsopen("Log.txt", "w+", _SH_DENYNO);
	CHECK_FAILD_JUMP(m_pLogFile);

	_time64(&llTime);

	nRetCode = _localtime64_s(&sLocalTime, &llTime);
	CHECK_FAILD_JUMP(!nRetCode);

	CLogSystem::LogPrint(
		LOG_TYPE_LOG,
		"Application Start: %d-%2.2d-%2.2d_%2.2d:%2.2d:%2.2d",
		sLocalTime.tm_year + 1900,
		sLocalTime.tm_mon + 1,
		sLocalTime.tm_mday,
		sLocalTime.tm_hour,
		sLocalTime.tm_min,
		sLocalTime.tm_sec
	);

Exit1:
	nResult = TRUE;
Exit0:
	return nResult;
}

void CLogSystem::LogPrint( LOG_TYPE eLogType, const char* cpszString, ... )
{
	int		nRetCode	= false;
	int		nBufferLen	= 0;
	char*	pszBuffer	= NULL;
	
	char	szBuffer[1024];
	va_list param;

	CHECK_FAILD_JUMP(eLogType >= LOG_TYPE_BEGIN);
	CHECK_FAILD_JUMP(eLogType <  LOG_TYPE_END);
	CHECK_FAILD_JUMP(cpszString);

	nBufferLen = sizeof(szBuffer);

	memset(szBuffer, 0, nBufferLen);

	pszBuffer = szBuffer;

	nRetCode = sprintf_s(pszBuffer, nBufferLen, "[%s]\t", s_cpszLogTitle[eLogType]);
	CHECK_FAILD_JUMP(nRetCode > 0);

	nBufferLen -= nRetCode;
	CHECK_FAILD_JUMP(nBufferLen > 0);
	pszBuffer += nRetCode;

	va_start(param, cpszString);
	nRetCode = vsnprintf_s(pszBuffer, nBufferLen, nBufferLen - 2, cpszString, param);
	CHECK_FAILD_JUMP(nRetCode > 0);

	nBufferLen -= nRetCode;
	CHECK_FAILD_JUMP(nBufferLen > 0);

	pszBuffer[nRetCode] = '\n';
	pszBuffer[nRetCode + 1] = '\0';

	va_end(param);

	nRetCode = fprintf_s(m_pLogFile, szBuffer);
	CHECK_FAILD_JUMP(nRetCode > 0);

	fflush(m_pLogFile);
Exit0:
	return;
}

void CLogSystem::UnInit( void )
{
	if (m_pLogFile)
	{
		CLogSystem::LogPrint(LOG_TYPE_LOG, "Log System Exit!");

		fclose(m_pLogFile);
		m_pLogFile = NULL;
	}	
}
