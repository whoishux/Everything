/*
    公共基础函数（跨平台）：(hux)
    1、线程ID;
    2、进程ID;
    3、当前时间（字符串）;
    4、时钟计时;
    5、log;
    6、当前时钟（世界时间）;
    7、创建、删除目录;
    8、删除文件;
    9、bit操作;
    10、睡眠等待;
    11、文件存在、可读写检查;
    12、获取文件路径、目录、文件名、后缀名;
    13、内存检查;
    14、bit拷贝;
*/

#ifndef CCOMMON
#define CCOMMON

#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>

#include "cinclude.h"

using namespace std;

typedef char                            Int8;
typedef short                           Int16;
typedef int                             Int32;
typedef long long                       Int64;
typedef unsigned char                   Uint8;
typedef unsigned short                  Uint16;
typedef unsigned int                    Uint32;
typedef unsigned long long              Uint64;
typedef Uint8                           UInt8;
typedef Uint16                          UInt16;
typedef Uint32                          UInt32;
typedef Uint64                          UInt64;
typedef Uint8                           Byte;
typedef void *                          HANDLE;
//typedef UInt8                           UCHAR;
//typedef UInt16                          USHORT;
//typedef UInt32                          UINT;
//typedef wchar_t                         WCHAR;
//typedef long                            LONG;



#define SIZE_1M                         (0x100000)
#define SIZE_10M                        (0xA00000)
#define SIZE_100M                       (104857600)

#define Max(a, b)                       ((a) > (b) ? (a) : (b))
#define Min(a, b)                       ((a) < (b) ? (a) : (b))
#define Similay(a, b, e)                (Max(a, b) - Min(a, b)) <= (e)

//#define SwapVal(a, b)                   ({(a) ^= (b); (b) ^= (a); (a) ^= (b);})
#define SwapVal(a, b)                   ((a) ^= (b) ^= (a) ^= (b))
#define UpdateMaxVal(max, min)          ((max) < (min) ? SwapVal((max), (min)) : (max))
#define UpdateMinVal(min, max)          ((min) > (max) ? SwapVal((min), (max)) : (min))

// bit[0, 7]
#define LCPL_BIT(value, bit)            ((value) ^= (1<<(bit)))   //取反指定位
#define LSET_BIT(value, bit)            ((value) |= (1<<(bit)))   //置位指定位
#define LCLR_BIT(value, bit)            ((value) &= ~(1<<(bit)))  //清零指定位
#define LGET_BIT(value, bit)            ((value) & (1<<(bit)))    //读取指定位
#define HCPL_BIT(value, bit)            ((value) ^= (1<<(7-(bit))))   //取反指定位
#define HSET_BIT(value, bit)            ((value) |= (1<<(7-(bit))))   //置位指定位
#define HCLR_BIT(value, bit)            ((value) &= ~(1<<(7-(bit))))  //清零指定位
#define HGET_BIT(value, bit)            ((value) & (1<<(7-(bit))))    //读取指定位

// 大小端转换;
#define BLSwap16(A)                     ((((A) & 0xff00) >> 8) | (((A) & 0x00ff) << 8))
#define BLSwap32(A)                     ((((A) & 0xff000000) >> 24) | \
                                        (((A) & 0x00ff0000) >> 8) | \
                                        (((A) & 0x0000ff00) << 8) | \
                                        (((A) & 0x000000ff) << 24))
#define BLSwap64(A)                     ((((A) & 0xff00000000000000) >> 56) | \
                                        (((A) & 0x00ff000000000000) >> 40) | \
                                        (((A) & 0x0000ff0000000000) >> 24) | \
                                        (((A) & 0x000000ff00000000) >> 8) | \
                                        (((A) & 0x00000000ff000000) << 8) | \
                                        (((A) & 0x0000000000ff0000) << 24) | \
                                        (((A) & 0x000000000000ff00) << 40) | \
                                        (((A) & 0x00000000000000ff) << 56))


