#include <memory.h>
#include <string.h>
#include <stdio.h>

#include "ccommon.h"
#include "wincommon.h"

//#define _MBCS

/**
    CancelWaitableTimer 这个函数用于取消一个可以等待下去的计时器操作
    CallNamedPipe 这个函数由一个希望通过管道通信的一个客户进程调用
    ConnectNamedPipe 指示一台服务器等待下去，直至客户机同一个命名管道连接
    CreateEvent 创建一个事件对象
    CreateMailslot 创建一个邮路。返回的句柄由邮路服务器使用（收件人）
    CreateMutex 创建一个互斥体（MUTEX）
    CreateNamedPipe 创建一个命名管道。返回的句柄由管道的服务器端使用
    CreatePipe 创建一个匿名管道
    CreateProcess 创建一个新进程（比如执行一个程序）
    CreateSemaphore 创建一个新的信号机
    CreateWaitableTimer 创建一个可等待的计时器对象
    DisconnectNamedPipe 断开一个客户与一个命名管道的连接
    DuplicateHandle 在指出一个现有系统对象当前句柄的情况下，为那个对象创建一个新句柄
    ExitProcess 中止一个进程
    FindCloseChangeNotification 关闭一个改动通知对象
    FindExecutable 查找与一个指定文件关联在一起的程序的文件名
    FindFirstChangeNotification 创建一个文件通知对象。该对象用于监视文件系统发生的变化
    FindNextChangeNotification 重设一个文件改变通知对象，令其继续监视下一次变化
    FreeLibrary 释放指定的动态链接库
    GetCurrentProcess 获取当前进程的一个伪句柄
    GetCurrentProcessId 获取当前进程一个唯一的标识符
    GetCurrentThread 获取当前线程的一个伪句柄
    GetCurrentThreadId 获取当前线程一个唯一的线程标识符
    GetExitCodeProces 获取一个已中断进程的退出代码
    GetExitCodeThread 获取一个已中止线程的退出代码
    GetHandleInformation 获取与一个系统对象句柄有关的信息
    GetMailslotInfo 获取与一个邮路有关的信息
    GetModuleFileName 获取一个已装载模板的完整路径名称
    GetModuleHandle 获取一个应用程序或动态链接库的模块句柄
    GetPriorityClass 获取特定进程的优先级别
    GetProcessShutdownParameters 调查系统关闭时一个指定的进程相对于其它进程的关闭早迟情况
    GetProcessTimes 获取与一个进程的经过时间有关的信息
    GetProcessWorkingSetSize 了解一个应用程序在运行过程中实际向它交付了多大容量的内存
    GetSartupInfo 获取一个进程的启动信息
    GetThreadPriority 获取特定线程的优先级别
    GetTheardTimes 获取与一个线程的经过时间有关的信息
    GetWindowThreadProcessId 获取与指定窗口关联在一起的一个进程和线程标识符
    LoadLibrary 载入指定的动态链接库，并将它映射到当前进程使用的地址空间
    LoadLibraryEx 装载指定的动态链接库，并为当前进程把它映射到地址空间
    LoadModule 载入一个Windows应用程序，并在指定的环境中运行
    MsgWaitForMultipleObjects 等侯单个对象或一系列对象发出信号。如返回条件已经满足，则立即返回
    SetPriorityClass 设置一个进程的优先级别
    SetProcessShutdownParameters 在系统关闭期间，为指定进程设置他相对于其它程序的关闭顺序
    SetProcessWorkingSetSize 设置操作系统实际划分给进程使用的内存容量
    SetThreadPriority 设定线程的优先级别
    ShellExecute 查找与指定文件关联在一起的程序的文件名
    TerminateProcess 结束一个进程
    WinExec 运行指定的程序
**/

void TcharToChar(const TCHAR * tchar, char * _char)
{
#ifdef _MBCS
    //memcpy(_char, tchar, strlen(tchar));
    strcpy(_char, tchar);
#else
    int iLength;
    //获取字节长度
    iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    //将tchar值赋给_char
    WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
#endif
}

void CharToTchar(const char * _char, TCHAR * tchar)
{
#ifdef _MBCS
    //memcpy(tchar, _char, strlen(_char));
    strcpy(tchar, _char);
#else
    int iLength;
    iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
#endif
}

char* TcharToChar(const TCHAR * tchar)
{
    int tLen=0;
    char * _char = NULL;

    if(!tchar) return NULL;

    tLen = _tcsclen(tchar);
    if(tLen <= 0) return NULL;
#ifdef _MBCS
    _char = (char *)calloc(tLen+1, sizeof(char));
    memcpy(_char, tchar, tLen);
#else
    int iLength;
    //获取字节长度
    iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    _char = (char *)calloc(iLength+1, sizeof(char));
    //将tchar值赋给_char
    WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
#endif

    return _char;
}

TCHAR * CharToTchar(const char * _char)
{
    int cLen = 0;
    TCHAR * tchar = NULL;

    if(!_char) return NULL;

    cLen = strlen(_char);
    if(cLen <= 0) return NULL;

#ifdef _MBCS
    tchar = (TCHAR *)calloc(cLen+1, sizeof(TCHAR));
    memcpy(tchar, _char, cLen);
#else
    int iLength;
    iLength = MultiByteToWideChar(CP_ACP, 0, _char, cLen + 1, NULL, 0);
    tchar = (TCHAR *)calloc(iLength+1, sizeof(TCHAR));
    MultiByteToWideChar(CP_ACP, 0, _char, cLen + 1, tchar, iLength);
#endif

    return tchar;
}

bool RunCmd(const char *exe, const char *cmd)
{
    STARTUPINFO stStartUpInfo;
    ::memset(&stStartUpInfo, 0 ,sizeof(stStartUpInfo));
    stStartUpInfo.cb = sizeof(stStartUpInfo);

    PROCESS_INFORMATION stProcessInfo;
    ::memset(&stProcessInfo, 0 ,sizeof(stProcessInfo));

    TCHAR szPath[1024];
    TCHAR szCmd[1024];

    if(exe) CharToTchar(exe, szPath);
    if(cmd) CharToTchar(cmd, szCmd);

    LogFile("RunCmd:%s %s\n", exe, cmd);

    bool bRet = ::CreateProcess(
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

    //printf("bRet=%d\n", bRet);

    if (bRet)
    {
        WaitForSingleObject(stProcessInfo.hProcess, INFINITE);
        ::CloseHandle(stProcessInfo.hProcess);
        ::CloseHandle(stProcessInfo.hThread);
        stProcessInfo.hProcess = NULL;
        stProcessInfo.hThread = NULL;
        stProcessInfo.dwProcessId = 0;
        stProcessInfo.dwThreadId = 0;
    }

    return bRet;
}
