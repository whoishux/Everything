#include "cmywmi.h"
#include "ccommon.h"
#include "wincommon.h"

#ifdef M_MSVC

#include <Windows.h>
#include <shellapi.h>

#define VERIFY(b)   LogPos(!b)

CMyWMI::CMyWMI(void)
{
    m_pWbemSvc=NULL;
    m_pWbemLoc=NULL;
    m_pWbemObj = NULL;
}

CMyWMI::~CMyWMI(void)
{
    m_pWbemSvc=NULL;
    m_pWbemLoc=NULL;
    m_pWbemObj = NULL;
}

HRESULT CMyWMI::InitializeComInterface()
{
    HRESULT hr;

    //һ����ʼ��COM���
    //��ʼ��COM
    hr=::CoInitializeEx(0,COINIT_MULTITHREADED);
    if (SUCCEEDED(hr) || RPC_E_CHANGED_MODE == hr)
    {
        //���ý��̵İ�ȫ���𣬣�����COM���ʱ�ڳ�ʼ��COM֮��Ҫ����CoInitializeSecurity���ý��̰�ȫ���𣬷���ᱻϵͳʶ��Ϊ������
        hr=CoInitializeSecurity(NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_PKT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE,
            NULL);
        //VERIFY(SUCCEEDED(hr));

        //��������һ��WMI�����ռ�����
        //����һ��CLSID_WbemLocator����
        hr=CoCreateInstance(CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            (LPVOID*)&m_pWbemLoc);
        VERIFY(SUCCEEDED(hr));

        //ʹ��m_pWbemLoc���ӵ�"root\cimv2"������m_pWbemSvc��ָ��
        hr=m_pWbemLoc->ConnectServer(CComBSTR(L"ROOT\\CIMV2"),
            NULL,
            NULL,
            0,
            NULL,
            0,
            0,
            &m_pWbemSvc);
        VERIFY(SUCCEEDED(hr));

        //��������WMI���ӵİ�ȫ��
        hr=CoSetProxyBlanket(m_pWbemSvc,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE);
        VERIFY(SUCCEEDED(hr));

    }
    return(hr);
}

HRESULT CMyWMI::ReleaseComInterface()
{
    HRESULT hr;

   if (NULL != m_pWbemSvc)
   {
       hr=m_pWbemSvc->Release();
   }
   if (NULL != m_pWbemLoc)
   {
       hr=m_pWbemLoc->Release();
   }
   if (NULL != m_pWbemObj)
   {
       hr=m_pWbemObj->Release();
   }

   ::CoUninitialize();

   return(hr);
}

BOOL CMyWMI::GetSingleItemInfo(string ClassName, string ClassMember, string &chRetValue)
{
    USES_CONVERSION;

    CComBSTR query("SELECT * FROM ");
    VARIANT vtProp;
    ULONG uReturn;
    HRESULT hr;
    BOOL bRet = FALSE;
    TCHAR buf[1024] = {0};

    if (NULL != m_pWbemSvc)
    {
        //��ѯ��ClassName�е������ֶ�,���浽m_pEnumObj��
        query+=CComBSTR(ClassName.c_str());
        hr=m_pWbemSvc->ExecQuery(CComBSTR("WQL"),query,WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            0,&m_pEnumObj);
        if (SUCCEEDED(hr))
        {
            //��ʼ��vtPropֵ
            VariantInit(&vtProp);
            uReturn=0;

            //���شӵ�ǰλ����ĵ�һ������m_pWbemObj��
            hr=m_pEnumObj->Next(WBEM_INFINITE,1,&m_pWbemObj,&uReturn);
            if(SUCCEEDED(hr)&&uReturn>0)
            {
                //��m_pWbemObj���ҳ�ClassMember��ʶ�ĳ�Ա����ֵ,�����浽vtProp������
                hr=m_pWbemObj->Get(CComBSTR(ClassMember.c_str()),0,&vtProp,0,0);
                if (SUCCEEDED(hr))
                {
                    VariantToString(&vtProp,chRetValue);
                    VariantClear(&vtProp);//���vtProp
                    bRet = TRUE;
                }
            }
        }
    }
    if(NULL != m_pEnumObj)
    {
        hr=m_pEnumObj->Release();
        m_pEnumObj = NULL;
    }
    if(NULL != m_pWbemObj)
    {
        hr=m_pWbemObj->Release();
        m_pWbemObj = NULL;
    }
    return bRet;
}

