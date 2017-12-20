// TurnoutDlg.h : ͷ�ļ�
//

#pragma once

#include "CESeries.h"
#include "afxwin.h"

// CTurnoutDlg �Ի���
class CTurnoutDlg : public CDialog
{
// ����
public:
	CTurnoutDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TURNOUT_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	// ���ڽ������ݴ�����
	afx_msg LONG OnRecvSerialData(WPARAM wParam,LPARAM lParam);
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
private:
	CBrush   m_brush1; 
	CBrush   m_brush2; 
	double	 m_OnceVal;
	double   m_SecondaryVal;
	double   m_Result;
	double	 m_Xishu[17];
	UINT16 m_Val;
private:
	//�̺߳���
    static  DWORD WINAPI ThreadFunc(LPVOID lparam);
	//�رն��߳�
	void CloseThread();
private:
	//���߳̾��
	HANDLE m_hThread;
	//���߳�ID��ʶ
	DWORD m_dwThreadID;
	

public:
	CCESeries *m_pSerial;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString m_ShowResult;
	CString m_ShowTitle;
	CFont fontTitle;
	CFont fontContent;
	CFont fontCompany;
	afx_msg void OnBnClickedButton4();
	int FileRead(CString filename,CString* content);
	void Split(CString source, CStringArray& dest, CString division);
	double GetMedianNum(double * dArray, int iFilterLen);
	void ReadParam(void);
	CString ReturnPath(void);
private:
	//���崮�ڽ������ݺ�������
	static void CALLBACK OnSerialRead(void * pOwner,BYTE* buf,DWORD bufLen);
	int GatherData(void);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton5();
	CButton btnExit;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
};