// 循环左移、循环右移;
#define ROTATE_LEFT(v, s, n)            ((v) << (n)) | ((v) >> ((s) - (n)))
#define ROTATE_RIGHT(v, s, n)           ((v) >> (n)) | ((v) << ((s) - (n)))
#define ROTATE_LEFT32(v, n)             ROTATE_LEFT((unsigned)(v), 32, n)
#define ROTATE_RIGHT32(v, n)            ROTATE_RIGHT((unsigned)(v), 32, n)
#define ROTATE_LEFT16(v, n)             ROTATE_LEFT((unsigned)(v), 16, n)
#define ROTATE_RIGHT16(v, n)            ROTATE_RIGHT((unsigned)(v), 16, n)
#define ROTATE_LEFT8(v, n)              ROTATE_LEFT((unsigned)(v), 8, n)
#define ROTATE_RIGHT8(v, n)             ROTATE_RIGHT((unsigned)(v), 8, n)

// 字节对齐;
#define ByteAlign(ByteLen, ByteWidth)   (((ByteLen) + (ByteWidth) - 1) / (ByteWidth) * (ByteWidth))

#define MaxBitVal(sz)                   ((sz) > 16 ? ((sz) < 32 ? 0xffffff : 0xffffffff) : (sz > 8 ? 0xffff : 0xff))

//#define GetBitsH8(val, pos, len)        ((pos)+(len)<=8 ? (val)&(1<<pos)&(~(1<<(8-pos-len))) : (val))
#define GetBitsH8(val, pos, len)        ((pos)+(len)<=8 ? ((val) & (0xff>>(pos)) & (0xff<<(8-(pos)-(len)))) : (val))
#define SetBitsH8(val, pos, len)        ((val) |= (pos)+(len)<=8 ? (0xff>>(pos)) & (0xff<<(8-(pos)-(len))) : (val))
#define GetBitsL8(val, pos, len)        ((pos)+(len)<=8 ? ((val) & (0xff<<(pos)) & (0xff>>(8-(pos)-(len)))) : (val))
#define SetBitsL8(val, pos, len)        ((val) |= (pos)+(len)<=8 ? (0xff<<(pos)) & (0xff>>(8-(pos)-(len))) : (val))
//多字节从低位pos位置获取连续len长度的bit位的数值;
#define BitValL(val, pos, len)          (((((val) & 0xffffffff) << (32 - (pos) - (len))) & 0xffffffff) >> (32 - (len)))
//单字节从高位pos位置获取连续len长度的bit位的数值;
#define BitValH(val, pos, len)          ((pos) + (len) <= 8 ? (((((val)&0xff) << (pos)) & 0xff) >> (8 - (len))) : (val))
#define BitValH16(val, pos, len)        (((sizeof(val) >= 2) && ((pos) + (len) <= 16)) ? ((BLSwap16(val) & (0xffff >> pos)) >> (16 - pos - len)) : (val))
#define BitValH32(val, pos, len)        (((sizeof(val) == 4) && ((pos) + (len) <= 32)) ? ((BLSwap32(val) & (0xffffffff >> pos)) >> (32 - pos - len)) : (val))
#if 0
#define BitCopy8BitH(s, sPos, d, dPos, len)    (d |= (((sPos >= 0 && sPos < 8) && (dPos >= 0 && dPos < 8) && (len >= 0 && len < 8) && (sPos + len <= 8)) ? \
                                                    (((8-len) > dPos) ? \
                                                    (BitValH(s, sPos, len) << (8-len-dPos)) : \
                                                    (BitValH(s, sPos, len) >> (dPos-(8-len)))) : \
                                                    0))
#else
#define BitCopy8BitH(s, sPos, d, dPos, len)    \
    (d |= (((sPos >= 0) && (dPos >= 0) && (len >= 0) && (sPos + len <= 8) && (dPos + len <= 8)) ? \
    (BitValH(s, sPos, len) << (8-len-dPos)) : \
    0))
#endif

#ifndef AlignPixel32Bit
// 计算width宽，pixelBit的像素位数，对应的一行RGB数据32位对齐后的字节数据;
#define AlignPixel32Bit(width, pixelBit)   (((long long int)((width) * (pixelBit) + 31) & ~31) >> 3)
#endif

// 高低位翻转，未实现;
//#define LRTurn(a)                       (a << 4 | a >> 4)

#define GetIntByte0(t)          (unsigned char)((t))
#define GetIntByte1(t)          (unsigned char)((t) >> 8)
#define GetIntByte2(t)          (unsigned char)((t) >> 16)
#define GetIntByte3(t)          (unsigned char)((t) >> 24)
#define SetIntByte0(t, c)       ((t) |= (c))
#define SetIntByte1(t, c)       ((t) |= (c) << 8)
#define SetIntByte2(t, c)       ((t) |= (c) << 16)
#define SetIntByte3(t, c)       ((t) |= (c) << 24)

