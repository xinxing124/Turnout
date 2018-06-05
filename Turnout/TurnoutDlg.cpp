// TurnoutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Turnout.h"
#include "TurnoutDlg.h"
#include "HDI_API.h"
#include "MessageDlg.h"
#include "tchar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//���崮�����ݽ�����Ϣ����
#define WM_RECV_SERIAL_DATA WM_USER + 101
#define WM_UPDATESIMULATION 1001

// CTurnoutDlg �Ի���
//���߳��˳��¼�
HANDLE g_hDetIOEvent;
HWND g_hWnd;

CTurnoutDlg::CTurnoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTurnoutDlg::IDD, pParent)
	, m_ShowResult(_T(""))
	, m_ShowTitle(_T(""))
	, m_pSerial(NULL)
	,m_Val(3600)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSerial=NULL;
}

void CTurnoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC2, m_ShowResult);
	DDX_Text(pDX, IDC_STATIC1, m_ShowTitle);
	DDX_Control(pDX, IDC_BUTTON5, btnExit);
}

BEGIN_MESSAGE_MAP(CTurnoutDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECV_SERIAL_DATA,OnRecvSerialData)
	ON_BN_CLICKED(IDC_BUTTON1, &CTurnoutDlg::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CTurnoutDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTurnoutDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTurnoutDlg::OnBnClickedButton4)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON5, &CTurnoutDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTurnoutDlg::OnBnClickedButton6)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTurnoutDlg ��Ϣ�������
//�̺߳���
DWORD CTurnoutDlg::ThreadFunc(LPVOID lparam)
{
	CTurnoutDlg *TurnoutDlg = (CTurnoutDlg*)lparam;
	DWORD dwRet;
	while (TRUE)
	{   	
		//����յ����߳��˳��źţ����˳��߳�
		dwRet=WaitForSingleObject(g_hDetIOEvent,500);
		if (dwRet== WAIT_OBJECT_0)//����ģʽ
		{
			//::AfxMessageBox(_T("1"));
			//TurnoutDlg->ReadParam();
			::SetFocus(g_hWnd);
			::ShowWindow(g_hWnd,1);
			break;
		}
		if ( dwRet== WAIT_TIMEOUT)//�޷��س�ʱ
		{
			//WAIT_TIMEOUT
			//::AfxMessageBox(_T("2"));
			//Sleep(10000);
			//TurnoutDlg->ReadParam();
			::SetFocus(g_hWnd);
			::ShowWindow(g_hWnd,0);
			break;
		}
	}
	return 0;
}

//�رն��߳�
void CTurnoutDlg::CloseThread()
{
    //�ȴ�4�룬������߳�û���˳�����ǿ���˳�
    if (WaitForSingleObject(m_hThread,4000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThread,0);
	}
	m_hThread = NULL;
}
BOOL CTurnoutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	SetTimer(WM_UPDATESIMULATION, 10000, NULL);

	g_hDetIOEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("DetIOEvent"));

	if(!g_hDetIOEvent)
	{
		//RETAILMSG(1,(TEXT("### Error -- OpenEvent\r\n")));
		//::AfxMessageBox(_T("OpenEvent"));
		return 0;
	}

	g_hWnd=GetDlgItem(IDC_BUTTON5)->m_hWnd; 
	//���������߳�
	m_hThread = CreateThread(NULL,0,ThreadFunc,this,0,&m_dwThreadID);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	Sleep(10000);
	ReadParam();
	//CFont Font;
	//Font.CreatePointFont(200,L"MS Sans Serif");
	//CFont Font;
	//Font.CreatePointFont(20,L"����");
	
	int iFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int iFullHeight = GetSystemMetrics(SM_CYSCREEN); 
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, iFullWidth, iFullHeight, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);

	fontTitle.CreatePointFont(200,L"΢���ź�");  
	fontContent.CreatePointFont(280,L"΢���ź�");  
	fontCompany.CreatePointFont(150,L"΢���ź�");  
	

	//GetDlgItem(IDC_STATIC)->SetFont(&Font);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("һ�β���"));
	GetDlgItem(IDC_STATIC2)->SetWindowTextW(_T("0.000"));
	//GetDlgItem(IDC_STATIC1)->SetFont(&Font);
	
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(_T("mm"));
	GetDlgItem(IDC_STATIC4)->SetWindowTextW(_T("��콵��ֵ������"));
	//GetDlgItem(IDC_STATIC5)->SetWindowTextW(_T("����������·�豸���޹�˾"));
	GetDlgItem(IDC_STATIC5)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC8)->SetWindowTextW(_T("UID:")+theApp.UID);
	
	::SetCursorPos(iFullHeight,iFullWidth);
	ShowCursor(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CTurnoutDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_TURNOUT_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_TURNOUT_DIALOG));
	}
}
#endif


void CTurnoutDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*
	double dTemp=0.0;
	double dVal[15];
	UINT16 Val;
	for(int i=0;i<5;i++)
	{
	API_ADC_Read(0,&Val);
	
	//m_OnceVal=Val*43.3/4096.0;
	dVal[i]=Val*43.3/4096.0;
	//dVal[i]=Val*1.0;
	//dVal[i]=(i+1)*1.0;
	dTemp+=dVal[i];
	}
	m_OnceVal=dTemp/5.0;
	//m_OnceVal=GetMedianNum(dVal,5);
	//	double	 m_OnceVal;
	//double   m_SecondaryVal;
	//double   m_Result;

	//this->GetDlgItem(IDC_STATIC)->SetWindowTextW("");
	//GetDlgItem(IDC_STATIC)->SetWindowTextW(_T("���տƼ�"));
	//GetDlgItem(IDC_STATIC)->UpdateData(false);
	//this->SetDlgItemTextW(IDC_STATIC,_T("���տƼ�"));
	//GetDlgItem(IDC_STATIC)->Invalidate();

	//SetDlgItemText(IDC_STATIC,_T("ABC"));
	//GetDlgItem(IDC_STATIC)->SetWindowText(_T("111"));////����������Ϊ111 
	//SetDlgItemText(IDC_STATIC,_T("��ʾ����"));
	m_ShowTitle.Format(_T("һ�β���"));
	m_ShowResult.Format(_T("%.3f"),m_OnceVal);
	UpdateData(false);
*/
	m_OnceVal=-99.9;
	m_ShowTitle.Format(_T("һ�β���"));
	GatherData();
}

HBRUSH CTurnoutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	switch(pWnd->GetDlgCtrlID())   
	{   
		case IDC_STATIC1: 
		    pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,0,0));    //Static�ؼ�1��������ɫ-��ɫ
			pDC->SelectObject(&fontTitle); 
			
			//return (HBRUSH)m_brush1.GetSafeHandle();
		break;   

		case IDC_STATIC2: 
		    pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(0,255,0));    //Static�ؼ�2��������ɫ-��ɫ
			pDC->SelectObject(&fontContent); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;   

		case IDC_STATIC3: 
 			pDC->SetTextColor(RGB(0,0,255)); //Static�ؼ�3��������ɫ-��ɫ
			pDC->SelectObject(&fontTitle); 
			//pDC->SetBkColor(RGB(255,0,0));   //Static�ؼ�3�����屳����ɫ-��ɫ
		break;  
		case IDC_STATIC4: 
		    pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(255,255,255));    //Static�ؼ�2��������ɫ-��ɫ
			pDC->SelectObject(&fontContent); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;  
		case IDC_STATIC5: 
		    pDC->SetBkMode(TRANSPARENT); 
			//pDC->SetTextColor(RGB(255,255,255));    //Static�ؼ�2��������ɫ-��ɫ
			pDC->SelectObject(&fontCompany); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;  
		case IDC_STATIC8: 
		    pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(199,199,199));    //Static�ؼ�2��������ɫ-��ɫ
			//pDC->SetTextColor(RGB(200,200,200));    //Static�ؼ�2��������ɫ-��ɫ
			pDC->SelectObject(&fontCompany); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;  
	}  
	GetDlgItem(IDC_BUTTON1)->UpdateWindow();
	GetDlgItem(IDC_BUTTON2)->UpdateWindow();
	GetDlgItem(IDC_BUTTON3)->UpdateWindow();
	GetDlgItem(IDC_BUTTON4)->UpdateWindow();
	GetDlgItem(IDC_BUTTON5)->UpdateWindow();
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CTurnoutDlg::OnBnClickedButton2()
{
	/*
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double dTemp=0.0;
	double dVal[5];
	UINT16 Val;
	for(int i=0;i<5;i++)
	{
	API_ADC_Read(0,&Val);
	//m_SecondaryVal=Val*43.3/4096.0;
	dVal[i]=Val*43.3/4096.0;
	dTemp+=dVal[i];
	}
	//m_SecondaryVal=GetMedianNum(dVal,5);
	m_SecondaryVal=dTemp/5.0;

	m_ShowTitle.Format(_T("���β���"));
	m_ShowResult.Format(_T("%.3f"),m_SecondaryVal);
	UpdateData(false);
*/
	m_ShowTitle.Format(_T("���β���"));
	m_SecondaryVal=-99.9;
	GatherData();
}

void CTurnoutDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_Result=fabs(m_SecondaryVal-m_OnceVal);
	m_Result=m_OnceVal-m_SecondaryVal;
	//m_Result=-0.0006*pow(m_Result,3.0)+0.0046*pow(m_Result,2.0)+1.0328*pow(m_Result,1.0)-0.0642;
	theApp.Val = m_Result;
	m_ShowTitle.Format(_T("�������"));
	m_ShowResult.Format(_T("%.3f"),m_Result);
	UpdateData(false);
}

void CTurnoutDlg::OnBnClickedButton4()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMessageDlg dlg;
	//m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	
	//OnOK();
}

int CTurnoutDlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// �ļ���� 
	if(::GetFileAttributes(filename)==0xFFFFFFFF)
	{
	//::AfxMessageBox(_T("�ļ�������"));
		return -5;
	}
	// ����һ���ļ����һ���ļ�
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("���ļ�ʧ��!"));
		return -1;
	}

	DWORD filelen,actlen;
	char *pcharbuff;
	
	filelen = GetFileSize(hFile, NULL);							// ��ȡ�ļ���С
	if (filelen == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("��ȡ�ļ���Сʧ��!"));
		return -2;	
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		// �ƶ��ļ�ָ�뵽�ļ���ͷ
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("���ļ�ָ�������ļ���ͷʧ��!"));
		return -3;	
	}

	pcharbuff = new char[filelen];
	ret = ReadFile(hFile, pcharbuff, filelen, &actlen, NULL);	// ���ļ��ж������� 
	if (ret == TRUE)
	{
		LPTSTR pStr = content->GetBuffer(filelen);	
		// ���ֽ�ת��Ϊ Unicode �ַ���
		MultiByteToWideChar(CP_ACP, 0, pcharbuff, filelen, pStr, filelen);
		content->ReleaseBuffer();
		//AfxMessageBox(_T("���ļ��ɹ�!"));	
		lRet=0;
	}
	else
	{
		//AfxMessageBox(_T("���ļ�ʧ��!"));	
		lRet=-4;
	}

	if (pcharbuff != NULL) 
		delete[] pcharbuff;

	CloseHandle(hFile);
	return lRet;
}

void CTurnoutDlg::Split(CString source, CStringArray& dest, CString division)
{
    dest.RemoveAll();
    int pos = 0;
    int pre_pos = 0;
    while( -1 != pos ){
        pre_pos = pos;
        pos = source.Find(division,(pos));
		if(pos<0)
			dest.Add(source.Mid(pre_pos));
		else
		{
			dest.Add(source.Mid(pre_pos,(pos-pre_pos)));
			pos++;
		}
		
    }
}

