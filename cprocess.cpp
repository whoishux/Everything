/******************************************************
* Program/Assignment:   ������ز�����;
* Author:               Hux
* Date:                 2019/12/24
* Description:          ��ǰֻʵ��Windowsϵͳ��ز�����Linuxϵͳδʵ��;
* *****************************************************/


#include "ccommon.h"
#include "cprocess.h"


#include <stdio.h>
#include <stdlib.h>

#include "wincommon.h"

#ifdef M_GNU
#include <sys/types.h>
#include <unistd.h>
#include <windows.h>
#else
#include <Windows.h>
#include <tchar.h>
#include <ShlObj.h>
#include <TlHelp32.h>
#include <Psapi.h>


#define ProcessBasicInformation 0

typedef struct
{
    DWORD ExitStatus;
    DWORD PebBaseAddress;
    DWORD AffinityMask;
    DWORD BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
}   PROCESS_BASIC_INFORMATION;

typedef LONG(__stdcall *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);

/**
1����������
            BOOL CreateProcess(
                    PCTSTR      psApplicationName, //��ִ���ļ�������
                    PTSTR      pszCommandLine, //�������ַ���
                    PSECURITY_ATTRIBUTES psaProcess,   //���̶���İ�ȫ��
                    PSECURITY_ATTRIBUTES psaThread,   //�̶߳���İ�ȫ��
                    BOOL      bInheritHandles, //����ɼ̳���
                    DWORD      fdwCreate,   //��ʶ��(���ȼ�)
                    PVOID      pvEnvironment, //ָ�򻷾��ַ���
                    PCTSTR      pszCurDir,   //�ӽ��̵�ǰĿ¼
                    PSTARTUPINFO    psiStartInfo,
                    PPROCESS_INFORMATION ppiProcInfo);   //�����߳̾����ID
2���򿪽���
            HANDLE OpenProcess(
                    DWORD dwDesiredAccess, //���ʰ�ȫ����
                    BOOL bInheritHandle, //�̳�����
                    DWORD hProcessId);   //����ID
            ע����ȡhPronessIdָ���Ľ��̵��ں˶���ľ��
3����ֹ����
��1�������̵߳Ľ���㺯������
��2���������Լ���ֹ�Լ�
              VOID ExitProcess(
                        UINT fuExitCode); //�˳�����
��3������ֹ������̻���������
              BOOL TerminateProcess(
                      HANDLE hProcess, //���̾��
                      UINT fuExitCode); //�˳�����
4����ȡ���̵Ŀ�ִ���ļ���DLL��Ӧ�ľ��
              HMODULE GetModuleHandle(
                      PCTSTR pszModule); //ģ������
              ע����������NULLʱ��ȡ���ǽ��̵ĵ�ַ�ռ��п�ִ���ļ��Ļ���ַ��
5����ȡ��ָ�����ڹ�����һ���һ�����̺��̱߳�ʶ��
              HANDLE GetWindowThreadProcessId(
                      HWND hWnd,    //���ھ��
                      LPDWORD lpdwProcessId); //��ô�����صĽ���ID
6����ȡ���̵�����ʱ��
              Bool GetProcessTimes(
                      HANDLE hProcess,   //���̾��
                      PFILETIME pftCreationTime, //����ʱ��
                      PFILETIME pftExitTime, //�˳�ʱ��
                      PFILETIME pftKernelTime, //�ں�ʱ��
                      PFILETIME pftUserTime); //�û�ʱ��
              ע�����ص�ʱ��������ĳ�������е������̣߳������Ѿ���ֹ���е��̣߳���
7����ȡ��ǰ���̵�һ��α���
              HANDLE GetCurrentProcess();
               ע���ú�����ȡ��ǰ���̵�α�����ͨ�����ֵΪ-1��ֻ�ܱ�ʶ��ǰ�����ں˶���
                      ���Ը��ƣ������ɼ̳С����ص���CloseHandle()�������ر���������
8�������̵�α���ת��Ϊʵ���
              HANDLE DuplicateHandle(
                      GetCurrentProcess(),
                      GetCurrentProcess(),
                      GetCurrentProcess(),
                      &hProcess,
                      0,
                      FALSE ,
                      DUPLICATE_SAME_ACCESS);
9����ȡ��ǰ����ID
              DWORD GetCurrentProcessId();
10����ȡ�������ȼ�
              DWORD GetPriorityClass(
                   HANDLE hProcess);
11���޸Ľ��̵����ȼ���
              BOOL SetPriorityClass(
                     HANDLE hProcess, //���̾��
                     DWORD fdwPriority); //��Խ������ȼ�
              ע1������߳����ȼ�
                     ʵʱ�� REALTIME_PRIORITY_CLASS
                     �ߣ� HIGH_PRIORITY_CLASS
                     ��������; ABOVE_NORMAL_PRIORITY_CLASS
                     ������ NORMAL_PRIORITY_CLASS
                     ���������� BELOW_NORMAL_PRIORITY_CLASS
                     ���У� IDLE_PRIORITY_CLASS
              ע2��ֻҪӵ�н��̵ľ�����㹻��Ȩ�ޣ����ܹ��޸�ϵͳ�����е��κν��̵���
                       �ȼ��ࡣ
12����ȡ���̾������
              BOOL GetProcessHandleCount(
                      HANDLE hProcess,   //���
                      PDWORD pdwHandleCount); //�������
13����ȡ������
              DWORD GetEnvironmentVariable(
                      LPCTSTR lpName, //��������������
                      LPTSTR lpValue, //��ŷ����ַ����Ļ�����
                      DWORD cchValue); //�������Ĵ�С
              ע������ֵΪ�����ַ����ĳ��ȣ������治��ʱ���������ַ����ĳ���
14�����û�����
              BOOL SetEnvironmentVariable(
                      LPCTSTR lpName, //��������������
                      LPCTSTR lpValue); //��ű���ֵ�ַ����Ļ�����

**/