#define GetLLIntByte0(t)        (unsigned char)((t))
#define GetLLIntByte1(t)        (unsigned char)((t) >> 8)
#define GetLLIntByte2(t)        (unsigned char)((t) >> 16)
#define GetLLIntByte3(t)        (unsigned char)((t) >> 24)
#define GetLLIntByte4(t)        (unsigned char)((t) >> 32)
#define GetLLIntByte5(t)        (unsigned char)((t) >> 40)
#define GetLLIntByte6(t)        (unsigned char)((t) >> 48)
#define GetLLIntByte7(t)        (unsigned char)((t) >> 56)
#define SetLLIntByte0(t, c)     ((t) |= (long long)(c))
#define SetLLIntByte1(t, c)     ((t) |= (long long)(c) << 8)
#define SetLLIntByte2(t, c)     ((t) |= (long long)(c) << 16)
#define SetLLIntByte3(t, c)     ((t) |= (long long)(c) << 24)
#define SetLLIntByte4(t, c)     ((t) |= (long long)(c) << 32)
#define SetLLIntByte5(t, c)     ((t) |= (long long)(c) << 40)
#define SetLLIntByte6(t, c)     ((t) |= (long long)(c) << 48)
#define SetLLIntByte7(t, c)     ((t) |= (long long)(c) << 56)

#ifdef MCHECK
#define MCalloc(T, N)                   ((N) > 0 ? (T *)mCalloc((size_t)(N), sizeof(T), __FILE__, __LINE__) : NULL)
#define MMalloc(T, N)                   ((N) > 0 ? (T *)mMalloc(sizeof(T)*N, __FILE__, __LINE__) : NULL)
#define MFree(p)                        if(p){mFree(p); p = NULL;}
#define MCount()                        mCount()
#else
#define MCalloc(T, N)                   ((N) > 0 ? (T *)calloc((size_t)(N), sizeof(T)) : NULL)
#define MMalloc(T, N)                   ((N) > 0 ? (T *)malloc(sizeof(T)*N) : NULL)
#define MFree(p)                        if(p){free(p); p = NULL;}
#define MCount()                        0
#endif


/* 变长数组添加操作 (数组指针，计数器，已分配内存，数据，数据类型，返回值) */
#define C_RESIZE_WIDTH  100
#define CAdd(arrPtr, counter, nAllocated, data, T, width, ret) \
{ T *p; int new_size; ret=-1; \
    if(counter >= nAllocated){ \
        new_size = 2* nAllocated ; \
        if(new_size < (width) ) new_size = width; \
        p = MCalloc(T, new_size); \
        if(p){  \
            memcpy(p, arrPtr, counter*sizeof(T) ); \
            MFree(arrPtr); \
            arrPtr = p; \
            nAllocated = new_size; \
            ret=0; \
        }   \
    }else{ ret=0; } \
    if(ret==0) arrPtr[counter++] = (data); \
}

#define CADD(arrPtr, counter, nAllocated, data, T, ret) \
            CAdd(arrPtr, counter, nAllocated, data, T, C_RESIZE_WIDTH, ret)


#define LogPos(a)  \
if(a){   \
    if(LogFileIsOpen()){ \
        LogFile("Err: Fun:'%s' Failed! File:%s, Line:%d\n", __FUNCTION__, __FILE__, __LINE__);  \
    }else{    \
        printf("Err: Fun:'%s' Failed! File:%s, Line:%d\n", __FUNCTION__, __FILE__, __LINE__);   \
    }   \
}

#define _STRING(x) #x
#define STRING(x) _STRING(x)

#define LogLocation     __FILE__ " :" STRING(__LINE__) " "
#define LogNoFile(str)  LogPos(1); LogFile("Err: File %s Not Exist!\n", str)
#define LogInt(a)       LogFile("%s=%d\n", #a, (a))
#define LogDouble(a)    LogFile("%s=%f\n", #a, (a))
#define LogStr(str)     LogFile("%s=%s\n", #str, str)

//int ntob(long long a, char *buf);