double CTurnoutDlg::GetMedianNum(double * dArray, int iFilterLen)  
{  
    int i,j;// ѭ������  
    double dTemp;  
      
    // ��ð�ݷ��������������  
    for (j = 0; j < iFilterLen ; j ++)  
    {  
        for (i = 0; i < iFilterLen - j; i ++)  
        {  
            if (dArray[i] > dArray[i + 1])  
            {  
                // ����  
                dTemp = dArray[i];  
                dArray[i] = dArray[i + 1];  
                dArray[i + 1] = dTemp;  
            }  
        }  
    }  
      
    // ������ֵ  
    if ((iFilterLen & 1) > 0)  
    {  
        // ������������Ԫ�أ������м�һ��Ԫ��  
        dTemp = dArray[(iFilterLen + 1) / 2];  
    }  
    else  
    {  
        // ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ  
        dTemp = (dArray[iFilterLen / 2] + dArray[iFilterLen / 2 + 1]) / 2;  
    }  
  
    return dTemp;  
}  

//���崮�ڽ������ݺ�������
void CALLBACK CTurnoutDlg::OnSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	BYTE *pRecvBuf = NULL; //���ջ�����
	//�õ�������ָ��
	CTurnoutDlg* pThis = (CTurnoutDlg*)pOwner;
	//�����յĻ�����������pRecvBuf��
	pRecvBuf = new BYTE[bufLen];
	CopyMemory(pRecvBuf,buf,bufLen);

	//�����첽��Ϣ����ʾ�յ��������ݣ���Ϣ�����꣬Ӧ�ͷ��ڴ�
	pThis->PostMessage(WM_RECV_SERIAL_DATA,WPARAM(pRecvBuf),bufLen);

}

