// Turnout.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CTurnoutApp:
// �йش����ʵ�֣������ Turnout.cpp
//

class CTurnoutApp : public CWinApp
{
public:
	CTurnoutApp();

	CString FileContent;
	CString  StandardContent;
	CString  SectionName;
	CString  SwitchNumber;
	CString  SwitchType;
	CString  TrackMeasurement;
	CString  StandardTurnout;
	CString  MeasuringPosition;
	CString  StandardValue;
	CString  MeasuredValue;
	CString  DifferenceValue;
	CString UID;
	double Val;
// ��д
public:
	virtual BOOL InitInstance();
	
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTurnoutApp theApp;
