#ifndef CMYWMI_H
#define CMYWMI_H

#include "cinclude.h"

#ifdef M_MSVC

//#include <afxpriv.h>
#include <WbemIdl.h>
#include <atlbase.h>

#include <string>

using namespace std;

#pragma comment(lib,"WbemUuid.lib")

#define FormatString(s, f, v)   \
{  \
    char buf[1024] = {0};   \
    sprintf(buf, f, v);  \
    s = string(buf);    \
}

class CMyWMI
{

public:
    CMyWMI(void);
    ~CMyWMI(void);

    HRESULT InitializeComInterface();
    HRESULT ReleaseComInterface();

    BOOL GetSingleItemInfo(string ClassName, string ClassMember, string&chRetValue);
    BOOL GetGroupItemInfo(string ClassName, string ClassMember[], int n, string &chRetValue);

    string GetQueryData(string MyWQL, string MyItem);


private:
    void VariantToString(const LPVARIANT, string &chRetValue) const;//将Variant类型的变量转换为string

private:
    IEnumWbemClassObject* m_pEnumObj;
    IWbemClassObject* m_pWbemObj;
    IWbemServices* m_pWbemSvc;
    IWbemLocator* m_pWbemLoc;
};

void StartWMI();
void GetProcessInfo();
void GetProcessInfo2();
void GetProcessPath();
void GetAllProcessPath();
void GetAllProcessInfo();
void GetAllProcessMemInfo();
void GetAllProcessStatInfo();
void GetAllProcessIOInfo();
void GetBIOSInfo();
void GetCDRomDiversName();
void GetSystemUserInfo();
void GetCpuId();
void GetDOCMInfo();
void GetDiskSize();
void GetDMAInfo();
void GetEnvironment();
void GetSystemGroupAccount();
void GetSystemIDEInfo();
void GetLogicalDistId();
void GetSystemCacheInfo();
void GetVirtualMemInfo();
void GetPhyMemInfo();
void GetNetClientInfo();
void GetLastDateTime();
void GetSetupDateTime();
void GetSetupPackageInfo();
void GetSetupSoftWareInfo();
void GetSoundDevInfo();
void GetAutoRunCmd();
void GetSystemSlotInfo();
void GetSetAccount();
void WmiTest();
void WmiGetCpuInfo();
void WmiGetCpuAddrWidth();

void WmiGetCpuDataWidth();
void WmiGetCpuCurrentClockSpeed();
void WmiGetCpuMaxClockSpeed();
void WmiGetCpuOutClockSpeed();
void WmiGetCpuLoadPercent();
void WmiGetCpuMaker();
void WmiGetCpuSecondCache();
void WmiGetCpuSecondCacheSpeed();

#endif

#endif // CMYWMI_H
