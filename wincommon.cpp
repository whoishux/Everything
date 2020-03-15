#include <memory.h>
#include <string.h>
#include <stdio.h>

#include "ccommon.h"
#include "wincommon.h"

//#define _MBCS

/**
    CancelWaitableTimer �����������ȡ��һ�����Եȴ���ȥ�ļ�ʱ������
    CallNamedPipe ���������һ��ϣ��ͨ���ܵ�ͨ�ŵ�һ���ͻ����̵���
    ConnectNamedPipe ָʾһ̨�������ȴ���ȥ��ֱ���ͻ���ͬһ�������ܵ�����
    CreateEvent ����һ���¼�����
    CreateMailslot ����һ����·�����صľ������·������ʹ�ã��ռ��ˣ�
    CreateMutex ����һ�������壨MUTEX��
    CreateNamedPipe ����һ�������ܵ������صľ���ɹܵ��ķ�������ʹ��
    CreatePipe ����һ�������ܵ�
    CreateProcess ����һ���½��̣�����ִ��һ������
    CreateSemaphore ����һ���µ��źŻ�
    CreateWaitableTimer ����һ���ɵȴ��ļ�ʱ������
    DisconnectNamedPipe �Ͽ�һ���ͻ���һ�������ܵ�������
    DuplicateHandle ��ָ��һ������ϵͳ����ǰ���������£�Ϊ�Ǹ����󴴽�һ���¾��
    ExitProcess ��ֹһ������
    FindCloseChangeNotification �ر�һ���Ķ�֪ͨ����
    FindExecutable ������һ��ָ���ļ�������һ��ĳ�����ļ���
    FindFirstChangeNotification ����һ���ļ�֪ͨ���󡣸ö������ڼ����ļ�ϵͳ�����ı仯
    FindNextChangeNotification ����һ���ļ��ı�֪ͨ�����������������һ�α仯
    FreeLibrary �ͷ�ָ���Ķ�̬���ӿ�
    GetCurrentProcess ��ȡ��ǰ���̵�һ��α���
    GetCurrentProcessId ��ȡ��ǰ����һ��Ψһ�ı�ʶ��
    GetCurrentThread ��ȡ��ǰ�̵߳�һ��α���
    GetCurrentThreadId ��ȡ��ǰ�߳�һ��Ψһ���̱߳�ʶ��
    GetExitCodeProces ��ȡһ�����жϽ��̵��˳�����
    GetExitCodeThread ��ȡһ������ֹ�̵߳��˳�����
    GetHandleInformation ��ȡ��һ��ϵͳ�������йص���Ϣ
    GetMailslotInfo ��ȡ��һ����·�йص���Ϣ
    GetModuleFileName ��ȡһ����װ��ģ�������·������
    GetModuleHandle ��ȡһ��Ӧ�ó����̬���ӿ��ģ����
    GetPriorityClass ��ȡ�ض����̵����ȼ���
    GetProcessShutdownParameters ����ϵͳ�ر�ʱһ��ָ���Ľ���������������̵Ĺر�������
    GetProcessTimes ��ȡ��һ�����̵ľ���ʱ���йص���Ϣ
    GetProcessWorkingSetSize �˽�һ��Ӧ�ó��������й�����ʵ�����������˶���������ڴ�
    GetSartupInfo ��ȡһ�����̵�������Ϣ
    GetThreadPriority ��ȡ�ض��̵߳����ȼ���
    GetTheardTimes ��ȡ��һ���̵߳ľ���ʱ���йص���Ϣ
    GetWindowThreadProcessId ��ȡ��ָ�����ڹ�����һ���һ�����̺��̱߳�ʶ��
    LoadLibrary ����ָ���Ķ�̬���ӿ⣬������ӳ�䵽��ǰ����ʹ�õĵ�ַ�ռ�
    LoadLibraryEx װ��ָ���Ķ�̬���ӿ⣬��Ϊ��ǰ���̰���ӳ�䵽��ַ�ռ�
    LoadModule ����һ��WindowsӦ�ó��򣬲���ָ���Ļ���������
    MsgWaitForMultipleObjects �Ⱥ�������һϵ�ж��󷢳��źš��緵�������Ѿ����㣬����������
    SetPriorityClass ����һ�����̵����ȼ���
    SetProcessShutdownParameters ��ϵͳ�ر��ڼ䣬Ϊָ�������������������������Ĺر�˳��
    SetProcessWorkingSetSize ���ò���ϵͳʵ�ʻ��ָ�����ʹ�õ��ڴ�����
    SetThreadPriority �趨�̵߳����ȼ���
    ShellExecute ������ָ���ļ�������һ��ĳ�����ļ���
    TerminateProcess ����һ������
    WinExec ����ָ���ĳ���
**/

void TcharToChar(const TCHAR * tchar, char * _char)
{
#ifdef _MBCS
    //memcpy(_char, tchar, strlen(tchar));
    strcpy(_char, tchar);
#else
    int iLength;
    //��ȡ�ֽڳ���
    iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    //��tcharֵ����_char
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
    //��ȡ�ֽڳ���
    iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
    _char = (char *)calloc(iLength+1, sizeof(char));
    //��tcharֵ����_char
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
