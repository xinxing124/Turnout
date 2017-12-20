// MessageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Turnout.h"
#include "MessageDlg.h"


// CMessageDlg �Ի���

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


// CMessageDlg ��Ϣ�������

void CMessageDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//if(::GetFileAttributes(_T("\\Ӳ��\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
	//	
	//	m_Temp=_T("վ��;�����;����;�����;����ֵ\r\n");
	//}
	//else{
	//	m_Temp=_T("");
	//}
	//m_Temp+=strTemp_01+_T(";")+strTemp_02+_T(";")+strTemp_03+_T(";")+strTemp_04+_T(";")+strTemp_05+_T("\r\n");
	////FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	//FileWrite(_T("\\Ӳ��\\") +strDate +_T(".txt"),m_Temp);
		
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
	GetDlgItem(IDC_STATIC_DIFF)->SetWindowTextW(_T("��:") +theApp.DifferenceValue);

	if(::GetFileAttributes(_T("\\Ӳ��\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
		
		m_Temp=_T("��������;������;��������;�����ɵ�;��׼���;����λ��;��׼ֵ;����ֵ;��׼��\r\n");
	}
	else{
		m_Temp=_T("");
	}
	m_Temp+=theApp.SectionName+_T(";")+theApp.SwitchNumber+_T(";")+theApp.SwitchType+_T(";")+theApp.TrackMeasurement+_T(";")
		+theApp.StandardTurnout+_T(";")+theApp.MeasuringPosition+_T(";")+theApp.StandardValue+_T(";")+theApp.MeasuredValue+_T(";")+theApp.DifferenceValue+_T("\r\n");
	//FileWrite(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),m_Temp);
	FileWrite(_T("\\Ӳ��\\") +strDate +_T(".txt"),m_Temp);

	OnOK();
}

void CMessageDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SYSTEMTIME st;
	CString strDate,strTime;
	GetLocalTime(&st);
	strDate.Format(_T("%4d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	//strTime.Format("%2d:%2d:2d", st.wHour,st.wMinute,st.wSecond);
	/*
	//if(::GetFileAttributes(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
	if(::GetFileAttributes(_T("\\Ӳ��\\") +strDate +_T(".txt")) == 0xFFFFFFFF){
		existfile=false;
		m_Temp=_T("վ��;�����;����;�����;����ֵ\r\n");
	}
	else{
		existfile=true;
		//FileRead(_T("\\ResidentFlash2\\GUI\\") +strDate +_T(".txt"),&m_Temp);
		m_Temp=_T("");
		FileRead(_T("\\Ӳ��\\") +strDate +_T(".txt"),&m_Temp);
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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//����������������	
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
		((CComboBox*)GetDlgItem(IDC_COMBO5))->ResetContent();//����������������
		for(int i=0;i<Count;i++)
		{
			Split(souce[i],dest,_T(";"));
			((CComboBox*)GetDlgItem(IDC_COMBO5))->AddString(dest[0].Mid(1));
			for(int j=0;j<2;j++)
			{
				if( theApp.StandardTurnout.Compare(dest[0].Mid(1))==0)
				{
					((CComboBox*)GetDlgItem(IDC_COMBO6))->ResetContent();//����������������
					((CComboBox*)GetDlgItem(IDC_COMBO7))->ResetContent();//����������������
					if(theApp.SwitchType.Compare(_T("���"))==0)
					{
						Split(dest[1],result,_T(","));
						nCount = result.GetSize();
						for(int j=0;j<nCount;j+=2)
						{
							((CComboBox*)GetDlgItem(IDC_COMBO6))->AddString(result[j]);
							((CComboBox*)GetDlgItem(IDC_COMBO7))->AddString(result[j+1]);
						}
					}
					else if(theApp.SwitchType.Compare(_T("�Ĺ�"))==0)
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
//			((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();//����������������
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
	�磺 //�ؼ����ݳ�ʼ��
 CString strTemp;
 ((CComboBox*)GetDlgItem(IDC_COMBO1))->ResetContent();//����������������
 for(int i=1;i<=100;i++)
 {
  strTemp.Format("%d",i);
  ((CComboBox*)GetDlgItem(IDC_COMBO_CF))->AddString(strTemp);
 }
3��������ʱ�����
�磺 CString strTemp;
 int iCount=((CComboBox*)GetDlgItem(IDC_COMBO_CF))->GetCount();//ȡ��Ŀǰ�Ѿ��е�����
 if(iCount<1)//��ֹ�ظ�������
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

//д���ļ�
int CMessageDlg::FileWrite(CString filename=_T(""),CString content=_T(""))
{
	HANDLE hFile = INVALID_HANDLE_VALUE;	// �ļ���� 
	// ����һ���ļ����һ���ļ�
	hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox(_T("�����ļ�ʧ��!"));
		return -1;
	}

	DWORD len,actlen;
	char *pcharbuff;

	//len = content.GetLength();							// ȡ�������ַ����ĳ���
	len=CStringA(content).GetLength();

	pcharbuff = new char[len];
	LPTSTR pStr = content.GetBuffer(len);					// ȡ�������ַ����Ļ�����ָ��

	// �� Unicode �� CString ת��Ϊ char ��
	WideCharToMultiByte(CP_ACP, 0, pStr, len, pcharbuff, len, NULL, NULL);
	content.ReleaseBuffer();

	BOOL ret = SetFilePointer(hFile, 0, NULL, FILE_END);	// �ƶ��ļ�ָ�뵽�ļ���ͷ
	if (ret == 0xFFFFFFFF)
	{
		//AfxMessageBox(_T("���ļ�ָ�������ļ���ͷʧ��!"));
		delete[] pcharbuff;
		return -2;	
	}

	ret = WriteFile(hFile, pcharbuff, len, &actlen, NULL);	// ������д���ļ��� 
	
	if (pcharbuff != NULL) 
		delete[] pcharbuff;

	CloseHandle(hFile);
	if (ret == TRUE)
		return 0;
		//AfxMessageBox(_T("д�ļ��ɹ�!"));	
	else
		return -3;
		//AfxMessageBox(_T("д�ļ�ʧ��!"));	
}

int CMessageDlg::FileRead(CString filename,CString* content)
{
	int lRet;
	HANDLE hFile = INVALID_HANDLE_VALUE;	// �ļ���� 
	if(::GetFileAttributes(filename) == 0xFFFFFFFF)
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
// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CMessageDlg::OnCbnDropdownCombo2()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			((CComboBox*)GetDlgItem(IDC_COMBO2))->ResetContent();//����������������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
					((CComboBox*)GetDlgItem(IDC_COMBO6))->ResetContent();//����������������
					((CComboBox*)GetDlgItem(IDC_COMBO7))->ResetContent();//����������������
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
						//MessageBox(_T("��ѡ��������ͣ�"));
					}
				}
				else
				{
					//MessageBox(_T("��ѡ���׼��ţ�"));
				}
			}
		}
	}
}

void CMessageDlg::OnCbnCloseupCombo6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}

void CMessageDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	//m_font.DeleteObject();
}

void CMessageDlg::OnCbnSelendokCombo6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	GetDlgItem(IDC_STATIC_DIFF)->SetWindowTextW(_T("��:") +theApp.DifferenceValue);
}

void CMessageDlg::OnCbnEditupdateCombo6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}
