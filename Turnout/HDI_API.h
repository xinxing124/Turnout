
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HDI_API_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HDI_API_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#define GPIO_HIGH			1
#define GPIO_LOW			0
#define GPIO_OUTPUT_CH0 	0
#define GPIO_OUTPUT_CH1 	1
#define GPIO_OUTPUT_CH2 	2
#define GPIO_OUTPUT_CH3 	3
#define GPIO_OUTPUT_CH4 	4
#define GPIO_OUTPUT_CH5 	5
#define GPIO_OUTPUT_CH6 	6
#define GPIO_OUTPUT_CH7 	7

#define GPIO_INPUT_CH0 		0
#define GPIO_INPUT_CH1 		1
#define GPIO_INPUT_CH2 		2
#define GPIO_INPUT_CH3 		3
#define GPIO_INPUT_CH4 		4
#define GPIO_INPUT_CH5 		5
#define GPIO_INPUT_CH6 		6
#define GPIO_INPUT_CH7 		7

#define ROTATION_0			0
#define ROTATION_90			1
#define ROTATION_180		2
#define ROTATION_270		3

#define FRAM				0
#define EEPROM				1

#define PWM_CH0				0
#define PWM_CH1				1
#define PWM_CH2				2
#define PWM_CH3				3

#define PWM_DIV1			1
#define PWM_DIV2			2
#define PWM_DIV4			4
#define PWM_DIV8			8
#define PWM_DIV16			16

#define ADC_CH0				0
#define ADC_CH1				1
#define ADC_CH2				2
#define ADC_CH3				3
#define ADC_CH4				4
#define ADC_CH5				5

#define FM24CL64_SIZE		(8*1024 - 64)
/**************************************************************************
������:API_GPIO_Output
��  ��:����IO�ڵ�ƽ
��  ��:ch,ͨ����,ȡֵ0,1,2,3
       level,1--�ߵ�ƽ;0--�͵�ƽ
����ֵ:TRUE--���óɹ�
       FALSE--����ʧ��
**************************************************************************/
extern "C" _declspec(dllexport)BOOL API_GPIO_Output(UINT8 ch, UINT8 level);

/**************************************************************************
������:API_GPIO_Input
��  ��:��ȡIO�ڵ�ƽ
��  ��:ch,ͨ����,ȡֵ0,1,2,3
       pLevel,����
����ֵ:TRUE--��ȡ�ɹ�
       FALSE--��ȡʧ��
**************************************************************************/
extern "C" _declspec(dllexport)BOOL API_GPIO_Input(UINT8 ch, UINT8* pLevel);

/**************************************************************************
������:API_BEEP_Delay
��  ��:������
��  ��:dwDelay,����ٺ���
����ֵ:��
**************************************************************************/
extern "C" _declspec(dllexport)void API_BEEP_Delay(DWORD dwDelay);

/*****************************************************************************************
������:	API_WDT_Enable
��  ��:�������Ź�
����ֵ:��
*****************************************************************************************/
extern "C" _declspec(dllexport)BOOL API_WDT_Enable(void);


/*****************************************************************************************
������:API_WDT_Disable
��  ��:���ÿ��Ź�
����ֵ:��
*****************************************************************************************/
extern "C" _declspec(dllexport)BOOL API_WDT_Disable(void);



/*****************************************************************************************
������:API_WDT_SetTimeOut
��  ��:���ÿ��Ź���ʱʱ��
��  ��:sec,��λΪ��,���ֵΪ40��
����ֵ:TRUE
       FALSE
*****************************************************************************************/
extern "C" _declspec(dllexport)BOOL API_WDT_SetTimeOut(UINT8 sec);

/*****************************************************************************************
������:API_WDT_Feed
��  ��:ι��
����ֵ:��
˵  ��:��������˿��Ź�����API_WDT_SetTimeOut���õ�Sec���ڲ�ι��������������ϵͳ��
       ��������£���������˿��Ź�������ÿSec��������ι�����������������ϵͳ��
*****************************************************************************************/
extern "C" _declspec(dllexport)BOOL API_WDT_Feed(void);
/****************************************************************************************
���ڿ��Ź���ʹ��˵��:
��1����wince����ʱ�����Ź�Ĭ���ǽ��õ�
��2���������Ź�������������
1.SetWdtTimeOut(sec) -- ��sec����10,20��
2.EnableWdt() -- �������Ź�
3.ÿsec���ڱ���FeedWdt(),���򳬻�����ϵͳ��
��4��������Ź���������DisableWdt()���Խ��ÿ��Ź�����
****************************************************************************************/
extern "C" _declspec(dllexport)BOOL API_PWM_SetDiv(UINT8 ch, UINT8 div);
extern "C" _declspec(dllexport)BOOL API_PWM_Start(UINT8 ch, UINT32 cntb, UINT32 cmpb);
extern "C" _declspec(dllexport)BOOL API_PWM_Stop(UINT8 ch);
extern "C" _declspec(dllexport)BOOL API_PWM_GetFrequency(UINT8 ch, DWORD* pFreq);

extern "C" _declspec(dllexport)BOOL API_ADC_Read(UINT8 ch, UINT16* pVal);

extern "C" _declspec(dllexport)BOOL API_BLK_SetLevel(UINT8 level);
extern "C" _declspec(dllexport)void API_BLK_GetLevel(UINT8* pLevel);

extern "C" _declspec(dllexport)BOOL API_GUI_Rotate(UINT8 rotation, BOOL bIsSaveReg);

extern "C" _declspec(dllexport)void API_HWWDT_Feed(void);
extern "C" _declspec(dllexport)void API_HW_Reboot(void);
extern "C" _declspec(dllexport)void API_HW_GetID(UINT8* pData);
extern "C" _declspec(dllexport)void API_HW_GetIMEI(UINT8* pData);
extern "C" _declspec(dllexport)void API_HW_GetOSBootCnt(DWORD* pCnt);
extern "C" _declspec(dllexport)void API_HW_GetOSLifeTime(DWORD* pCnt);
extern "C" _declspec(dllexport)void API_HW_GetOSCurrLifeTime(DWORD* pCnt);

extern "C" _declspec(dllexport)BOOL API_FRAM_Read(UINT16 Addr,UINT8 *pBuf,UINT32 Lenght);
extern "C" _declspec(dllexport)BOOL API_FRAM_Write(UINT16 Addr,UINT8 *pBuf,UINT32 Lenght);