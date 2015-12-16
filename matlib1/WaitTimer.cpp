// WaitTimer.cpp: implementation of the CWaitTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TimerEx01.h"
#include "WaitTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaitTimer::CWaitTimer()
{   
	m_hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hThread =  NULL;
}

CWaitTimer::~CWaitTimer()
{
//	KillWaitTimer();
	CloseHandle(m_hEvent);
	WaitForSingleObject(m_hThread,INFINITE);
}

UINT	TimerThreadProc(LPVOID  pParam)
{
		CWaitTimer	*pTimer = static_cast<CWaitTimer *>(pParam);
		HANDLE		hTimer;
		DWORD		dwError = -1;

		hTimer = ::CreateWaitableTimer(NULL,FALSE,NULL);
	    if(hTimer)
		{
				//if(::GetLastError() == ERROR_ALREADY_EXISTS)
				//	TRACE(_T("Timer Handle Created \n"));
		}
		else
		{
				dwError = ::GetLastError();
				//TRACE(_T("Error code %d\n"),dwError);
				return  dwError;
		}
		
		LARGE_INTEGER liDueTime;
		
        liDueTime.QuadPart=-10000000 * pTimer->m_lDelay;
		
		if (!SetWaitableTimer(hTimer, 
					&liDueTime,
					pTimer->m_lDelay * 1000,  
					pTimer->m_pTimerProc,
					pTimer->m_pArgToTimerProc , 0))
		{
			DWORD	dwError = ::GetLastError();			 
			//TRACE(_T("SetWaitableTimer failed (%d)\n"),dwError);
			return dwError;
		}
	//	DWORD   dwWhile = 0;
		while(TRUE)
		{
			if(WaitForSingleObject(pTimer->m_hEvent,0) == WAIT_OBJECT_0)
			{
				ResetEvent(pTimer->m_hEvent);
				CloseHandle(hTimer);
				hTimer = NULL;
				pTimer->m_hThread = NULL;
				return  0;				//	Stop Timer and  KillTimer
			}
			if(WaitForSingleObject(hTimer,0) == WAIT_OBJECT_0 )
			{
				SleepEx(INFINITE,TRUE);
			//	TRACE("While = %d\n",dwWhile);
			//	dwWhile++;
			}
			 
		}
		return	0;
}
DWORD CWaitTimer::CreateWaitTimer(LONG lDelay,PTIMERAPCROUTINE pTimerProc,LPVOID pArgToTimerProc)
{
		m_lDelay = lDelay;
		CWinThread	*pThread = NULL;
		
		if(m_hThread == NULL)
	       pThread = AfxBeginThread((AFX_THREADPROC)TimerThreadProc,this);
		
		if(pThread)
			m_hThread = pThread->m_hThread;
		else
			return  -1;

		m_pTimerProc = pTimerProc;
		m_pArgToTimerProc = pArgToTimerProc;

		return  0;
}

BOOL CWaitTimer::KillWaitTimer()
{
	if(!SetEvent(m_hEvent))
		return	FALSE;
    return  TRUE;
}