#define ntob(a, str, num)   \
{                           \
    int aBitNum = sizeof((a)) * 8;   \
    int loopNum = 1;        \
    int shift = 0;          \
    int mod = aBitNum;      \
    char tmp[64] = {0};     \
    char *pbuf = (char *)(str);       \
    if(aBitNum > 32){loopNum = 2; shift = 32; mod = 32;} \
    int bitNum = 0;            \
    while(loopNum-- > 0){    \
        int aint = (a) >> shift;              \
        /*printf(#a"=0x%llx, aint=%x, shift=%d ", a, aint, shift);*/  \
        itoa(aint, tmp, 2);         \
        int len = strlen(tmp);      \
        int cnt = len % mod;        \
        /*printf("len=%d, cnt=%d\n", len, cnt);*/   \
        if(cnt != 0){               \
            cnt = mod - cnt;        \
            memset(pbuf, '0', cnt); \
            pbuf += cnt;            \
        }   \
        memcpy(pbuf, tmp, len);     \
        pbuf += len;                \
        bitNum += cnt + len;        \
        shift-=32;  \
    }   \
    if(aBitNum < bitNum){memcpy((str), (str)+bitNum-aBitNum, aBitNum); (str)[aBitNum] = 0;}  \
    num = Min(bitNum, aBitNum);   \
}

