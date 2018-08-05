// MessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Turnout.h"
#include "MessageDlg.h"


// CMessageDlg 对话框

IMPLEMENT_DYNAMIC(CMessageDlg, CDialog)

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
	, existfile(false)
	, m_Temp(_T(""))
{

}

CMessageDlg::~CMessageDlg()
{
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMessageDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMessageDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMessageDlg::OnCbnSelchangeCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CMessageDlg::OnCbnDropdownCombo2)
	ON_WM_DESTROY()
	ON_CBN_DROPDOWN(IDC_COMBO6, &CMessageDlg::OnCbnDropdownCombo6)
	ON_CBN_CLOSEUP(IDC_COMBO6, &CMessageDlg::OnCbnCloseupCombo6)
	ON_CBN_SELENDOK(IDC_COMBO6, &CMessageDlg::OnCbnSelendokCombo6)
	ON_CBN_EDITUPDATE(IDC_COMBO6, &CMessageDlg::OnCbnEditupdateCombo6)
END_MESSAGE_MAP()


// CMessageDlg 消息处理程序

void CMessageDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d%02d%02d"),st.wYear,st.wMonth,st.wDay);

	//CString strTemp_01,strTemp_02,strTemp_03,strTemp_04,strTemp_05;
	//((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(strTemp_01);
	//((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowTextW(strTemp_02);
	//((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowTextW(strTemp_03);
	//((CComboBox*)GetDlgItem(IDC_COMBO4))->GetWindowTextW(strTemp_04);
	//strTemp_05.Format(_T("%.3f"),theApp.Val);
	//
	//if(::GetFileAttributes(_T("\\硬盘\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
	//	
	//	m_Temp=_T("站名;轨枕号;类型;道岔号;测量值\r\n");
	//}
	//else{
	//	m_Temp=_T("");
	//}
	//m_Temp+=strTemp_01+_T(";")+strTemp_02+_T(";")+strTemp_03+_T(";")+strTemp_04+_T(";")+strTemp_05+_T("\r\n");
	////FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	//FileWrite(_T("\\硬盘\\") +strDate +_T(".txt"),m_Temp);
		
		((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(theApp.SectionName);
	((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowTextW(theApp.SwitchNumber);
	((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowTextW(theApp.SwitchType);
	((CComboBox*)GetDlgItem(IDC_COMBO4))->GetWindowTextW(theApp.TrackMeasurement);
	((CComboBox*)GetDlgItem(IDC_COMBO5))->GetWindowTextW(theApp.StandardTurnout);
	((CComboBox*)GetDlgItem(IDC_COMBO6))->GetWindowTextW(theApp.MeasuringPosition);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->GetWindowTextW(theApp.StandardValue);

	
	wchar_t   *stopstring;		
	double dStandard=wcstod(theApp.StandardValue,&stopstring);
	double dValue=theApp.Val-dStandard;
	theApp.MeasuredValue.Format(_T("%.3f"),theApp.Val);
	theApp.DifferenceValue.Format(_T("%.3f"),dValue);
	GetDlgItem(IDC_STATIC_DIFF)->SetWindowTextW(_T("差:") +theApp.DifferenceValue);

	if(::GetFileAttributes(_T("\\硬盘\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
		
		m_Temp=_T("区段名称;道岔编号;道岔类型;测量股道;标准岔号;测量位置;标准值;测量值;标准差\r\n");
	}
	else{
		m_Temp=_T("");
	}
	m_Temp+=theApp.SectionName+_T(";")+theApp.SwitchNumber+_T(";")+theApp.SwitchType+_T(";")+theApp.TrackMeasurement+_T(";")
		+theApp.StandardTurnout+_T(";")+theApp.MeasuringPosition+_T(";")+theApp.StandardValue+_T(";")+theApp.MeasuredValue+_T(";")+theApp.DifferenceValue+_T("\r\n");
	//FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	FileWrite(_T("\\硬盘\\") +strDate +_T(".txt"),m_Temp);

	OnOK();
}

void CMessageDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(theApp.SectionName);
	((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowTextW(theApp.SwitchNumber);
	((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowTextW(theApp.SwitchType);
	((CComboBox*)GetDlgItem(IDC_COMBO4))->GetWindowTextW(theApp.TrackMeasurement);
	((CComboBox*)GetDlgItem(IDC_COMBO5))->GetWindowTextW(theApp.StandardTurnout);
	((CComboBox*)GetDlgItem(IDC_COMBO6))->GetWindowTextW(theApp.MeasuringPosition);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->GetWindowTextW(theApp.StandardValue);

	OnCancel();
}


BOOL CMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	//strTime.Format("%2d:%2d:2d", st.wHour,st.wMinute,st.wSecond);
	/*
	//if(::GetFileAttributes(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
	if(::GetFileAttributes(_T("\\硬盘\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
		existfile=false;
		m_Temp=_T("站名;轨枕号;类型;道岔号;测量值\r\n");
	}
	else{
		existfile=true;
		//FileRead(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),&m_Temp);
		m_Temp=_T("");
		FileRead(_T("\\硬盘\\") +strDate +_T(".txt"),&m_Temp);
	}
	*/
	/*
	CWnd   *pEdit   =   GetDlgItem(IDC_COMBO1);
	CFont   *pfont   =   pEdit->GetFont();
	LOGFONT   logfont;
	pfont->GetLogFont(&logfont);
	logfont.lfHeight   =   -12;
	m_font.CreateFontIndirect(&logfont);
	pEdit->SetFont(&m_font);
	
	
	GetDlgItem(IDC_STATIC01)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC02)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC03)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC04)->SetFont(&m_font);
	
	GetDlgItem(IDOK)->SetFont(&m_font);
	GetDlgItem(IDCANCEL)->SetFont(&m_font);
	*/
	//IDC_STATIC01

	//theApp.FileContent
	//theApp.FileContent.
	int Count;
	int nCount;
	CString strTemp;
	CStringArray souce;
	CStringArray dest;
	CStringArray result;
	Split(theApp.FileContent,souce,_T("}"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//消除现有所有内容	
	Count = souce.GetSize();

	for(int i=0;i<Count;i++)
	{

		//MessageBox(NULL, souce[i], _T(""), MB_OK);
		Split(souce[i],dest,_T(","));
		nCount = dest.GetSize();
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[0].Mid(1));
		if( theApp.SectionName.Compare(dest[0].Mid(1))==0)
		{
			for(int j=1;j<nCount;j++)
			{
				((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(dest[j]);
			}
		}
	}
	
	if(theApp.StandardContent.GetLength()>2)
	{
		Split(theApp.StandardContent,souce,_T("]"));
		Count = souce.GetSize();
		((CComboBox*)GetDlgItem(IDC_COMBO5))->ResetContent();//消除现有所有内容
		for(int i=0;i<Count;i++)
		{
			Split(souce[i],dest,_T(";"));
			((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(dest[0].Mid(1));
			for(int j=0;j<2;j++)
			{
				if( theApp.StandardTurnout.Compare(dest[0].Mid(1))==0)
				{
					((CComboBox*)GetDlgItem(IDC_COMBO6))->ResetContent();//消除现有所有内容
					((CComboBox*)GetDlgItem(IDC_COMBO7))->ResetContent();//消除现有所有内容
					if(theApp.SwitchType.Compare(_T("尖轨"))==0)
					{
						Split(dest[1],result,_T(","));
						nCount = result.GetSize();
						for(int j=0;j<nCount;j+=2)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO6))->AddString(result[j]);
							((CComboBox*)GetDlgItem(IDC_COMBO7))->AddString(result[j+1]);
						}
					}
					else if(theApp.SwitchType.Compare(_T("心轨"))==0)
					{
						Split(dest[2],result,_T(","));
						nCount = result.GetSize();
						for(int j=0;j<nCount;j+=2)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO6))->AddString(result[j]);
							((CComboBox*)GetDlgItem(IDC_COMBO7))->AddString(result[j+1]);
						}
					}
				}
			}
		}
	}

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetWindowTextW(theApp.SectionName);
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetWindowTextW(theApp.SwitchNumber);
	((CComboBox*)GetDlgItem(IDC_COMBO3))->SetWindowTextW(theApp.SwitchType);
	((CComboBox*)GetDlgItem(IDC_COMBO4))->SetWindowTextW(theApp.TrackMeasurement);
	((CComboBox*)GetDlgItem(IDC_COMBO5))->SetWindowTextW(theApp.StandardTurnout);
	((CComboBox*)GetDlgItem(IDC_COMBO6))->SetWindowTextW(theApp.MeasuringPosition);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->SetWindowTextW(theApp.StandardValue);



//nCount = dest.GetSize();
//		
//
//		if(i==((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
//		{
//			((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();//消除现有所有内容
//			for(int j=1;j<nCount;j++)
//			{
//				//((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[j].Mid(1));
//				((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(dest[j]);
//			}
//		}
//
//int Count = possouce.GetSize();
//		int nCount;
//		for(int i=0;i<Count;i++)
//		{	
//			if(possouce[i].Find(sFigureNumber)>0)
//			{
//				Split(possouce[i],posmiddle,_T(";"));
//				Split(posmiddle[lMeasurementTypeIndex+1],posdest,_T(","));
//				TRACE(_T("%s\n"),posmiddle[theApp.AppMeasurementTypeIndex+1]);
//				nCount = posdest.GetSize();
//	
//				wchar_t   *stopstring;
//				theApp.AppFirstSectionStart=(int)wcstol(posdest[lMeasurementPosIndex*4],&stopstring,10);
//				theApp.AppFirstSectionFinish=(int)wcstol(posdest[lMeasurementPosIndex*4+1],&stopstring,10);
//				theApp.AppSecondSectionStart=(int)wcstol(posdest[lMeasurementPosIndex*4+2],&stopstring,10);
//				theApp.AppSecondSectionFinish=(int)wcstol(posdest[lMeasurementPosIndex*4+3],&stopstring,10);
//				break;			
//			}
//		}


//strTemp.Mid(1);

/*
	如： //控件内容初始化
 CString strTemp;
 ((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//消除现有所有内容
 for(int i=1;i<=100;i++)
 {
  strTemp.Format("%d",i);
  ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
 }
3，下拉的时候添加
如： CString strTemp;
 int iCount=((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetCount();//取得目前已经有的行数
 if(iCount<1)//防止重复多次添加
 {
  ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->ResetContent();
  for(int i=1;i<=100;i++)
  {
   strTemp.Format("%d",i);
   ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
  }
 }
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//写入文件
int CMessageDlg::FileWrite(CString filename=_T(""),CString content=_T(""))
{
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	// 创建一个文件或打开一个文件
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("创建文件失败!"));
		return -1;
	}

	DWORD len,actlen;
	char *pcharbuff;

	//len = content.GetLength();							// 取得输入字符串的长度
	len=CStringA(content).GetLength();

	pcharbuff = new char[len];
	LPTSTR pStr = content.GetBuffer(len);					// 取得输入字符串的缓冲区指针

	// 将 Unicode 的 CString 转换为 char 型
	WideCharToMultiByte(CP_ACP, 0, pStr, len, pcharbuff, len, NULL, NULL);
	content.ReleaseBuffer();

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_END);	// 移动文件指针到文件开头
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("将文件指针移至文件开头失败!"));
		delete[] pcharbuff;
		return -2;	
	}

	ret = WriteFile(hFile, pcharbuff, len, &actlen, NULL);	// 将数据写入文件中 
	
	if (pcharbuff != NULL) 
		delete[] pcharbuff;

	CloseHandle(hFile);
	if (ret == TRUE)
		return 0;
		//AfxMessageBox(_T("写文件成功!"));	
	else
		return -3;
		//AfxMessageBox(_T("写文件失败!"));	
}

int CMessageDlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// 文件句柄 
	if(::GetFileAttributes(filename) == 0xFFFFFFFF)
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

void CMessageDlg::Split(CString source, CStringArray& dest, CString division)
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

void CMessageDlg::OnCbnSelchangeCombo2()
{
// TODO: 在此添加控件通知处理程序代码
}

void CMessageDlg::OnCbnDropdownCombo2()
{

	// TODO: 在此添加控件通知处理程序代码
	CStringArray souce;
	CStringArray dest;
	Split(theApp.FileContent,souce,_T("}"));

	CString strTemp;
	int Count = souce.GetSize();
	int nCount;

	for(int i=0;i<Count;i++)
	{
		Split(souce[i],dest,_T(","));
		nCount = dest.GetSize();

		if(i==((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel())
		{
			((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();//消除现有所有内容
			for(int j=1;j<nCount;j++)
			{
				//((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(dest[j].Mid(1));
				((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(dest[j]);
			}
		}
	}

}


void CMessageDlg::OnCbnDropdownCombo6()
{
	// TODO: 在此添加控件通知处理程序代码
	int Count;
	int nCount;
	CString strTemp;
	CStringArray souce;
	CStringArray dest;
	CStringArray result;
	if(theApp.StandardContent.GetLength()>2)
	{

		Split(theApp.StandardContent,souce,_T("]"));
		Count = souce.GetSize();

		for(int i=0;i<Count;i++)
		{
			if(((CComboBox*)GetDlgItem(IDC_COMBO5))->GetCurSel()>=0)
			{
				if(i==((CComboBox*)GetDlgItem(IDC_COMBO5))->GetCurSel())
				{
					((CComboBox*)GetDlgItem(IDC_COMBO6))->ResetContent();//消除现有所有内容
					((CComboBox*)GetDlgItem(IDC_COMBO7))->ResetContent();//消除现有所有内容
					Split(souce[i],dest,_T(";"));
					if(((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel()>=0)
					{
						if(((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel()==0)
						{
							strTemp=dest[1];
						}
						else if(((CComboBox*)GetDlgItem(IDC_COMBO3))->GetCurSel()==1)
						{
							strTemp=dest[2];

						}
						Split(strTemp,result,_T(","));
						nCount = result.GetSize();
						for(int j=0;j<nCount;j+=2)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO6))->AddString(result[j]);
							((CComboBox*)GetDlgItem(IDC_COMBO7))->AddString(result[j+1]);
						}
					}
					else
					{
						//MessageBox(_T("请选择道岔类型！"));
					}
				}
				else
				{
					//MessageBox(_T("请选择标准岔号！"));
				}
			}
		}
	}
}

void CMessageDlg::OnCbnCloseupCombo6()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CMessageDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//m_font.DeleteObject();
}

void CMessageDlg::OnCbnSelendokCombo6()
{
	// TODO: 在此添加控件通知处理程序代码
	int intSel=((CComboBox*)GetDlgItem(IDC_COMBO6))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO7))->SetCurSel(intSel);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->GetWindowTextW(theApp.SectionName);
	((CComboBox*)GetDlgItem(IDC_COMBO2))->GetWindowTextW(theApp.SwitchNumber);
	((CComboBox*)GetDlgItem(IDC_COMBO3))->GetWindowTextW(theApp.SwitchType);
	((CComboBox*)GetDlgItem(IDC_COMBO4))->GetWindowTextW(theApp.TrackMeasurement);
	((CComboBox*)GetDlgItem(IDC_COMBO5))->GetWindowTextW(theApp.StandardTurnout);
	((CComboBox*)GetDlgItem(IDC_COMBO6))->GetWindowTextW(theApp.MeasuringPosition);
	((CComboBox*)GetDlgItem(IDC_COMBO7))->GetWindowTextW(theApp.StandardValue);

	
	wchar_t   *stopstring;		
	double dStandard=wcstod(theApp.StandardValue,&stopstring);
	double dValue=theApp.Val-dStandard;
	theApp.MeasuredValue.Format(_T("%.3f"),theApp.Val);
	theApp.DifferenceValue.Format(_T("%.3f"),dValue);
	GetDlgItem(IDC_STATIC_DIFF)->SetWindowTextW(_T("差:") +theApp.DifferenceValue);
}

void CMessageDlg::OnCbnEditupdateCombo6()
{
	// TODO: 在此添加控件通知处理程序代码

}