#endif


CProcess::CProcess()
{

}

int CProcess::CreateProc(const string &fileName, const string &cmd)
{
    int ret = -1;

#ifdef M_GNU
#if 0
    pid_t pid;
    pid = fork();
    ret = pid;
#else
    STARTUPINFO stStartUpInfo;
    ::memset(&stStartUpInfo, 0 ,sizeof(stStartUpInfo));
    stStartUpInfo.cb = sizeof(stStartUpInfo);

    PROCESS_INFORMATION stProcessInfo;
    ::memset(&stProcessInfo, 0 ,sizeof(stProcessInfo));

    TCHAR szPath[1024];
    TCHAR szCmd[1024];

    CharToTchar(fileName.c_str(), szPath);
    CharToTchar(cmd.c_str(), szCmd);

    CreateProcess(
            szPath,
            szCmd,
            NULL,
            NULL,
            false,
            CREATE_NO_WINDOW,
            NULL,
            NULL,
            &stStartUpInfo,
            &stProcessInfo);

#endif
#else

    TCHAR szFilename[MAX_PATH] ;
    TCHAR szCmdLine[MAX_PATH] ;
#if 0
    // ��ȡ���ڵ�ǰ��ִ���ļ����ļ���
   :: GetModuleFileName(NULL, szFilename, MAX_PATH) ;
   // ��ʽ�������ӽ��̵������в�֪ͨ��EXE�ļ����Ϳ�¡ID
   :: sprintf(szCmdLine, "%s, %d", szFilename, nCloneID) ;
#else
    CharToTchar(fileName.c_str(), szFilename);
    CharToTchar(cmd.c_str(), szCmdLine);
#endif


   // �����ӽ��̵�STARTUPINFO�ṹ
   STARTUPINFO si;
   :: ZeroMemory(reinterpret_cast <void*> (&si) , sizeof(si) ) ;
   si.cb = sizeof(si) ;                                        // �����Ǳ��ṹ�Ĵ�С

   // ���ص������ӽ��̵Ľ�����Ϣ
   PROCESS_INFORMATION pi;
   // ����ͬ���Ŀ�ִ���ļ��������д������̣����������ӽ��̵�����
   BOOL bCreateOK = :: CreateProcess(
       szFilename,                  // �������EXE��Ӧ�ó��������
       szCmdLine,                   // ��������Ϊ��һ���ӽ��̵ı�־
       NULL,                        // ȱʡ�Ľ��̰�ȫ��
       NULL,                        // ȱʡ���̰߳�ȫ��
       FALSE,                       // ���̳о��
       CREATE_NEW_CONSOLE,          // ʹ���µĿ���̨
       NULL,                        // �µĻ���
       NULL,                        // ��ǰĿ¼
       &si,                         // ������Ϣ
       &pi) ;                       // ���صĽ�����Ϣ

    if (bCreateOK)
    {
        //�ȴ�3s��رս���
        //WaitForSingleObject(pi.hProcess,3000L);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        ret = pi.dwProcessId;
    }
    else{
        //�����������ʧ�ܣ��鿴������
        DWORD dwErrCode = GetLastError();
        printf_s("ErrCode : %d\n",dwErrCode);
        ret = -1;
   }

#endif
   return ret;
}

