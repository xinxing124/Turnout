// Turnout.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CTurnoutApp:
// 有关此类的实现，请参阅 Turnout.cpp
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
// 重写
public:
	virtual BOOL InitInstance();
	
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTurnoutApp theApp;
