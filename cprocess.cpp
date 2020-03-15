/******************************************************
* Program/Assignment:   进程相关操作类;
* Author:               Hux
* Date:                 2019/12/24
* Description:          当前只实现Windows系统相关操作，Linux系统未实现;
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
1、创建进程
            BOOL CreateProcess(
                    PCTSTR      psApplicationName, //可执行文件的名字
                    PTSTR      pszCommandLine, //命令行字符串
                    PSECURITY_ATTRIBUTES psaProcess,   //进程对象的安全性
                    PSECURITY_ATTRIBUTES psaThread,   //线程对象的安全性
                    BOOL      bInheritHandles, //句柄可继承性
                    DWORD      fdwCreate,   //标识符(优先级)
                    PVOID      pvEnvironment, //指向环境字符串
                    PCTSTR      pszCurDir,   //子进程当前目录
                    PSTARTUPINFO    psiStartInfo,
                    PPROCESS_INFORMATION ppiProcInfo);   //进程线程句柄及ID
2、打开进程
            HANDLE OpenProcess(
                    DWORD dwDesiredAccess, //访问安全属性
                    BOOL bInheritHandle, //继承属性
                    DWORD hProcessId);   //进程ID
            注：获取hPronessId指定的进程的内核对象的句柄
3、终止进程
（1）、主线程的进入点函数返回
（2）、进程自己终止自己
              VOID ExitProcess(
                        UINT fuExitCode); //退出代码
（3）、终止自身进程或其他进程
              BOOL TerminateProcess(
                      HANDLE hProcess, //进程句柄
                      UINT fuExitCode); //退出代码
4、获取进程的可执行文件或DLL对应的句柄
              HMODULE GetModuleHandle(
                      PCTSTR pszModule); //模块名称
              注：当参数传NULL时获取的是进程的地址空间中可执行文件的基地址。
5、获取与指定窗口关联在一起的一个进程和线程标识符
              HANDLE GetWindowThreadProcessId(
                      HWND hWnd,    //窗口句柄
                      LPDWORD lpdwProcessId); //与该窗口相关的进程ID
6、获取进程的运行时间
              Bool GetProcessTimes(
                      HANDLE hProcess,   //进程句柄
                      PFILETIME pftCreationTime, //创建时间
                      PFILETIME pftExitTime, //退出时间
                      PFILETIME pftKernelTime, //内核时间
                      PFILETIME pftUserTime); //用户时间
              注：返回的时间适用于某个进程中的所有线程（甚至已经终止运行的线程）。
7、获取当前进程的一个伪句柄
              HANDLE GetCurrentProcess();
               注：该函数获取当前进程的伪句柄，通常情况值为-1，只能标识当前进程内核对象，
                      可以复制，但不可继承。不必调用CloseHandle()函数来关闭这个句柄。
8、将进程的伪句柄转换为实句柄
              HANDLE DuplicateHandle(
                      GetCurrentProcess(),
                      GetCurrentProcess(),
                      GetCurrentProcess(),
                      &hProcess,
                      0,
                      FALSE ,
                      DUPLICATE_SAME_ACCESS);
9、获取当前进程ID
              DWORD GetCurrentProcessId();
10、获取进程优先级
              DWORD GetPriorityClass(
                   HANDLE hProcess);
11、修改进程的优先级类
              BOOL SetPriorityClass(
                     HANDLE hProcess, //进程句柄
                     DWORD fdwPriority); //相对进程优先级
              注1：相对线程优先级
                     实时： REALTIME_PRIORITY_CLASS
                     高： HIGH_PRIORITY_CLASS
                     高于正常; ABOVE_NORMAL_PRIORITY_CLASS
                     正常： NORMAL_PRIORITY_CLASS
                     低于正常： BELOW_NORMAL_PRIORITY_CLASS
                     空闲： IDLE_PRIORITY_CLASS
              注2：只要拥有进程的句柄和足够的权限，就能够修改系统中运行的任何进程的优
                       先级类。
12、获取进程句柄计数
              BOOL GetProcessHandleCount(
                      HANDLE hProcess,   //句柄
                      PDWORD pdwHandleCount); //句柄计数
13、获取环境块
              DWORD GetEnvironmentVariable(
                      LPCTSTR lpName, //环境变量的名字
                      LPTSTR lpValue, //存放返回字符串的缓冲区
                      DWORD cchValue); //缓冲区的大小
              注：返回值为返回字符串的长度，当缓存不足时返回所需字符串的长度
14、设置环境块
              BOOL SetEnvironmentVariable(
                      LPCTSTR lpName, //环境变量的名字
                      LPCTSTR lpValue); //存放变量值字符串的缓冲区

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
    // 提取用于当前可执行文件的文件名
   :: GetModuleFileName(NULL, szFilename, MAX_PATH) ;
   // 格式化用于子进程的命令行并通知其EXE文件名和克隆ID
   :: sprintf(szCmdLine, "%s, %d", szFilename, nCloneID) ;
#else
    CharToTchar(fileName.c_str(), szFilename);
    CharToTchar(cmd.c_str(), szCmdLine);
#endif


   // 用于子进程的STARTUPINFO结构
   STARTUPINFO si;
   :: ZeroMemory(reinterpret_cast <void*> (&si) , sizeof(si) ) ;
   si.cb = sizeof(si) ;                                        // 必须是本结构的大小

   // 返回的用于子进程的进程信息
   PROCESS_INFORMATION pi;
   // 利用同样的可执行文件和命令行创建进程，并赋于其子进程的性质
   BOOL bCreateOK = :: CreateProcess(
       szFilename,                  // 产生这个EXE的应用程序的名称
       szCmdLine,                   // 告诉其行为像一个子进程的标志
       NULL,                        // 缺省的进程安全性
       NULL,                        // 缺省的线程安全性
       FALSE,                       // 不继承句柄
       CREATE_NEW_CONSOLE,          // 使用新的控制台
       NULL,                        // 新的环境
       NULL,                        // 当前目录
       &si,                         // 启动信息
       &pi) ;                       // 返回的进程信息

    if (bCreateOK)
    {
        //等待3s后关闭进程
        //WaitForSingleObject(pi.hProcess,3000L);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        ret = pi.dwProcessId;
    }
    else{
        //如果创建进程失败，查看错误码
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
    //创建系统快照
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if(hSnapshot == NULL){LogPos(1); return false;}
    //初始化 lppe 的大小
    lppe.dwSize = sizeof(lppe);
    //查找第一个进程
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
    while (Process32Next(hSnapshot, &lppe)); //查找下一个进程
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

    TOKEN_ELEVATION_TYPE tokenType = TokenElevationTypeDefault ; // 用于接收令牌类型
    DWORD dwRetSize = 0; // 用于接收函数输出信息的字节数
    // 2. 查询进程令牌中的权限提升值.( 这个值会记录当前的令牌是何种类型( 细节在17_权限管理_令牌的获取.cpp ) )
    GetTokenInformation( hToken ,
                         TokenElevationType ,// 获取令牌的当前提升等级
                         &tokenType ,
                         sizeof( tokenType ) ,
                         &dwRetSize // 所需缓冲区的字节数
                         );

    // 根据令牌的类型来输出相应的信息
    if( TokenElevationTypeFull == tokenType ) {
        // 3. 如果令牌是TokenElevationTypeFull , 则拥有至高无上的能力,可以给令牌添加任何特权
        printf( "管理员账户,并拥有全部的权限,可以给令牌添加任何特权\n" );
        ret = true;
    }
    // 4. 如果是其他的, 则需要以管理员身份重新运行本进程. 这样就能以第三步的方法解决剩下的问题.
    else if( TokenElevationTypeDefault == tokenType ) {
        printf( "默认用户, 可能是一个普通用户, 可能是关闭UAC时登录的管理员用户\n" );
        // 调用系统函数IsUserAnAdmin, 进一步确定是普通用户还是管理员用户
        ret = IsUserAnAdmin();
    }
    else if( TokenElevationTypeLimited == tokenType ) {
        // 判断受限制的用户是管理员
        // 如果是管理员, 则这个令牌中会保存有管理员的SID

        // 1. 获取系统内键管理员用户的SID
        SID adminSid;
        DWORD dwSize = sizeof( adminSid );
        CreateWellKnownSid( WinBuiltinAdministratorsSid , // 获取SID的类型,这里是系统内键管理员
                            NULL , // 传NULL,获取本地计算机的管理员
                            &adminSid ,// 函数输出的管理员SID
                            &dwSize // 输入结构的大小,也作为输出
                            );

        // 获取本令牌的连接令牌(受限制的令牌都会有一个连接的令牌,受限制的令牌正式由主令牌所创建的. )
        TOKEN_LINKED_TOKEN linkToken;
        GetTokenInformation( hToken ,
                             TokenLinkedToken , // 获取连接的令牌句柄
                             &linkToken ,
                             sizeof( linkToken ) ,
                             &dwSize
                             );

        // 在连接的令牌中查找是否具有管理员的SID
        BOOL    bIsContain = FALSE; // 用于保存是否包含.
        CheckTokenMembership( linkToken.LinkedToken , // 在这个令牌中检查
                              &adminSid ,             // 检查令牌中是否包含此SID
                              &bIsContain );           // 输出TRUE则包含,反之不包含

        if( bIsContain ) {
            printf( "权限被阉割的受限制管理员账户, 部分权限被移处理\n" );
        }

        ret = bIsContain; // 不是以管理员权限运行
    }
    CloseHandle(hToken);
#endif
    return ret;
}

void CProcess::RunAsAdmin()
{
#ifdef M_GNU
#else
    // 将特权权限提升到SeDebug特权, 就能够打开任何进程.
    // 但是只有以管理员身份运行,并且不是受限制的管理员身份才能修改,因此,
    // 需要判断当前是否具有管理员的全部权限
    // 如果不是, 则需要以管理员身份运行本程序
    if(!IsAdmin())
    {
        printf("isUser\n");
        // 以管理员身份运行本进程
        //  1 获取本进程的文件路径.
        TCHAR path[ MAX_PATH ] = { 0 }; // 需要初始化
        DWORD dwPathSize = MAX_PATH;
        QueryFullProcessImageName( GetCurrentProcess( ) , 0 ,
                                   path ,
                                   &dwPathSize );

        // 2 调用创建进程的API运行本进程.
        ShellExecute( NULL ,            // 窗口句柄,没有则填NULL
                      _T( "runas" ) ,   // 以管理员身份运行的重要参数
                      path ,            // 所有运行的程序的路径(这里是本进程)
                      NULL ,            // 命令行参数
                      NULL ,            // 新进程的工作目录的路径
                      SW_SHOW           // 创建后的显示标志(最小化,最大化, 显示,隐藏等)
                      );

        // 退出本进程
        ExitProcess( 0 );
    }
#if 1
    // 将权限提升到SeDbug权限
    if(!AddPrivilege() ) {
        printf( "提升权限失败\n" );
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

    // 进程的特权使用LUID值来表示, 因此, 需要先获取传入的权限名对应的LUID值
    // 0. 获取特权对应的LUID值
    LUID privilegeLuid;
    if( !LookupPrivilegeValue( NULL , pszPrivilegeName , &privilegeLuid ) )
        return false;

    // 1. 获取本进程令牌
    HANDLE hToken = NULL;
    // 打开令牌时, 需要加上TOKEN_ADJUST_PRIVILEGES 权限(这个权限用于修改令牌特权)
    if( !OpenProcessToken( GetCurrentProcess( ) , TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY , &hToken ) ) {
        printf( "错误码:%x\n" , GetLastError( ) );
        CloseHandle(hToken);
        return false;
    }

    // 2. 使用令牌特权修改函数将SeDebug的LUID特权值添加到本进程令牌
    TOKEN_PRIVILEGES tokenPrivieges; // 新的特权

    // 使用特权的LUID来初始化结构体.
    tokenPrivieges.PrivilegeCount = 1; // 特权个数
    tokenPrivieges.Privileges[ 0 ].Luid = privilegeLuid; // 将特权LUID保存到数组中
    tokenPrivieges.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;// 将属性值设为启用(有禁用,移除等其他状态)

    // 调用函数添加特权
    ret =  AdjustTokenPrivileges( hToken ,              // 要添加特权的令牌
                                  FALSE ,               // TRUE是移除特权, FALSE是添加特权
                                  &tokenPrivieges ,     // 要添加的特权数组
                                  sizeof( tokenPrivieges ) ,// 整个特权数组的大小
                                  NULL ,                // 旧的特权数组
                                  NULL                  // 旧的特权数组的长度
                                  );
    CloseHandle(hToken);
#endif

    return ret;
}
