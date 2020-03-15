#ifndef WINCOMMON_H
#define WINCOMMON_H

#include <Windows.h>
#include <tchar.h>

#include "ccommon.h"


ALGAPI void TcharToChar(const TCHAR * tchar, char * _char);
ALGAPI void CharToTchar(const char * _char, TCHAR * tchar);

ALGAPI bool RunCmd(const char *exe, const char *cmd);

#endif // WINCOMMON_H