// ���ڽ������ݴ�����
LONG CTurnoutDlg::OnRecvSerialData(WPARAM wParam,LPARAM lParam)
{
	CString strOldRecv = L"";
	CString strRecv = L"";
	//���ڽ��յ���BUF
	CHAR *pBuf = (CHAR*)wParam;
	//Wendu=*pBuf;
	//���ڽ��յ���BUF����
	DWORD dwBufLen = lParam;
	
	//���տ�
	//CEdit *pEdtRecvMsg = (CEdit*)GetDlgItem(IDC_STATIC2);
	//ASSERT(pEdtRecvMsg != NULL);

	//�õ����տ��е���ʷ�ı�
	//pEdtRecvMsg->GetWindowTextW(strOldRecv);

	//
    strRecv = CString(pBuf+2);
	double t=atof(pBuf+2);
	double nTmp=t;
	//t=m_Xishu[3]*pow(t,3.0)+m_Xishu[2]*pow(t,2.0)+m_Xishu[1]*pow(t,1.0)+m_Xishu[0];
	//B2+(B3-B2)/(A3-A2)*(x-A2)

	if(m_Xishu[0]<t&&t<=m_Xishu[2]) nTmp=0.0+5.0/(m_Xishu[2]-m_Xishu[0])*(t-m_Xishu[0]);//t=t*m_Xishu[1];//0-5
	if(m_Xishu[2]<t&&t<=m_Xishu[4]) nTmp=5.0+5.0/(m_Xishu[4]-m_Xishu[2])*(t-m_Xishu[2]);//t=t*m_Xishu[3];//5-10
	if(m_Xishu[4]<t&&t<=m_Xishu[6]) nTmp=10.0+5.0/(m_Xishu[6]-m_Xishu[4])*(t-m_Xishu[4]);//t=t*m_Xishu[5];//10-15
	if(m_Xishu[6]<t&&t<=m_Xishu[8]) nTmp=15.0+5.0/(m_Xishu[8]-m_Xishu[6])*(t-m_Xishu[6]);//t=t*m_Xishu[7];//15-20
	if(m_Xishu[8]<t&&t<=m_Xishu[10]) nTmp=20.0+5.0/(m_Xishu[10]-m_Xishu[8])*(t-m_Xishu[8]);//t=t*m_Xishu[9];//20-25

	if(m_Xishu[10]<t&&t<=m_Xishu[12]) nTmp=25.0+5.0/(m_Xishu[12]-m_Xishu[10])*(t-m_Xishu[10]);//t=t*m_Xishu[11];//25-30
	if(m_Xishu[12]<t&&t<=m_Xishu[14]) nTmp=30.0+5.0/(m_Xishu[14]-m_Xishu[12])*(t-m_Xishu[12]);//t=t*m_Xishu[13];//30-35
	if(m_Xishu[14]<t&&t<=m_Xishu[16]) nTmp=35.0+5.0/(m_Xishu[16]-m_Xishu[14])*(t-m_Xishu[14]);//t=t*m_Xishu[15];//35-40

	if(m_OnceVal==-99.9)
	{
		m_OnceVal=nTmp;
	}
	else if(m_SecondaryVal==-99.9)
	{
		m_SecondaryVal=nTmp;
	}
	
	//���½��յ����ı���ӵ����տ���
	strOldRecv = strOldRecv + strRecv;
	
	//pEdtRecvMsg->SetWindowTextW(strRecv);

	//�ͷ��ڴ�
	delete[] pBuf;
	pBuf = NULL;

	//m_ShowResult.Format(_T("%s_%f"),strRecv,t);
	m_ShowResult.Format(_T("%.3f"),nTmp);
	UpdateData(false);

	//�رմ���
	//
	if (m_pSerial != NULL)
	{
		//�رմ���
		m_pSerial->ClosePort();

		//�ͷŴ��ڶ���
		delete m_pSerial;
		m_pSerial = NULL;
		//AfxMessageBox(L"���ڹرճɹ�");
	}


	return 0;
}
int CTurnoutDlg::GatherData(void)
{
	
	//�жϴ����Ƿ��Ѿ���
	if (m_pSerial != NULL)
	{
		m_pSerial->ClosePort();

		delete m_pSerial;
		m_pSerial = NULL;
	}
	
	//�½�����ͨѶ����
	m_pSerial = new CCESeries();
	m_pSerial->m_OnSeriesRead = OnSerialRead; //

	//�򿪴���
	if(m_pSerial->OpenPort(this,1,9600,0,8,0))
	{
		//AfxMessageBox(L"���ڴ򿪳ɹ�");
		Sleep(200);
	}
	else
	{
		AfxMessageBox(L"���ڴ�ʧ��");
	}


	
	//��������
	char * buf  =NULL;  //���巢�ͻ�����
	DWORD dwBufLen = 0;   //���巢�ͻ���������
	CString strSend = L"#010\r";

	//�������û�д򿪣�ֱ�ӷ���
	if (m_pSerial == NULL)
	{
		AfxMessageBox(L"���ȴ򿪴���");
		return -1;
	}
	
	//�������͵��ַ���ת���ɵ��ֽڣ����з���
	buf = new char[strSend.GetLength()*2+1];
	ZeroMemory(buf,strSend.GetLength()*2+1);
	//ת���ɵ��ֽڽ��з���	
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strSend.GetBuffer(strSend.GetLength())
	   ,strSend.GetLength(),buf,strSend.GetLength()*2,NULL,NULL);

	dwBufLen = strlen(buf) + 1;

	//�����ַ���
	m_pSerial->WriteSyncPort((BYTE*)buf,dwBufLen);

	//�ͷ��ڴ�
	delete[] buf;
	buf = NULL;

	return 0;
}

void CTurnoutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//OnOK();
	CDialog::OnLButtonUp(nFlags, point);
}

void CTurnoutDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CloseThread();
	OnOK();
}

CString CTurnoutDlg::ReturnPath(void)
{
	  CString    sPath;   
	  GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	  sPath.ReleaseBuffer();   
	  int    nPos;   
	  nPos=sPath.ReverseFind('\\');   
	  sPath=sPath.Left(nPos+1);   
	  return    sPath;   
}