#define PrintBitNum(a)  \
{   \
    char aName[8] = {' '}; \
    char str[128] = {0};  \
    int len = 0;    \
    memset(aName, ' ', sizeof(aName));  \
    len = snprintf(aName, sizeof(aName), "%s", #a);  \
    if(len < sizeof(aName)) aName[len] = ' ';   \
    aName[7] = 0;       \
    printf("%s:", aName);  \
    if(sizeof(a) <= 4) \
        printf("0x%08x ", a);  \
    else    \
        printf("0x%016llx ", a);  \
    ntob(a, str, len); \
    /*printf(#a"=%d, len=%d, %s\n", a, len, str);*/    \
    for(int i=0; i<len; i++){   \
        if(i%8==0 && i!=0) printf(" ");   \
        printf("%c", str[i]);   \
    }   \
    printf("\n");   \
}

#define PrintBitChar(cp, num)  \
{   \
    char aName[8] = {' '}; \
    char str[10] = {0};  \
    int len = 0;    \
    char *ap = (char *)cp;   \
    memset(aName, ' ', sizeof(aName));  \
    len = snprintf(aName, sizeof(aName), "%s", #cp);  \
    if(len < sizeof(aName)) aName[len] = ' ';   \
    aName[7] = 0;       \
    printf("%s:", aName);  \
    for(int i=0; i<num; i++){   \
        ntob(ap[i], str, len); \
        /*printf(#a"=%d, len=%d, %s\n", a, len, str);*/    \
        printf("%s ", str);     \
    }   \
    printf("\n");   \
}

#ifdef M_MSVC
#define snprintf    _snprintf
#define access      _access
#endif



#ifdef __cplusplus
extern "C" {
#endif


ALGAPI void Pause();
// 剥夺尾部空格、回车、换行符;
ALGAPI void CStrip(char *s);

// 获取当前进程、线程ID;
ALGAPI int GetPID();
ALGAPI int GetTID();

ALGAPI char *GetEnv(const char *name);
ALGAPI int SetEnv(const char *name, const char *value);

ALGAPI const char* GetDate(int *year, int *month, int *day);
ALGAPI const char *GetTime();
// clock
ALGAPI unsigned int ClockMSec();
ALGAPI void MsSleep(unsigned int ms);
// timer
/** 获取当前时钟
 *  @return
 *          - 时钟，单位秒（精度10e-6）
*/
ALGAPI double Timer(void);

/*------------------------------------------------------------
 *  MakeDir
 *------------------------------------------------------------
 */
/** 创建目录
 *  @param[in] dirname      = 目录名称（可包含路径）
 *  @return
 *          -  0, 创建成功。
 *          - -1, 创建失败，目录已经存在。
 *          - -2, 创建失败，原因：路径不存在、无法访问。
*/
ALGAPI int MakeDir(const char* dirname);
/* 删除空目录;
 * Return:
 *  0       成功;
 *  -1      失败;
*/
ALGAPI int RmDir(const char *dir);
/* 删除文件;
 * Return:
 *  0       成功;
 *  -1      失败;
*/
ALGAPI int DelFile(const char*filename);

const char* CBaseName(const char *path);

typedef struct CDir_t{
    int num,nAlloc;
    char **path;
}CDir;
void CDirFree(CDir *dir);

/* 获取目录及文件;
 * Input:
 *  path        搜索路径;
 *  dir         目录列表;
 *  mode        获取模式：0 目录和文件，1 目录，2 文件;
*/
int GetDirs(const char *path, CDir *dir, int mode);
/* 删除目录及其文件;
 * Return:
 *  0       成功;
 *  -1      失败;
*/
int DelDir(const char *dir);

// 文件权限判断;
ALGAPI int PathIsExist(const char *pathname);
ALGAPI int FileCanRead(const char *pathname);
ALGAPI int FileCanWrite(const char *pathname);
ALGAPI int FileCanRW(const char *pathname);
ALGAPI void GetFileDir(const char *path, int size, char buf[]);
ALGAPI void GetFileName(const char *path, int size, char buf[]);
ALGAPI void GetNoExtFileName(const char *path, int size, char buf[]);
ALGAPI void GetFileExtName(const char *path, int size, char buf[]);
ALGAPI char *GetCWD(char *buf, int sz);

// log
ALGAPI void SetLogInfoType(int type);
ALGAPI int GetLogInfoType(void);
ALGAPI void LogFileOpen(const char *fileName);
ALGAPI void LogFileClose();
ALGAPI void LogFile(const char * fmt, ...);
#define LOGFILE(...)    LogFile(LogLocation __VA_ARGS__)
ALGAPI int LogFileIsOpen();
ALGAPI void *GetLogFileHandle();
ALGAPI void *ReOpenLogFile(const char *filename);


ALGAPI void *mCalloc(int count, int size, const char *fileName, int lineNum);
ALGAPI void *mMalloc(int size, const char *fileName, int lineNum);
ALGAPI void mFree(void *p);
ALGAPI int mCount();
ALGAPI void OpenMemFile(const char *file);
ALGAPI void CloseMemFile();
ALGAPI void *MemMapAdd(void *p, const char*file, int line);
ALGAPI void MemMapDelete(void *p);
ALGAPI int MemAnalyze(const char *file);

/* 内存bit拷贝;
 * Input:
 *  dst:                目的内存地址;
 *  dstBitLen:          目的内存bit长度;
 *  dstStartBit:        目的内存开始拷贝bit位置;
 *  src:                原始内存地址;
 *  srcBitLen:          原始内存bit长度;
 *  srcStartBit:        原始内存开始拷贝bit位置;
 *  bitLen:             拷贝长度;
 * Output:
 *  dst:
 * Return:
 *  0       失败;
 *  1       成功;
*/
ALGAPI int BitMemCopy(void *dst, int dstBitLen, int dstStartBit, void *src, int srcBitLen, int srcStartBit, int bitLen);

/* 重置bit位
 * Input:
 *  buf             数据内存地址;
 *  bufLen          数据字节长度;
 *  startBit        重置开始bit位;
 *  bitLen          重置数据bit位长度;
 *  val             重置值：0 或 1;
 * Return:
 *  0       失败;
 *  1       成功;
*/
ALGAPI int BitMemSet(void *buf, int bufLen, int startBit, int bitLen, char val);


#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

#ifdef MCHECK
#define MNew(T)                      (T *)MemMapAdd(new T, __FILE__, __LINE__);
#define MDelete(p)                   if(p){MemMapDelete(p); delete (p); (p) = (nullptr);}
#define MDeleteV(T, p)               if(p){MemMapDelete(p); delete ((T*)p); (p) = (nullptr);}
#define MNewArr(T, N)                (T *)MemMapAdd(new T[N], __FILE__, __LINE__);
#define MDeleteArr(p)                if(p){MemMapDelete(p); delete [] (p); (p) = (nullptr);}
#else
#define MNew(T)                      new T
#define MDelete(p)                   if(p){delete p; p = nullptr;}
#define MDeleteV(T, p)               if(p){T*pp=(T*)p; delete (pp); (p) = (nullptr);}
#define MNewArr(T, N)                (T *)(new T[N]);
#define MDeleteArr(p)                if(p){delete [] (p); (p) = (nullptr);}
#endif // MCHECK

#define PrintVal(str) cout << #str << " = " << str << endl

#else

#define MNew(T)                      MCalloc(T, 1)
#define MDelete(p)                   MFree(p)

#endif //__cplusplus

#endif // CCOMMON

