// TurnoutDlg.cpp : 实现文件
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

//定义串口数据接收消息常量
#define WM_RECV_SERIAL_DATA WM_USER + 101
#define WM_UPDATESIMULATION 1001

// CTurnoutDlg 对话框
//读线程退出事件
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


// CTurnoutDlg 消息处理程序
//线程函数
DWORD CTurnoutDlg::ThreadFunc(LPVOID lparam)
{
	CTurnoutDlg *TurnoutDlg = (CTurnoutDlg*)lparam;
	DWORD dwRet;
	while (TRUE)
	{   	
		//如果收到读线程退出信号，则退出线程
		dwRet=WaitForSingleObject(g_hDetIOEvent,500);
		if (dwRet== WAIT_OBJECT_0)//调试模式
		{
			//::AfxMessageBox(_T("1"));
			//TurnoutDlg->ReadParam();
			::SetFocus(g_hWnd);
			::ShowWindow(g_hWnd,1);
			break;
		}
		if ( dwRet== WAIT_TIMEOUT)//无返回超时
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

//关闭读线程
void CTurnoutDlg::CloseThread()
{
    //等待4秒，如果读线程没有退出，则强制退出
    if (WaitForSingleObject(m_hThread,4000) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThread,0);
	}
	m_hThread = NULL;
}
BOOL CTurnoutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	SetTimer(WM_UPDATESIMULATION, 10000, NULL);

	g_hDetIOEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("DetIOEvent"));

	if(!g_hDetIOEvent)
	{
		//RETAILMSG(1,(TEXT("### Error -- OpenEvent\r\n")));
		//::AfxMessageBox(_T("OpenEvent"));
		return 0;
	}

	g_hWnd=GetDlgItem(IDC_BUTTON5)->m_hWnd; 
	//创建监听线程
	m_hThread = CreateThread(NULL,0,ThreadFunc,this,0,&m_dwThreadID);

	// TODO: 在此添加额外的初始化代码
	Sleep(10000);
	ReadParam();
	//CFont Font;
	//Font.CreatePointFont(200,L"MS Sans Serif");
	//CFont Font;
	//Font.CreatePointFont(20,L"宋体");
	
	int iFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int iFullHeight = GetSystemMetrics(SM_CYSCREEN); 
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, iFullWidth, iFullHeight, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);

	fontTitle.CreatePointFont(200,L"微软雅黑");  
	fontContent.CreatePointFont(280,L"微软雅黑");  
	fontCompany.CreatePointFont(150,L"微软雅黑");  
	

	//GetDlgItem(IDC_STATIC)->SetFont(&Font);
	GetDlgItem(IDC_STATIC1)->SetWindowTextW(_T("一次测量"));
	GetDlgItem(IDC_STATIC2)->SetWindowTextW(_T("0.000"));
	//GetDlgItem(IDC_STATIC1)->SetFont(&Font);
	
	GetDlgItem(IDC_STATIC3)->SetWindowTextW(_T("mm"));
	GetDlgItem(IDC_STATIC4)->SetWindowTextW(_T("尖轨降低值测量仪"));
	//GetDlgItem(IDC_STATIC5)->SetWindowTextW(_T("北京燕宏达铁路设备有限公司"));
	GetDlgItem(IDC_STATIC5)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC8)->SetWindowTextW(_T("UID:")+theApp.UID);
	
	::SetCursorPos(iFullHeight,iFullWidth);
	ShowCursor(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
	// TODO: 在此添加控件通知处理程序代码
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
	//GetDlgItem(IDC_STATIC)->SetWindowTextW(_T("明日科技"));
	//GetDlgItem(IDC_STATIC)->UpdateData(false);
	//this->SetDlgItemTextW(IDC_STATIC,_T("明日科技"));
	//GetDlgItem(IDC_STATIC)->Invalidate();

	//SetDlgItemText(IDC_STATIC,_T("ABC"));
	//GetDlgItem(IDC_STATIC)->SetWindowText(_T("111"));////将内容设置为111 
	//SetDlgItemText(IDC_STATIC,_T("显示内容"));
	m_ShowTitle.Format(_T("一次测量"));
	m_ShowResult.Format(_T("%.3f"),m_OnceVal);
	UpdateData(false);
*/
	m_OnceVal=-99.9;
	m_ShowTitle.Format(_T("一次测量"));
	GatherData();
}