void CTurnoutDlg::ReadParam(void)
{
	//if(::GetFileAttributes(_T("\\ResidentFlash2\\GUI\\parameter.txt")) == 0xFFFFFFFF)
	//{
		if(::GetFileAttributes(_T("\\Ӳ��\\parameter.txt")) != 0xFFFFFFFF)
			::CopyFile(_T("\\Ӳ��\\parameter.txt"),ReturnPath()  +_T("parameter.txt"),false);
	//}
	//if(::GetFileAttributes(_T("\\ResidentFlash2\\GUI\\xishu.txt")) == 0xFFFFFFFF)
	//{
		//if(::GetFileAttributes(_T("\\Ӳ��\\xishu.txt")) != 0xFFFFFFFF)
		//	::CopyFile(_T("\\Ӳ��\\xishu.txt"),_T("\\ResidentFlash2\\GUI\\xishu.txt"),false);
	//}
	//(CTurnoutApp*) AfxGetApp()->MyVar
	//
	int ret=0;
	CString strTmp;
	FileRead(ReturnPath()  +_T("parameter.txt"),&strTmp);
	ret=strTmp.Find(_T("{"));
	if(ret>0)
	{
		theApp.StandardContent=strTmp.Left(ret-1);
		theApp.FileContent=strTmp.Mid(ret);
	}
	else
	{
		theApp.StandardContent=_T("");
		theApp.FileContent=strTmp;
	}
		
	ret=FileRead(ReturnPath()  +_T("xishu.txt"),&strTmp);
	if(ret<0)
	{
		//m_Xishu[0]=1.0;
		//m_Xishu[1]=1.0;
		//m_Xishu[2]=1.0;
		//m_Xishu[3]=1.0;
		//m_Xishu[4]=1.0;

		for(int i=0;i<17;i++)
		{
			m_Xishu[i]=1.0;
		}
	}
	else
	{
		CString sTmp;
		CStringArray dest;
		wchar_t   *stopstring;
		if(strTmp.Find(_T(";"))>0)
		{
			Split(strTmp,dest,_T(";"));
			theApp.UID=dest[0];
			sTmp=dest[1];
		}
		else
		{
			sTmp=strTmp;
		}
		Split(sTmp,dest,_T(","));
		int Count = dest.GetSize();
		for(int i=0;i<17;i++)
		{
			//sscanf( dest[i].GetBuffer(dest[i].GetLength()), "%f", &m_Xishu[i] );
			//m_Xishu[i]=atof(dest[i].GetBuffer(dest[i].GetLength()));
			//m_Xishu[i]=atof(dest[i]);
			m_Xishu[i]=wcstod(dest[i],&stopstring);

		}
	}
}

void CTurnoutDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CTurnoutDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT16 Val;
	API_ADC_Read(0,&Val);
	//if(Val<m_Val) m_Val=Val;
	m_Val=Val;
	double dBattery=m_Val*5.0/4096.0;

	 HBITMAP hBitmap;
	 CStatic *pStatic=(CStatic *)GetDlgItem(IDC_BATTERY);
	 /*
	 if(dBattery>=4.47)
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	 }
	 
	 if(dBattery>=4.204&&dBattery<4.47)
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	 }
	*/
	 if(dBattery>=3.75)//4.20
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	 }else if(dBattery>=3.48&&dBattery<3.75)//if(dBattery>=3.93&&dBattery<4.20)
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	 }else if(dBattery>=3.22&&dBattery<3.48)//if(dBattery>=3.93&&dBattery<4.20)
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
	 }else if(dBattery>=2.95&&dBattery<3.22)//if(dBattery>=3.66&&dBattery<3.93)
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
	 }else if(dBattery<2.95)////if(dBattery<3.66)
	 {
		//���λͼ���
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
	 }
	 //���þ�̬�ؼ�����ʽ��ʹ�����ʹ��λͼ������λ����ʾʹ����
	 pStatic->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
	 //���þ�̬�ؼ���ʾλͼ
	 pStatic->SetBitmap(hBitmap); 

	CDialog::OnTimer(nIDEvent);
}

void CTurnoutDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	KillTimer(WM_UPDATESIMULATION);
}