BOOL CMyWMI::GetGroupItemInfo(string ClassName, string ClassMember[], int n, string &chRetValue)
{
    USES_CONVERSION;

    CComBSTR query("SELECT * FROM ");
    string result,info;
    VARIANT vtProp;
    ULONG uReturn;
    HRESULT hr;
    int i;
    BOOL bRet = FALSE;
    if (NULL  != m_pWbemSvc)
    {
        query+=CComBSTR(ClassName.c_str());
        hr=m_pWbemSvc->ExecQuery(CComBSTR("WQL"),query,WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,0,&m_pEnumObj);
        if (SUCCEEDED(hr))
        {
            VariantInit(&vtProp); //��ʼ��vtProp����
            if(m_pEnumObj)
            {
                Sleep(10);
                uReturn=0;
                hr=m_pEnumObj->Next(WBEM_INFINITE,1,&m_pWbemObj,&uReturn);
                if (SUCCEEDED(hr) &&uReturn>0)
                {
                    for(i=0;i<n;++i)
                    {
                        hr=m_pWbemObj->Get(CComBSTR(ClassMember[i].c_str()),0,&vtProp,0,0);
                        if (SUCCEEDED(hr))
                        {
                            VariantToString(&vtProp,info);
                            chRetValue+=info+("\t");
                            VariantClear(&vtProp);
                            bRet = TRUE;
                        }
                    }
                    chRetValue+=("\r\n");
                }
            }
        }
    }

    if(NULL != m_pEnumObj)
    {
        hr=m_pEnumObj->Release();
        m_pEnumObj=NULL;
    }
    if(NULL != m_pWbemObj)
    {
        hr=m_pWbemObj->Release();
        m_pWbemObj=NULL;
    }
    return bRet;
}

string CMyWMI::GetQueryData(string MyWQL, string MyItem)
{
    USES_CONVERSION;
    TCHAR buf[1024] = {0};
    CharToTchar(MyWQL.c_str(), buf);

    CComBSTR MyQuery=CComBSTR(buf);
    string MyData;
    VARIANT MyVar;
    ULONG MyReturn;
    HRESULT MyHResult;
    m_pEnumObj=0;
    m_pWbemObj=0;
    MyHResult=m_pWbemSvc->ExecQuery(CComBSTR("WQL"),MyQuery,WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,0,&m_pEnumObj);
    VariantInit(&MyVar);
    MyReturn=0;
    MyHResult=m_pEnumObj->Next(WBEM_INFINITE,1,&m_pWbemObj,&MyReturn);
    if(SUCCEEDED(MyHResult)&&MyReturn>0){
        CharToTchar(MyItem.c_str(), buf);
        MyHResult=m_pWbemObj->Get(CComBSTR(buf),0,&MyVar,0,0);
    }else
        return string("");
    VariantToString(&MyVar, MyData);
    VariantClear(&MyVar);
    if(m_pEnumObj)
        MyHResult=m_pEnumObj->Release();
    if(m_pWbemObj)
        MyHResult=m_pWbemObj->Release();
    m_pEnumObj=0;
    m_pWbemObj=0;
    return(MyData);
}

void CMyWMI::VariantToString(const LPVARIANT pVar, string &chRetValue) const
{
    USES_CONVERSION;
    CComBSTR HUGEP* pMyBstr;
    BYTE HUGEP* pMyBuf;
    string MyTip;
    LONG iMyLow,iMyHigh;
    HRESULT MyHResult;
    TCHAR buf[1024] = {0};
    char cBuf[1024] = {0};
    int sz = 0;

    switch(pVar->vt)
    {
        case VT_BSTR:
        printf("VT_BSTR\n");
            _stprintf(buf, _T("%s"), pVar->bstrVal);
            break;
        case VT_BOOL:
        printf("VT_BOOL\n");
            if(VARIANT_TRUE==pVar->boolVal)
                _stprintf(buf, _T("��"));
            else
                _stprintf(buf, _T("��"));
            break;
        case VT_I4:
        printf("VT_I4\n");
            _stprintf(buf, _T("%d"), pVar->lVal);
            break;
        case VT_UI1:
        printf("VT_UI1\n");
            _stprintf(buf, _T("%d"), pVar->bVal);
            break;
        case VT_UI4:
        printf("VT_UI4\n");
            _stprintf(buf, _T("%d"), pVar->ulVal);
            break;
        case VT_BSTR|VT_ARRAY:
        printf("VT_BSTR|VT_ARRAY\n");
            MyHResult=SafeArrayAccessData(pVar->parray,(void HUGEP**)&pMyBstr);
            MyHResult=SafeArrayUnaccessData(pVar->parray);
            _stprintf(buf, _T("%s"), pMyBstr->m_str);
            break;
        case VT_I4|VT_ARRAY:
        printf("VT_I4|VT_ARRAY\n");
            SafeArrayGetLBound(pVar->parray,1,&iMyLow);
            SafeArrayGetUBound(pVar->parray,1,&iMyHigh);
            MyHResult=SafeArrayAccessData(pVar->parray,(void HUGEP**)&pMyBuf);
            MyHResult=SafeArrayUnaccessData(pVar->parray);
            iMyHigh=min(iMyHigh,MAX_PATH*2-1);

            for(int i=iMyLow;i<=iMyHigh;++i)
            {
                sz += _stprintf(buf+sz, _T("%02X"), pMyBuf[i]);
            }
            break;
        default:
            break;
    }

    TcharToChar(buf, cBuf);
    chRetValue = string(cBuf);
}