HBRUSH CTurnoutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	switch(pWnd->GetDlgCtrlID())   
	{   
		case IDC_STATIC1: 
		    pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,0,0));    //Static控件1的字体颜色-红色
			pDC->SelectObject(&fontTitle); 
			
			//return (HBRUSH)m_brush1.GetSafeHandle();
		break;   

		case IDC_STATIC2: 
		    pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(0,255,0));    //Static控件2的字体颜色-绿色
			pDC->SelectObject(&fontContent); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;   

		case IDC_STATIC3: 
 			pDC->SetTextColor(RGB(0,0,255)); //Static控件3的字体颜色-蓝色
			pDC->SelectObject(&fontTitle); 
			//pDC->SetBkColor(RGB(255,0,0));   //Static控件3的字体背景颜色-红色
		break;  
		case IDC_STATIC4: 
		    pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(255,255,255));    //Static控件2的字体颜色-绿色
			pDC->SelectObject(&fontContent); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;  
		case IDC_STATIC5: 
		    pDC->SetBkMode(TRANSPARENT); 
			//pDC->SetTextColor(RGB(255,255,255));    //Static控件2的字体颜色-绿色
			pDC->SelectObject(&fontCompany); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;  
		case IDC_STATIC8: 
		    pDC->SetBkMode(TRANSPARENT); 
			pDC->SetTextColor(RGB(199,199,199));    //Static控件2的字体颜色-绿色
			//pDC->SetTextColor(RGB(200,200,200));    //Static控件2的字体颜色-绿色
			pDC->SelectObject(&fontCompany); 
			//return (HBRUSH)m_brush2.GetSafeHandle();   
		break;  
	}  
	GetDlgItem(IDC_BUTTON1)->UpdateWindow();
	GetDlgItem(IDC_BUTTON2)->UpdateWindow();
	GetDlgItem(IDC_BUTTON3)->UpdateWindow();
	GetDlgItem(IDC_BUTTON4)->UpdateWindow();
	GetDlgItem(IDC_BUTTON5)->UpdateWindow();
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CTurnoutDlg::OnBnClickedButton2()
{
	/*
	// TODO: 在此添加控件通知处理程序代码
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

	m_ShowTitle.Format(_T("二次测量"));
	m_ShowResult.Format(_T("%.3f"),m_SecondaryVal);
	UpdateData(false);
*/
	m_ShowTitle.Format(_T("二次测量"));
	m_SecondaryVal=-99.9;
	GatherData();
}

void CTurnoutDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_Result=fabs(m_SecondaryVal-m_OnceVal);
	m_Result=m_OnceVal-m_SecondaryVal;
	//m_Result=-0.0006*pow(m_Result,3.0)+0.0046*pow(m_Result,2.0)+1.0328*pow(m_Result,1.0)-0.0642;
	theApp.Val = m_Result;
	m_ShowTitle.Format(_T("测量结果"));
	m_ShowResult.Format(_T("%.3f"),m_Result);
	UpdateData(false);
}

void CTurnoutDlg::OnBnClickedButton4()
{
	
	// TODO: 在此添加控件通知处理程序代码
	CMessageDlg dlg;
	//m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	
	//OnOK();
}

int CTurnoutDlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	if(::GetFileAttributes(filename)==0xFFFFFFFF)
	{
	//::AfxMessageBox(_T("文件不存在"));
		return -5;
	}
	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("打开文件失败!"));
		return -1;
	}

	DWORD filelen,actlen;
	char *pcharbuff;
	
	filelen = GetFileSize(hFile, NULL);							// 获取文件大小
	if (filelen == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("获取文件大小失败!"));
		return -2;	
	}

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);		// 移动文件指针到文件开头
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("将文件指针移至文件开头失败!"));
		return -3;	
	}

	pcharbuff = new char[filelen];
	ret = ReadFile(hFile, pcharbuff, filelen, &actlen, NULL);	// 从文件中读出数据 
	if (ret == TRUE)
	{
		LPTSTR pStr = content->GetBuffer(filelen);	
		// 将字节转化为 Unicode 字符串
		MultiByteToWideChar(CP_ACP, 0, pcharbuff, filelen, pStr, filelen);
		content->ReleaseBuffer();
		//AfxMessageBox(_T("读文件成功!"));	
		lRet=0;
	}
	else
	{
		//AfxMessageBox(_T("读文件失败!"));	
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
    int i,j;// 循环变量  
    double dTemp;  
      
    // 用冒泡法对数组进行排序  
    for (j = 0; j < iFilterLen ; j ++)  
    {  
        for (i = 0; i < iFilterLen - j; i ++)  
        {  
            if (dArray[i] > dArray[i + 1])  
            {  
                // 互换  
                dTemp = dArray[i];  
                dArray[i] = dArray[i + 1];  
                dArray[i + 1] = dTemp;  
            }  
        }  
    }  
      
    // 计算中值  
    if ((iFilterLen & 1) > 0)  
    {  
        // 数组有奇数个元素，返回中间一个元素  
        dTemp = dArray[(iFilterLen + 1) / 2];  
    }  
    else  
    {  
        // 数组有偶数个元素，返回中间两个元素平均值  
        dTemp = (dArray[iFilterLen / 2] + dArray[iFilterLen / 2 + 1]) / 2;  
    }  
  
    return dTemp;  
}  

//定义串口接收数据函数类型
void CALLBACK CTurnoutDlg::OnSerialRead(void * pOwner,BYTE* buf,DWORD bufLen)
{
	BYTE *pRecvBuf = NULL; //接收缓冲区
	//得到父对象指针
	CTurnoutDlg* pThis = (CTurnoutDlg*)pOwner;
	//将接收的缓冲区拷贝到pRecvBuf种
	pRecvBuf = new BYTE[bufLen];
	CopyMemory(pRecvBuf,buf,bufLen);

	//发送异步消息，表示收到串口数据，消息处理完，应释放内存
	pThis->PostMessage(WM_RECV_SERIAL_DATA,WPARAM(pRecvBuf),bufLen);

}