void CProcess::ExitProc()
{
    exit(0);
}

bool CProcess::KillProc(int pid)
{
    bool ret = false;
#ifdef M_GNU
#else
    HANDLE pHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE, pid );
    if(pHandle == NULL){LogPos(1); return false;}
    ret = ::TerminateProcess( pHandle, 0 );
#endif
    return ret;
}

bool CProcess::KillProc(const string &name)
{
    bool ret = false;
#ifdef M_GNU
#else
    TCHAR pProcessName[MAX_PATH] = {0};
    HANDLE hSnapshot;
    PROCESSENTRY32 lppe;

    CharToTchar(name.c_str(), pProcessName);
    //����ϵͳ����
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(hSnapshot == NULL){LogPos(1); return false;}
    //��ʼ�� lppe �Ĵ�С
    lppe.dwSize = sizeof(lppe);
    //���ҵ�һ������
    if (!Process32First(hSnapshot, &lppe)) return false;
    do
    {
        if(_tcscmp(lppe.szExeFile, pProcessName) == 0)
        {

            DWORD xCode;
            HANDLE hProc;
            hProc = OpenProcess(PROCESS_TERMINATE, false, lppe.th32ProcessID);
            if(!GetExitCodeProcess(hProc, &xCode) )
            {
                TerminateProcess(hProc, xCode);
            }
        }
    }
    while (Process32Next(hSnapshot, &lppe)); //������һ������
    CloseHandle(hSnapshot);
#endif
    return ret;
}

int CProcess::GetPid()
{
    return GetPID();
}

int CProcess::GetPid(const string &prcName)
{
    int pid = -1;
#ifdef M_GNU
#else
    PROCESSENTRY32 pe32;
    MODULEENTRY32 me32;
    HANDLE hSnapshot_proc;
    char buf[MAX_PATH] = {0};

    pe32.dwSize = sizeof(pe32);
    hSnapshot_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot_proc, &pe32)){
        do
        {
//            CharToTchar(target, tBuf);
            TcharToChar(pe32.szExeFile, buf);
            if(strcmp(buf, prcName.c_str()) == 0){
                pid = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot_proc, &pe32));
    }
    CloseHandle(hSnapshot_proc);
#endif
    return pid;
}

int CProcess::GetPPid(int id)
{
    int pid = -1;
#ifdef M_GNU
#else
#if 0
    LONG                        status;
    HANDLE                      hProcess;
    PROCESS_BASIC_INFORMATION   pbi;

    PROCNTQSIP NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
        GetModuleHandle(L"ntdll"), "NtQueryInformationProcess");

    if (NULL == NtQueryInformationProcess)
    {
        LogPos(1);
        return -1;
    }
    // Get process handle
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
    if (!hProcess)
    {
        LogPos(1);
        return -1;
    }

    // Retrieve information
    status = NtQueryInformationProcess(hProcess,
        ProcessBasicInformation,
        (PVOID)&pbi,
        sizeof(PROCESS_BASIC_INFORMATION),
        NULL
        );

    // Copy parent Id on success
    if (status)
    {
        pid = pbi.InheritedFromUniqueProcessId;
    }

    CloseHandle(hProcess);
#else
    PROCESSENTRY32 pe32;
    MODULEENTRY32 me32;
    HANDLE hSnapshot_proc;

    pe32.dwSize = sizeof(pe32);
    hSnapshot_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot_proc, &pe32)){
        do
        {
            if(pe32.th32ProcessID == id){
                pid = pe32.th32ParentProcessID;
                break;
            }
        } while (Process32Next(hSnapshot_proc, &pe32));
    }
    CloseHandle(hSnapshot_proc);
#endif
#endif
    return pid;
}

