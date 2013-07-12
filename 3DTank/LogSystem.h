//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   LogSystem.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-04 19:31
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

enum LOG_TYPE
{
	LOG_TYPE_INVALID,
	LOG_TYPE_BEGIN,

	LOG_TYPE_DEBUG = LOG_TYPE_BEGIN,
	LOG_TYPE_ERROR,
	LOG_TYPE_LOG,

	LOG_TYPE_END,
	LOG_TYPE_SIZE = LOG_TYPE_END,
};

static const char* s_cpszLogTitle[] = 
{
	"INVALID",
	"DEBUG", 
	"ERROR", 
	"LOG", 
};

class CLogSystem
{
public:

	static int  Init(void);
	static void UnInit(void);
	static void LogPrint(LOG_TYPE eLogType, const char* cpszString, ...);

private:
	static FILE* m_pLogFile;
	//static const char* ms_cpszLogTitle[LOG_TYPE_SIZE];

	CLogSystem(void);
	~CLogSystem(void);
};