// 串口接收数据处理函数
LONG CTurnoutDlg::OnRecvSerialData(WPARAM wParam,LPARAM lParam)
{
	CString strOldRecv = L"";
	CString strRecv = L"";
	//串口接收到的BUF
	CHAR *pBuf = (CHAR*)wParam;
	//Wendu=*pBuf;
	//串口接收到的BUF长度
	DWORD dwBufLen = lParam;
	
	//接收框
	//CEdit *pEdtRecvMsg = (CEdit*)GetDlgItem(IDC_STATIC2);
	//ASSERT(pEdtRecvMsg != NULL);

	//得到接收框中的历史文本
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
	
	//将新接收到的文本添加到接收框中
	strOldRecv = strOldRecv + strRecv;
	
	//pEdtRecvMsg->SetWindowTextW(strRecv);

	//释放内存
	delete[] pBuf;
	pBuf = NULL;

	//m_ShowResult.Format(_T("%s_%f"),strRecv,t);
	m_ShowResult.Format(_T("%.3f"),nTmp);
	UpdateData(false);

	//关闭串口
	//
	if (m_pSerial != NULL)
	{
		//关闭串口
		m_pSerial->ClosePort();

		//释放串口对象
		delete m_pSerial;
		m_pSerial = NULL;
		//AfxMessageBox(L"串口关闭成功");
	}


	return 0;
}
int CTurnoutDlg::GatherData(void)
{
	
	//判断串口是否已经打开
	if (m_pSerial != NULL)
	{
		m_pSerial->ClosePort();

		delete m_pSerial;
		m_pSerial = NULL;
	}
	
	//新建串口通讯对象
	m_pSerial = new CCESeries();
	m_pSerial->m_OnSeriesRead = OnSerialRead; //

	//打开串口
	if(m_pSerial->OpenPort(this,1,9600,0,8,0))
	{
		//AfxMessageBox(L"串口打开成功");
		Sleep(200);
	}
	else
	{
		AfxMessageBox(L"串口打开失败");
	}


	
	//发送数据
	char * buf  =NULL;  //定义发送缓冲区
	DWORD dwBufLen = 0;   //定义发送缓冲区长度
	CString strSend = L"#010\r";

	//串口如果没有打开，直接返回
	if (m_pSerial == NULL)
	{
		AfxMessageBox(L"请先打开串口");
		return -1;
	}
	
	//将待发送的字符串转换成单字节，进行发送
	buf = new char[strSend.GetLength()*2+1];
	ZeroMemory(buf,strSend.GetLength()*2+1);
	//转换成单字节进行发送	
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,strSend.GetBuffer(strSend.GetLength())
	   ,strSend.GetLength(),buf,strSend.GetLength()*2,NULL,NULL);

	dwBufLen = strlen(buf) + 1;

	//发送字符串
	m_pSerial->WriteSyncPort((BYTE*)buf,dwBufLen);

	//释放内存
	delete[] buf;
	buf = NULL;

	return 0;
}

void CTurnoutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//OnOK();
	CDialog::OnLButtonUp(nFlags, point);
}

void CTurnoutDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
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
		if(::GetFileAttributes(_T("\\硬盘\\parameter.txt")) != 0xFFFFFFFF)
			::CopyFile(_T("\\硬盘\\parameter.txt"),ReturnPath()  +_T("parameter.txt"),false);
	//}
	//if(::GetFileAttributes(_T("\\ResidentFlash2\\GUI\\xishu.txt")) == 0xFFFFFFFF)
	//{
		//if(::GetFileAttributes(_T("\\硬盘\\xishu.txt")) != 0xFFFFFFFF)
		//	::CopyFile(_T("\\硬盘\\xishu.txt"),_T("\\ResidentFlash2\\GUI\\xishu.txt"),false);
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
	// TODO: 在此添加控件通知处理程序代码
}

void CTurnoutDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	 }
	 
	 if(dBattery>=4.204&&dBattery<4.47)
	 {
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	 }
	*/
	 if(dBattery>=3.75)//4.20
	 {
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	 }else if(dBattery>=3.48&&dBattery<3.75)//if(dBattery>=3.93&&dBattery<4.20)
	 {
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
	 }else if(dBattery>=3.22&&dBattery<3.48)//if(dBattery>=3.93&&dBattery<4.20)
	 {
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
	 }else if(dBattery>=2.95&&dBattery<3.22)//if(dBattery>=3.66&&dBattery<3.93)
	 {
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
	 }else if(dBattery<2.95)////if(dBattery<3.66)
	 {
		//获得位图句柄
		hBitmap=::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
	 }
	 //设置静态控件的样式，使其可以使用位图，并试位标显示使居中
	 pStatic->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
	 //设置静态控件显示位图
	 pStatic->SetBitmap(hBitmap); 

	CDialog::OnTimer(nIDEvent);
}

void CTurnoutDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(WM_UPDATESIMULATION);
}