void StartWMI()
{//����Windows������ϵ�ṹ(WMI)
    char* MyCommand="cmd /c \"wmimgmt.msc\"";
    WinExec(MyCommand,SW_HIDE);
}

void GetProcessInfo()
{//��ȡָ�����̵���ϸ������Ϣ
    char* MyCommand="cmd /c wmic process WHERE Name='Notepad.exe' list FULL > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb= _T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetProcessInfo2()
{//��ȡָ�����̵Ľ��̼�Ҫ��Ϣ
    char* MyCommand="cmd /c wmic process WHERE Name='Notepad.exe' list Brief > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetProcessPath()
{//��ȡָ�����̵�ִ��·����Ϣ
    char* MyCommand="cmd /c wmic process WHERE Name='Notepad.exe' Get ExecutablePath > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetAllProcessPath()
{//��ȡ���н��̵�ִ��·����Ϣ
    char* MyCommand="cmd /c wmic process  get caption, ExecutablePath > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetAllProcessInfo()
{//����ȡ�Ľ�����Ϣ����ΪXML�ļ�
    char* MyCommand="cmd /c  wmic process list /format:XML  > d:\\MyProc.xml";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyProc.xml");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetAllProcessMemInfo()
{//��ȡ���н��̵��ڴ�ʹ����Ϣ
    char* MyCommand="cmd /c wmic process list Memory > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(10000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetAllProcessStatInfo()
{//��ȡ���н��̵�ͳ��������Ϣ
    char* MyCommand="cmd /c wmic process list statistics > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(15000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetAllProcessIOInfo()
{//��ȡ���н��̵�IO������Ϣ
    char* MyCommand="cmd /c wmic process list IO > d:\\MyProc.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(15000);
    TCHAR *MyFile = _T("d:\\MyProc.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetBIOSInfo()
{//��ȡBIOS�����ƺ���������Ϣ
    char* MyCommand="cmd /c wmic bios get name,Manufacturer /value  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetCDRomDiversName()
{//��ȡCDROM���ڵ�����������
    char* MyCommand="cmd /c wmic CDROM get Drive /value  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSystemUserInfo()
{//��ȡ�����ϵͳ�Ľ�ɫ��Ϣ
    char* MyCommand="cmd /c wmic COMPUTERSYSTEM GET Roles  /value > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetCpuId()
{//��ȡCPU��������ID������Ϣ
    char* MyCommand="cmd /c wmic CPU GET ProcessorID  /value > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetDOCMInfo()
{//��ȡ�������DOCM����������Ϣ
    char* MyCommand="cmd /c wmic DCOMAPP GET Name  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(5000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetDiskSize()
{//��ȡ����(���߼�����)��������С
    char* MyCommand="cmd /c wmic diskdrive GET Name,size /value  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetDMAInfo()
{//��ȡֱ���ڴ����(DMA)Ƶ����Ϣ
    char* MyCommand="cmd /c wmic dmachannel list brief  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetEnvironment()
{//��ȡ��ǰϵͳ�����л���������ֵ
    char* MyCommand="cmd /c wmic environment get Name,variablevalue  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(5000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSystemGroupAccount()
{//��ȡϵͳ�����Ա�˻�����ְ��
    char* MyCommand="cmd /c wmic group list full  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSystemIDEInfo()
{//��ȡ��ǰϵͳ��IDE��������Ϣ
    char* MyCommand="cmd /c wmic IDECONTROLLER list full  > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetLogicalDistId()
{//��ȡ��ǰϵͳ���߼����������к�
    char* MyCommand="cmd /c wmic logicalDisk get  name,volumeserialnumber /value> d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(8000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSystemCacheInfo()
{//��ȡ��ǰϵͳ�Ļ��������Ϣ
    char* MyCommand="cmd /c wmic memcache list full > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetVirtualMemInfo()
{//��ȡ��ǰϵͳ�������ڴ���Ϣ
    char* MyCommand="cmd /c wmic memlogical get totalvirtualmemory /value > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetPhyMemInfo()
{//��ȡ��ǰϵͳ�������ڴ���Ϣ
    char* MyCommand="cmd /c wmic memlogical get totalphysicalmemory /value > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetNetClientInfo()
{//��ȡ��ǰϵͳ������ͻ�����Ϣ
    char* MyCommand="cmd /c wmic netclient list full > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetLastDateTime()
{//��ȡ�û������¼ϵͳ��������Ϣ
    char* MyCommand="cmd /c wmic netlogin get name,lastlogon /value> d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSetupDateTime()
{//��ȡ��ǰ����ϵͳ�İ�װ����
    char* MyCommand="cmd /c wmic os get installdate /value> d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(2000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSetupPackageInfo()
{//��ȡ��ǰ����ϵͳ�İ�װ��������Ϣ
    char* MyCommand="cmd /c wmic product list full> d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(5000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSetupSoftWareInfo()
{//��ȡ��ǰϵͳ�Ѿ���װ�������Ϣ
    char* MyCommand="cmd /c wmic softwarefeature list full > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(10000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSoundDevInfo()
{//��ȡϵͳ�Ѿ���װ�������豸��Ϣ
    char* MyCommand="cmd /c wmic Sounddev list full > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetAutoRunCmd()
{//��ȡϵͳ����ʱ�Զ����е�������Ϣ
    char* MyCommand="cmd /c wmic startup get command > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSystemSlotInfo()
{//��ȡ��ǰϵͳ��ϵͳ�����Ϣ
    char* MyCommand="cmd /c wmic SYSTEMSLOT get tag,SlotDesignation /value > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}

void GetSetAccount()
{//��ȡ��ǰϵͳ���õ��û��˻���Ϣ
    char* MyCommand="cmd /c wmic useraccount get name > d:\\MyWMIC.txt";
    WinExec(MyCommand,SW_HIDE);
    Sleep(1000);
    TCHAR *MyFile = _T("d:\\MyWMIC.txt");
    SHELLEXECUTEINFO MyExe;
    memset (&MyExe, 0, sizeof(MyExe)) ;
    MyExe.cbSize=sizeof (MyExe) ;
    MyExe.lpVerb=_T("open") ;
    MyExe.lpFile=MyFile;
    MyExe.lpParameters=NULL;
    MyExe.fMask=SEE_MASK_NOCLOSEPROCESS ;
    MyExe.nShow=SW_SHOWDEFAULT ;
    ShellExecuteEx (&MyExe);
}


void WmiTest()
{//ʹ��COM�ӿڵ���WMI��ѯ������Ϣ
    string MyData="CPU��������Ϣ�ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","Description");
    if(!MyTip.empty())
    {
        printf("%s%s\n", MyData.c_str(), MyTip.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuInfo()
{//��ȡ��ǰ�����CPU��������Ϣ
    string MyData="��ǰ�����CPU�������ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","Name");
    if(!MyTip.empty())
    {
        printf("%s%s\n", MyData.c_str(), MyTip.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuAddrWidth()
{//��ȡ��ǰ�����CPU�ĵ�ַ���
    string MyData="��ǰ�����CPU�ĵ�ַ����ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","AddressWidth");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuDataWidth()
{//��ȡ��ǰ�����CPU�����ݿ��
    string MyData="��ǰ�����CPU�����ݿ���ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","DataWidth");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuCurrentClockSpeed()
{//��ȡ��ǰ�����CPU�ĵ�ǰʱ���ٶ�
    string MyData="��ǰ�����CPU�ĵ�ǰʱ���ٶ��ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","CurrentClockSpeed");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuMaxClockSpeed()
{//��ȡ��ǰ�����CPU�����ʱ���ٶ�
    string MyData="��ǰ�����CPU�����ʱ���ٶ��ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","MaxClockSpeed");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuOutClockSpeed()
{//��ȡ��ǰ�����CPU���ⲿʱ���ٶ�
    string MyData="��ǰ�����CPU���ⲿʱ���ٶ��ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","ExtClock");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuLoadPercent()
{//��ȡ��ǰ�����CPU�ļ��ذٷֱ�
    string MyData="��ǰ�����CPU�ļ��ذٷֱ��ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","LoadPercentage");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuMaker()
{//��ȡ��ǰ�����CPU���豸������
    string MyData="��ǰ�����CPU���豸�������ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","Manufacturer");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuSecondCache()
{//��ȡ��ǰ�����CPU�Ķ�������ߴ�
    string MyData="��ǰ�����CPU�Ķ�������ߴ��ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","L2CacheSize");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

void WmiGetCpuSecondCacheSpeed()
{//��ȡ��ǰ�����CPU�Ķ��������ٶ�
    string MyData="��ǰ�����CPU�Ķ��������ٶ��ǣ�";
    CMyWMI MyWmi;
    MyWmi.InitializeComInterface();
    string MyTip=MyWmi.GetQueryData("SELECT * FROM Win32_Processor","L2CacheSpeed");
    if(!MyTip.empty())
    {
        MyData+=MyTip+"\r\n";
        printf("%s\n", MyData.c_str());
    }
    MyWmi.ReleaseComInterface();
}

#endif