int CProcess::GetPPid(const string &name)
{
    int pid = -1;
#ifdef M_GNU
#else
    HANDLE PHANDLE = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (PHANDLE == INVALID_HANDLE_VALUE){
        LogPos(1);
        return -1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    pe32.dwFlags = sizeof(pe32);
    BOOL hProcess = Process32First(PHANDLE, &pe32);

    TCHAR processName[MAX_PATH] = {0};
    CharToTchar(name.c_str(), processName);
    while (hProcess){
        if (_tcscmp(pe32.szExeFile, processName) == 0){
            pid = pe32.th32ParentProcessID;
            break;
        }

        hProcess = Process32Next(PHANDLE, &pe32);
    }

    CloseHandle(PHANDLE);
#endif
    return pid;
}

string CProcess::GetProcessName(int pid)
{
    string name;
#ifdef M_GNU
#else
    HANDLE PHANDLE = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (PHANDLE == INVALID_HANDLE_VALUE){
        LogPos(1);
        return "";
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    pe32.dwFlags = sizeof(pe32);
    BOOL hProcess = Process32First(PHANDLE, &pe32);

    char processName[MAX_PATH] = {0};
    while (hProcess){
        if (pe32.th32ProcessID == pid){
            TcharToChar(pe32.szExeFile, processName);
            break;
        }

        hProcess = Process32Next(PHANDLE, &pe32);
    }

    CloseHandle(PHANDLE);
    name += processName;
#endif
    return name;
}

int CProcess::GetProcList(vector<int> &pid, vector<string> &name)
{
    int pNum = 0;
#ifdef M_GNU
#else
    PROCESSENTRY32 pe32;
    MODULEENTRY32 me32;
    HANDLE hSnapshot_proc;
    char buf[MAX_PATH] = {0};

    pe32.dwSize = sizeof(pe32);
    hSnapshot_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot_proc, &pe32)){
        do
        {
//            CharToTchar(target, tBuf);
            TcharToChar(pe32.szExeFile, buf);
            pid.push_back((int)pe32.th32ProcessID);
            name.push_back(string(buf));
            pNum++;
        } while (Process32Next(hSnapshot_proc, &pe32));
    }
    CloseHandle(hSnapshot_proc);
#endif
    return pNum;
}

bool CProcess::IsAdmin()
{
    bool ret = false;
#ifdef M_GNU
#else
    HANDLE hProcess = GetCurrentProcess();
    if(hProcess == NULL){LogPos(1); return false;}

    HANDLE hToken = NULL;
    OpenProcessToken(hProcess , TOKEN_QUERY , &hToken );

    TOKEN_ELEVATION_TYPE tokenType = TokenElevationTypeDefault ; // ���ڽ�����������
    DWORD dwRetSize = 0; // ���ڽ��պ��������Ϣ���ֽ���
    // 2. ��ѯ���������е�Ȩ������ֵ.( ���ֵ���¼��ǰ�������Ǻ�������( ϸ����17_Ȩ�޹���_���ƵĻ�ȡ.cpp ) )
    GetTokenInformation( hToken ,
                         TokenElevationType ,// ��ȡ���Ƶĵ�ǰ�����ȼ�
                         &tokenType ,
                         sizeof( tokenType ) ,
                         &dwRetSize // ���軺�������ֽ���
                         );

    // �������Ƶ������������Ӧ����Ϣ
    if( TokenElevationTypeFull == tokenType ) {
        // 3. ���������TokenElevationTypeFull , ��ӵ���������ϵ�����,���Ը���������κ���Ȩ
        printf( "����Ա�˻�,��ӵ��ȫ����Ȩ��,���Ը���������κ���Ȩ\n" );
        ret = true;
    }
    // 4. �����������, ����Ҫ�Թ���Ա����������б�����. ���������Ե������ķ������ʣ�µ�����.
    else if( TokenElevationTypeDefault == tokenType ) {
        printf( "Ĭ���û�, ������һ����ͨ�û�, �����ǹر�UACʱ��¼�Ĺ���Ա�û�\n" );
        // ����ϵͳ����IsUserAnAdmin, ��һ��ȷ������ͨ�û����ǹ���Ա�û�
        ret = IsUserAnAdmin();
    }
    else if( TokenElevationTypeLimited == tokenType ) {
        // �ж������Ƶ��û��ǹ���Ա
        // ����ǹ���Ա, ����������лᱣ���й���Ա��SID

        // 1. ��ȡϵͳ�ڼ�����Ա�û���SID
        SID adminSid;
        DWORD dwSize = sizeof( adminSid );
        CreateWellKnownSid( WinBuiltinAdministratorsSid , // ��ȡSID������,������ϵͳ�ڼ�����Ա
                            NULL , // ��NULL,��ȡ���ؼ�����Ĺ���Ա
                            &adminSid ,// ��������Ĺ���ԱSID
                            &dwSize // ����ṹ�Ĵ�С,Ҳ��Ϊ���
                            );

        // ��ȡ�����Ƶ���������(�����Ƶ����ƶ�����һ�����ӵ�����,�����Ƶ�������ʽ����������������. )
        TOKEN_LINKED_TOKEN linkToken;
        GetTokenInformation( hToken ,
                             TokenLinkedToken , // ��ȡ���ӵ����ƾ��
                             &linkToken ,
                             sizeof( linkToken ) ,
                             &dwSize
                             );

        // �����ӵ������в����Ƿ���й���Ա��SID
        BOOL    bIsContain = FALSE; // ���ڱ����Ƿ����.
        CheckTokenMembership( linkToken.LinkedToken , // ����������м��
                              &adminSid ,             // ����������Ƿ������SID
                              &bIsContain );           // ���TRUE�����,��֮������

        if( bIsContain ) {
            printf( "Ȩ�ޱ��˸�������ƹ���Ա�˻�, ����Ȩ�ޱ��ƴ���\n" );
        }

        ret = bIsContain; // �����Թ���ԱȨ������
    }
    CloseHandle(hToken);
#endif
    return ret;
}

