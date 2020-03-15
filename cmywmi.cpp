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

    //一、初始化COM组件
    //初始化COM
    hr=::CoInitializeEx(0,COINIT_MULTITHREADED);
    if (SUCCEEDED(hr) || RPC_E_CHANGED_MODE == hr)
    {
        //设置进程的安全级别，（调用COM组件时在初始化COM之后要调用CoInitializeSecurity设置进程安全级别，否则会被系统识别为病毒）
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

        //二、创建一个WMI命名空间连接
        //创建一个CLSID_WbemLocator对象
        hr=CoCreateInstance(CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            (LPVOID*)&m_pWbemLoc);
        VERIFY(SUCCEEDED(hr));

        //使用m_pWbemLoc连接到"root\cimv2"并设置m_pWbemSvc的指针
        hr=m_pWbemLoc->ConnectServer(CComBSTR(L"ROOT\\CIMV2"),
            NULL,
            NULL,
            0,
            NULL,
            0,
            0,
            &m_pWbemSvc);
        VERIFY(SUCCEEDED(hr));

        //三、设置WMI连接的安全性
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
        //查询类ClassName中的所有字段,保存到m_pEnumObj中
        query+=CComBSTR(ClassName.c_str());
        hr=m_pWbemSvc->ExecQuery(CComBSTR("WQL"),query,WBEM_FLAG_FORWARD_ONLY|WBEM_FLAG_RETURN_IMMEDIATELY,
            0,&m_pEnumObj);
        if (SUCCEEDED(hr))
        {
            //初始化vtProp值
            VariantInit(&vtProp);
            uReturn=0;

            //返回从当前位置起的第一个对象到m_pWbemObj中
            hr=m_pEnumObj->Next(WBEM_INFINITE,1,&m_pWbemObj,&uReturn);
            if(SUCCEEDED(hr)&&uReturn>0)
            {
                //从m_pWbemObj中找出ClassMember标识的成员属性值,并保存到vtProp变量中
                hr=m_pWbemObj->Get(CComBSTR(ClassMember.c_str()),0,&vtProp,0,0);
                if (SUCCEEDED(hr))
                {
                    VariantToString(&vtProp,chRetValue);
                    VariantClear(&vtProp);//清空vtProp
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
            VariantInit(&vtProp); //初始化vtProp变量
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
                _stprintf(buf, _T("是"));
            else
                _stprintf(buf, _T("否"));
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
{//启动Windows管理体系结构(WMI)
    char* MyCommand="cmd /c \"wmimgmt.msc\"";
    WinExec(MyCommand,SW_HIDE);
}

void GetProcessInfo()
{//获取指定进程的详细进程信息
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
{//获取指定进程的进程简要信息
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
{//获取指定进程的执行路径信息
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
{//获取所有进程的执行路径信息
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
{//将获取的进程信息保存为XML文件
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
{//获取所有进程的内存使用信息
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
{//获取所有进程的统计数据信息
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
{//获取所有进程的IO数据信息
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
{//获取BIOS的名称和制造商信息
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
{//获取CDROM所在的驱动器名称
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
{//获取计算机系统的角色信息
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
{//获取CPU处理器的ID号码信息
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
{//获取计算机的DOCM程序名称信息
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
{//获取磁盘(非逻辑分区)的容量大小
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
{//获取直接内存访问(DMA)频道信息
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
{//获取当前系统的所有环境变量及值
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
{//获取系统的组成员账户及其职责
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
{//获取当前系统的IDE控制器信息
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
{//获取当前系统的逻辑分区卷序列号
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
{//获取当前系统的缓存管理信息
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
{//获取当前系统的虚拟内存信息
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
{//获取当前系统的物理内存信息
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
{//获取当前系统的网络客户端信息
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
{//获取用户最近登录系统的日期信息
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
{//获取当前操作系统的安装日期
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
{//获取当前操作系统的安装包管理信息
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
{//获取当前系统已经安装的软件信息
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
{//获取系统已经安装的声音设备信息
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
{//获取系统启动时自动运行的命令信息
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
{//获取当前系统的系统插槽信息
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
{//获取当前系统设置的用户账户信息
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
{//使用COM接口调用WMI查询数据信息
    string MyData="CPU的描述信息是：";
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
{//获取当前计算机CPU的名称信息
    string MyData="当前计算机CPU的名称是：";
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
{//获取当前计算机CPU的地址宽度
    string MyData="当前计算机CPU的地址宽度是：";
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
{//获取当前计算机CPU的数据宽度
    string MyData="当前计算机CPU的数据宽度是：";
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
{//获取当前计算机CPU的当前时钟速度
    string MyData="当前计算机CPU的当前时钟速度是：";
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
{//获取当前计算机CPU的最大时钟速度
    string MyData="当前计算机CPU的最大时钟速度是：";
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
{//获取当前计算机CPU的外部时钟速度
    string MyData="当前计算机CPU的外部时钟速度是：";
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
{//获取当前计算机CPU的加载百分比
    string MyData="当前计算机CPU的加载百分比是：";
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
{//获取当前计算机CPU的设备制造商
    string MyData="当前计算机CPU的设备制造商是：";
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
{//获取当前计算机CPU的二级缓存尺寸
    string MyData="当前计算机CPU的二级缓存尺寸是：";
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
{//获取当前计算机CPU的二级缓存速度
    string MyData="当前计算机CPU的二级缓存速度是：";
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
