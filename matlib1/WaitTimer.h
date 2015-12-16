// WaitTimer.h: interface for the CWaitTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAITTIMER_H__53C1DDC4_35DC_4A03_BC86_3F75AC23481E__INCLUDED_)
#define AFX_WAITTIMER_H__53C1DDC4_35DC_4A03_BC86_3F75AC23481E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaitTimer  
{
public:
	BOOL KillWaitTimer();
	DWORD CreateWaitTimer(LONG  lDelay,PTIMERAPCROUTINE pTimerProc,LPVOID pArgToTimerProc);
	CWaitTimer();
	virtual ~CWaitTimer();
 
//	HANDLE	m_hTimer;
	HANDLE	m_hEvent;
	HANDLE	m_hThread;
	LONG	m_lDelay;
	PTIMERAPCROUTINE	m_pTimerProc;
	LPVOID	m_pArgToTimerProc;
};

#endif // !defined(AFX_WAITTIMER_H__53C1DDC4_35DC_4A03_BC86_3F75AC23481E__INCLUDED_)
