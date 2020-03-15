#ifndef CMD5
#define CMD5

#include "ccommon.h"

ALGAPI int MD5EncodeStr(int size, unsigned char *str, unsigned char md5Val[32]);
ALGAPI int MD5EncodeFile(const char *filename, unsigned char md5Val[32]);

#endif // CMD5