void CProcess::RunAsAdmin()
{
#ifdef M_GNU
#else
    // ����ȨȨ��������SeDebug��Ȩ, ���ܹ����κν���.
    // ����ֻ���Թ���Ա�������,���Ҳ��������ƵĹ���Ա��ݲ����޸�,���,
    // ��Ҫ�жϵ�ǰ�Ƿ���й���Ա��ȫ��Ȩ��
    // �������, ����Ҫ�Թ���Ա������б�����
    if(!IsAdmin())
    {
        printf("isUser\n");
        // �Թ���Ա������б�����
        //  1 ��ȡ�����̵��ļ�·��.
        TCHAR path[ MAX_PATH ] = { 0 }; // ��Ҫ��ʼ��
        DWORD dwPathSize = MAX_PATH;
        QueryFullProcessImageName( GetCurrentProcess( ) , 0 ,
                                   path ,
                                   &dwPathSize );

        // 2 ���ô������̵�API���б�����.
        ShellExecute( NULL ,            // ���ھ��,û������NULL
                      _T( "runas" ) ,   // �Թ���Ա������е���Ҫ����
                      path ,            // �������еĳ����·��(�����Ǳ�����)
                      NULL ,            // �����в���
                      NULL ,            // �½��̵Ĺ���Ŀ¼��·��
                      SW_SHOW           // ���������ʾ��־(��С��,���, ��ʾ,���ص�)
                      );

        // �˳�������
        ExitProcess( 0 );
    }
#if 1
    // ��Ȩ��������SeDbugȨ��
    if(!AddPrivilege() ) {
        printf( "����Ȩ��ʧ��\n" );
        system( "pause" );
        return;
    }
#endif
#endif
}

bool CProcess::AddPrivilege()
{
    bool ret = false;
#ifdef M_GNU
#else
    TCHAR *pszPrivilegeName = SE_DEBUG_NAME;
//    CharToTchar(pPrivilegeName, pszPrivilegeName);

    // ���̵���Ȩʹ��LUIDֵ����ʾ, ���, ��Ҫ�Ȼ�ȡ�����Ȩ������Ӧ��LUIDֵ
    // 0. ��ȡ��Ȩ��Ӧ��LUIDֵ
    LUID privilegeLuid;
    if( !LookupPrivilegeValue( NULL , pszPrivilegeName , &privilegeLuid ) )
        return false;

    // 1. ��ȡ����������
    HANDLE hToken = NULL;
    // ������ʱ, ��Ҫ����TOKEN_ADJUST_PRIVILEGES Ȩ��(���Ȩ�������޸�������Ȩ)
    if( !OpenProcessToken( GetCurrentProcess( ) , TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &hToken ) ) {
        printf( "������:%x\n" , GetLastError( ) );
        CloseHandle(hToken);
        return false;
    }

    // 2. ʹ��������Ȩ�޸ĺ�����SeDebug��LUID��Ȩֵ��ӵ�����������
    TOKEN_PRIVILEGES tokenPrivieges; // �µ���Ȩ

    // ʹ����Ȩ��LUID����ʼ���ṹ��.
    tokenPrivieges.PrivilegeCount = 1; // ��Ȩ����
    tokenPrivieges.Privileges[ 0 ].Luid = privilegeLuid; // ����ȨLUID���浽������
    tokenPrivieges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;// ������ֵ��Ϊ����(�н���,�Ƴ�������״̬)

    // ���ú��������Ȩ
    ret =  AdjustTokenPrivileges( hToken ,              // Ҫ�����Ȩ������
                                  FALSE ,               // TRUE���Ƴ���Ȩ, FALSE�������Ȩ
                                  &tokenPrivieges ,     // Ҫ��ӵ���Ȩ����
                                  sizeof( tokenPrivieges ) ,// ������Ȩ����Ĵ�С
                                  NULL ,                // �ɵ���Ȩ����
                                  NULL                  // �ɵ���Ȩ����ĳ���
                                  );
    CloseHandle(hToken);
#endif

    return ret;
}
