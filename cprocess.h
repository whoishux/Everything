#ifndef CPROCESS_H
#define CPROCESS_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class CProcess
{
public:
    CProcess();

    static int CreateProc(const string &fileName, const string &cmd="");
    static void ExitProc();
    static bool KillProc(int pid);
    static bool KillProc(const string &name);
    static int GetPid();
    static int GetPid(const string &prcName);
    static int GetPPid(int id);
    static int GetPPid(const string &name);
    static string GetProcessName(int pid);
    static int GetProcList(vector<int> &pid, vector<string> &name);
    static bool IsAdmin();
    static void RunAsAdmin();

private:
    static bool AddPrivilege();

};

#endif // CPROCESS_H
