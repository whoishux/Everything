/******************************************************
*?Program?Assignment:   ��������;
*?Author:               Hux
*?Date:                 2018/01/01
*?Description:          ���ֹ����ຯ�������Ż�;
* *****************************************************/

#include "cinclude.h"


#ifndef M_GNU
#include <Windows.h>
//#include <synchapi.h>
//#include <processthreadsapi.h>
#include <process.h>
#include <direct.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#else
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
#include <limits.h>
#include <pthread.h>
#endif

#include <errno.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "ccommon.h"

//#define USEMEMANALYZE



int GetPID()
{
#ifndef M_GNU
    //return _getpid();
    return (int)GetCurrentProcessId();
#else
    return getpid();
#endif
}

int GetTID()
{
#ifndef M_GNU
    return (int)GetCurrentThreadId();
#else
    return (int)pthread_self();
#endif
}

char *GetEnv(const char *name)
{
    return getenv(name);
}

int SetEnv(const char *name, const char *value)
{
    int ret = -1;
    size_t len = 0;
    char *buf = NULL;

    len = (strlen(name) + strlen(value)) * 2;
    buf = (char*)calloc(len, 1);
    if(!buf) return -1;

    snprintf(buf, len, "%s=%s;", name, value);
#ifndef M_GNU
    ret = _putenv(buf);
#else
    ret = putenv(buf);
#endif

    if(buf) free(buf);
    return ret;
}

const char* GetTime()
{
    int len;
    time_t timep;
    static char tStr[30] = {0};

    time(&timep);
#if 0
    strcpy(tStr, ctime(&timep));
#else
    struct tm *tp = NULL;
    tp = localtime(&timep);
    snprintf(tStr, sizeof(tStr), "%d-%d-%d %d:%d:%d",
            tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday,
            tp->tm_hour, tp->tm_min, tp->tm_sec
            );
#endif

    len = strlen(tStr);
    if(len > 0 && tStr[len - 1] == '\n'){
        tStr[len - 1] = 0;
    }

    return tStr;
}

const char* GetDate(int *year, int *month, int *day)
{
    int len;
    time_t timep;
    static char tStr[30] = {0};

    if(!year || !month || !day) return NULL;

    time(&timep);

    struct tm *tp = NULL;
    tp = localtime(&timep);
    snprintf(tStr, sizeof(tStr), "%04d-%02d-%02d",
            tp->tm_year+1900, tp->tm_mon+1, tp->tm_mday);

    *year = tp->tm_year + 1900;
    *month = tp->tm_mon + 1;
    *day = tp->tm_mday;

    len = strlen(tStr);
    if(len > 0 && tStr[len - 1] == '\n'){
        tStr[len - 1] = 0;
    }

    return tStr;
}


unsigned int ClockMSec()
{
    clock_t t = clock();

    return (unsigned int)t;
}

static int LogInfoType = 0;
void SetLogInfoType(int type)
{
    LogInfoType = type;
}

int GetLogInfoType(void)
{
    return LogInfoType;
}

static FILE *LogFp = NULL;
void LogFileOpen(const char *fileName)
{
    if (!LogFp && fileName){
        LogFp = fopen(fileName, "w");
    }
}

void LogFileClose()
{
    if (LogFp){
        fclose(LogFp);
        LogFp = NULL;
    }
}

void LogFile(const char * fmt, ...)
{
    int size = 0;
    char buf[1024] = { 0 };
    int len = sizeof(buf);

    if(LogInfoType <= 0) return ;

    if(LogInfoType >= 2){
        snprintf(buf, len, "TID:%d, ", GetTID());
        size = strlen(buf);
        snprintf(buf+size, len-size, "%s: ", GetTime());
        size = strlen(buf);
    }
    if(fmt){
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf+size, sizeof(buf)-size, fmt, args);
        va_end(args);

        if (LogFp){
            fprintf(LogFp, buf);
            fflush(LogFp);
        }else{
//            fprintf(stdout, buf);
//            fflush(stdout);
            printf("%s", buf);
            fflush(stdout);
        }
    }
}

int LogFileIsOpen()
{
    if(LogFp)
        return 1;
    else
        return 0;
}


void *GetLogFileHandle()
{
    return (void*)LogFp;
}

void *ReOpenLogFile(const char *filename)
{
    static FILE *fp = NULL;

    if(filename){
        fp = LogFp;
        LogFp = NULL;
        LogFileOpen(filename);
    }else{
        if(LogFp) LogFileClose();
        if(fp) LogFp = fp;
        fp = NULL;
    }

    return fp;
}


/*----------------------------------------------------------------------
 *  Timer
 *----------------------------------------------------------------------
*/
/** ��ȡ��ǰʱ��
 *
 *  @param[in]
 *
 *  @return
 *          - ʱ�ӣ���λ�루����10e-6��
 *
 *  @note
 */
double Timer(void)
{
    double tm=0.0;
#ifndef M_GNU
    struct _timeb t1;
    _ftime(&t1);
    tm=t1.millitm/1000000.0 + t1.time;
#else
    int ret;
    struct timeval t1;
    ret=gettimeofday(&t1,NULL);
    if(ret<0) return tm;
    tm=t1.tv_usec/1000000.0 + t1.tv_sec;
#endif
        return tm;
}


/*------------------------------------------------------------
 *  MakeDir
 *------------------------------------------------------------
 */
/** ����Ŀ¼
 *
 *  @param[in] dirname      = Ŀ¼���ƣ��ɰ���·����
 *
 *  @return
 *          -  0, �����ɹ���
 *          - -1, ����ʧ�ܣ�Ŀ¼�Ѿ����ڡ�
 *          - -2, ����ʧ�ܣ�ԭ��·�������ڡ��޷����ʡ�
 *
 *  @note
*/
int MakeDir(const char* dirname)
{
#ifndef M_GNU  /*WIN32*/
  if( _mkdir(dirname) == -1){ /* failed */
    switch(errno){
    case EEXIST: return -1;
    case ENOENT: return -2;
    }
  }
#else /* UNIX, Linux */
  if( mkdir(dirname) == -1){ /* failed */
    switch(errno){
    case EEXIST: return -1;
    default: /* ENOTDIR, EACCES, ENAMETOOLONG, ENOENT, ... */
      return -2;
    }
  }
#endif
  return 0;
}

/* ɾ����Ŀ¼;
 * Return:
 *  0       �ɹ�;
 *  -1      ʧ��;
*/
int RmDir(const char *dir)
{
#ifndef M_GNU
    return _rmdir(dir);
#else
    return rmdir(dir);
#endif
}

/* ɾ���ļ�;
 * Return:
 *  0       �ɹ�;
 *  -1      ʧ��;
*/
int DelFile(const char*filename)
{
#ifndef M_GNU
    return remove(filename);
#else
    return unlink(filename);
#endif
}

const char* CBaseName(const char *path)
{
    const char *s, *s2;
    const char *basename=NULL;

    s = strrchr(path, '/');
    s2 = strrchr(path, '\\');
    if(s==NULL) s=s2;

    if(s){
        basename = s+1;
    }else{
        basename = path;
    }

    return basename;
}

#define PATH_MAX_SIZE   1024

void CDirFree(CDir *dir)
{
    if(!dir){
        for(int i=0; i<dir->num; i++){
            MFree(dir->path[i]);
        }
        MFree(dir->path);
        memset(dir, 0, sizeof(CDir));
    }
}

/* ��ȡĿ¼���ļ�;
 * Input:
 *  path        ����·��;
 *  dir         Ŀ¼�б�;
 *  mode        ��ȡģʽ��0 Ŀ¼���ļ���1 Ŀ¼��2 �ļ�;
 * Return:
 *  0           �ɹ�;
 *  -1          ʧ��;
*/
int GetDirs(const char *path, CDir *dir, int mode)
{
    int ret = -1;
    intptr_t hFile  =  0;
    char dirpath[PATH_MAX_SIZE] = {0};
    char dirpath1[PATH_MAX_SIZE] = {0};
    struct _finddata_t fileinfo;
    int fileAttrMask = 0;
    int len = 0;

    if(!path || !dir) return -1;

    len = snprintf(dirpath, PATH_MAX_SIZE, "%s", path);
//    printf("len=%d, dirpath=%s\n", len, dirpath);
    if(len > 0 && (dirpath[len-1] == '\\' || dirpath[len-1] == '/')) dirpath[len-1] = 0;
    sprintf(dirpath1, "%s/*", dirpath);
//    printf("path1=%s\n", dirpath1);

    switch (mode) {
    case 0:
        fileAttrMask = 0xffffffff;
        break;
    case 1:
        fileAttrMask = _A_SUBDIR;
        break;
    case 2:
        fileAttrMask = ~_A_SUBDIR;
        break;
    default:
        LogPos(1);
        return -1;
    }

    if((hFile = _findfirst(dirpath1, &fileinfo)) != -1){
        do{
//            printf("dir.num=%d\n", dir->num);
            if(fileinfo.attrib & fileAttrMask){
                if(strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0){
                    char *tmp = MCalloc(char, PATH_MAX_SIZE);
                    if(!tmp){LogPos(1); goto done;}
                    snprintf(tmp, PATH_MAX_SIZE,"%s/%s", dirpath, fileinfo.name);
                    CADD(dir->path, dir->num, dir->nAlloc, tmp, char *, ret);
                }
            }
        }while(_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }

    ret = 0;
done:
    if(ret < 0){
        CDirFree(dir);
    }
    return ret;
}

/* ɾ��Ŀ¼�����ļ�;
 * Return:
 *  0       �ɹ�;
 *  -1      ʧ��;
*/
int DelDir(const char *dir)
{
    int ret = -1;
    CDir paths, files;

    if(!dir) return -1;
    if(!PathIsExist(dir)) return -1;

    memset(&paths, 0, sizeof(CDir));
    memset(&files, 0, sizeof(CDir));

    // ��ȡ��Ŀ¼��ɾ����Ŀ¼�����ļ�;
    GetDirs(dir, &paths, 1);
    for(int i=0; i<paths.num; i++){
        ret = DelDir(paths.path[i]);
        if(ret < 0){LogPos(1); goto done;}
    }

    // ��ȡ��ǰĿ¼���ļ���ɾ��;
    GetDirs(dir, &files, 2);
    for(int i=0; i<files.num; i++){
        ret = DelFile(files.path[i]);
        if(ret < 0){LogPos(1); goto done;};
    }

    // ɾ����ǰĿ¼;
    ret = RmDir(dir);

done:
    CDirFree(&paths);
    CDirFree(&files);
    return ret;
}


void MsSleep(unsigned int ms)
{
#ifndef M_GNU
    Sleep(ms);
#else
    while(ms >= 1000){
        usleep(999999);
        ms -= 999;
    }
    usleep(1000*ms);

#endif
}

int PathIsExist(const char *pathname)
{
    return access(pathname, 0) == 0;
}

int FileCanRead(const char *pathname)
{
    return access(pathname, 4) == 0;
}

int FileCanWrite(const char *pathname)
{
    return access(pathname, 2) == 0;
}

int FileCanRW(const char *pathname)
{
    return access(pathname, 6) == 0;
}

void GetFileDir(const char *path, int size, char buf[])
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    _splitpath(path, drive, dir, fname, ext);
    memset(buf, 0, size);
    snprintf(buf, size, "%s%s", drive, dir);
}

void GetFileName(const char *path, int size, char buf[])
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    memset(buf, 0, size);
    _splitpath(path, drive, dir, fname, ext);
    snprintf(buf, size, "%s%s", fname, ext);
}

void GetNoExtFileName(const char *path, int size, char buf[])
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    memset(buf, 0, size);
    _splitpath(path, drive, dir, fname, ext);
    snprintf(buf, size, "%s", fname);
}


void GetFileExtName(const char *path, int size, char buf[])
{
    char drive[_MAX_DRIVE];
    char dir[_MAX_DIR];
    char fname[_MAX_FNAME];
    char ext[_MAX_EXT];

    memset(buf, 0, size);
    _splitpath(path, drive, dir, fname, ext);
    // ������.����;
    snprintf(buf, size, "%s", ext+1);
}


char *GetCWD(char *buf, int sz)
{
    return _getcwd(buf, sz);
}


#ifdef USEMEMANALYZE
#include <map>
using namespace std;
map<void *, int> MemMap;
FILE *MemFp = NULL;

#include "ccounter.h"
CCounter MemId = 0;
CCounter gMCount(0);


void OpenMemFile(const char *file)
{
    if(file) MemFp = fopen(file, "w");
}

void MemFilePrint(const char *file, int linenum, int id)
{
    if(MemFp){
        fprintf(MemFp, "MId:%d, F:%s, L:%d\n", id, file, linenum);
        fflush(MemFp);
    }
}

void CloseMemFile()
{
    if(MemFp){
        fclose(MemFp);
        MemFp = NULL;
    }
}

int mCount()
{
    return gMCount.Value();
}

void *mCalloc(int count, int size, const char *fileName, int lineNum)
{
    void *p = NULL;

    p = mMalloc(count * size, fileName, lineNum);
    if(p) memset(p, 0, count * size);
    return p;
}

void *mMalloc(int size, const char *fileName, int lineNum)
{
    void *p = NULL;

    gMCount++;
    p = malloc(size+4);
    if(p){
        int id = (++MemId);
        ((int*)p)[0] = id;
        if(MemFp){
            fprintf(MemFp, "MId:%d, F:%s, L:%d\n", id, fileName, lineNum);
            fflush(MemFp);
        }
        return (char*)p + 4;
    }else{
        return NULL;
    }
}

void mFree(void *p)
{
    int id;
    if(p){
        gMCount--;
        id = ((int*)((char*)p-4))[0];
        if(MemFp){fprintf(MemFp, "FMId:%d\n", id); fflush(MemFp);}
        free((char*)p-4);
    }
}

void* MemMapAdd(void *p, const char *file, int line)
{
    if(p){int id;
        gMCount++;
        id = (++MemId);
        MemMap[p] = id;
        MemFilePrint(file, line, id);
    }
    return p;
}

void MemMapDelete(void *p)
{
    if(p){
        int id = 0;
        gMCount--;
        id = MemMap[p];
        if(MemFp){
            fprintf(MemFp, "FMId:%d\n", id);
            fflush(MemFp);
        }
        MemMap.erase(p);
    }
}


int MemAnalyze(const char *file)
{
    map<int, char> Mmap;
    FILE *fp = NULL;
    char buf[1024] = {0};
    int id;
    int readNum = 0;
    int MNum = 0;
    int FMNum = 0;

    if(!file) file = "mem.txt";

    fp = fopen(file, "r");
    if(!fp){LogFile("open failed!\n"); return -1;}

    while(fgets(buf, sizeof(buf), fp)){
        readNum++;
        if(strncmp(buf, "MId:", 4) == 0){
            sscanf(buf, "MId:%d", &id);
            Mmap[id] = 1;
            MNum++;
        }else if(strncmp(buf, "FMId:", 5) == 0) {
            sscanf(buf, "FMId:%d", &id);
            if(Mmap.find(id) != Mmap.end())
                Mmap.erase(id);
            else
                Mmap[id] = -1;
            FMNum++;
        }
    }

    LogFile("mmap.size=%d, readNum=%d, MNum=%d, FMNum=%d\n", Mmap.size(), readNum, MNum, FMNum);
    for(map<int, char>::iterator p = Mmap.begin(); p != Mmap.end(); p++){
        LogFile("Id=%d, v=%d\n", p->first, p->second);
    }

    return 0;
}

#else
int gMCount = 0;
int mCount()
{
    return gMCount;
}

void *mCalloc(int count, int size, const char *, int )
{
    gMCount++;
    return calloc(count, size);
}

void *mMalloc(int size, const char *, int )
{
    gMCount++;
    return malloc(size);
}

void mFree(void *p)
{
    gMCount--;
    free(p);
}

void OpenMemFile(const char *)
{
}

void CloseMemFile()
{
}

void MemFilePrint(const char *, int )
{
}

void *MemMapAdd(void *p, const char*, int )
{
    if(p) gMCount++;
    return p;
}

void MemMapDelete(void *p)
{
    if(p) gMCount--;
}

int MemAnalyze(const char *file)
{
    return 0;
}

#endif

/* �ڴ�bit�����������ڴ�����;
 * Input:
 *  dst:                Ŀ���ڴ��ַ;
 *  dstBitLen:          Ŀ���ڴ�bit����;
 *  dstStartBit:        Ŀ���ڴ濪ʼ����bitλ��;
 *  src:                ԭʼ�ڴ��ַ;
 *  srcBitLen:          ԭʼ�ڴ�bit����;
 *  srcStartBit:        ԭʼ�ڴ濪ʼ����bitλ��;
 *  bitLen:             ��������;
 * Output:
 *  dst:
 * Return:
 *  0       ʧ��;
 *  1       �ɹ�;
*/
int BitMemCopy(void *dst, int dstBitLen, int dstStartBit, void *src, int srcBitLen, int srcStartBit, int bitLen)
{
    char *dstP = (char*)dst;
    char *srcP = (char*)src;
    int dStartIdx, dEndIdx;
    int sStartIdx, sEndIdx;
    int dBitCopyLen, sBitcopyLen;
    int dStartBit, sStartBit;
    const int CopyCell = 8;
    const int moveval = 3;
    int len;

    if(!dst || !src){LogPos(1); return 0;}
    if(dstStartBit + bitLen > dstBitLen){LogPos(1); return 0;}
    if(srcStartBit + bitLen > srcBitLen){LogPos(1); return 0;}

//    LogFile("dstBitLen=%d, dstStartBit=%d, srcBitLen=%d, srcStartBit=%d, bitLen=%d\n", dstBitLen, dstStartBit, srcBitLen, srcStartBit, bitLen);

//    memset(dst, 0, dstBitLen>>3);
    dStartIdx = dstStartBit >> moveval;
    dEndIdx = (dstStartBit + bitLen) >> moveval;
    dStartBit = dstStartBit - (dStartIdx << moveval);
    dBitCopyLen = CopyCell - dStartBit;
    sStartIdx = srcStartBit >> moveval;
    sEndIdx = (srcStartBit + bitLen) >> moveval;
    sStartBit = srcStartBit - (sStartIdx << moveval);
    sBitcopyLen = CopyCell - sStartBit;
    len = Min(sBitcopyLen, dBitCopyLen);
    len = Min(len, bitLen);

//    LogFile("sStartIdx=%d, sEndIdx=%d, sStartBit=%d, sBitcopyLen=%d\n", sStartIdx, sEndIdx, sStartBit, sBitcopyLen);
//    LogFile("dStartIdx=%d, dEndIdx=%d, dStartBit=%d, dBitCopyLen=%d\n", dStartIdx, dEndIdx, dStartBit, dBitCopyLen);

    if(sStartBit == dStartBit){
        int sEndBitLen = (dstStartBit + bitLen) - (dEndIdx << moveval);
        if(sEndIdx > sStartIdx){
            int byteLen = sEndIdx - sStartIdx - 1;
            BitCopy8BitH(srcP[sStartIdx], sStartBit, dstP[dStartIdx], dStartBit, len);
            if(byteLen > 0) memcpy(dstP+dStartIdx+1, srcP+sStartIdx+1,  byteLen);
            BitCopy8BitH(srcP[sEndIdx], 0, dstP[dEndIdx], 0, sEndBitLen);
        }else{
            BitCopy8BitH(srcP[sStartIdx], sStartBit, dstP[dStartIdx], dStartBit, len);
        }
    }else{
        while(bitLen > 0){
            BitCopy8BitH(srcP[sStartIdx], sStartBit, dstP[dStartIdx], dStartBit, len);
//            printf("srcP[%d]=%x, dstP[%d]=%x, sStartBit=%d, dStartBit=%d, len=%d\n",
//                   sStartIdx, srcP[sStartIdx]&0xff, dStartIdx, dstP[dStartIdx]&0xff, sStartBit, dStartBit, len);
            dStartBit += len;
            sStartBit += len;
            bitLen -= len;

            if(dStartBit >= CopyCell){dStartBit = 0; dStartIdx++;}
            if(sStartBit >= CopyCell){sStartBit = 0; sStartIdx++;}
            dBitCopyLen = CopyCell - dStartBit;
            sBitcopyLen = CopyCell - sStartBit;
            len = Min(sBitcopyLen, dBitCopyLen);
            len = Min(len, bitLen);
        }
    }


    return 1;
}

/* ����bitλ
 * Input:
 *  buf             �����ڴ��ַ;
 *  bufLen          �����ֽڳ���;
 *  startBit        ���ÿ�ʼbitλ;
 *  bitLen          ��������bitλ����;
 *  val             ����ֵ��0 �� 1;
 * Return:
 *  0       ʧ��;
 *  1       �ɹ�;
*/
int BitMemSet(void *buf, int bufLen, int startBit, int bitLen, char val)
#if 0
{
    int sIdx, eIdx;
    int sBit, eBit;
    char bVal = 0;
    char *cBuf = (char *)buf;

    if(!buf){LogPos(1); return 0;}
    if(startBit < 0 || bitLen < 0){LogPos(1); return 0;}
    if(startBit + bitLen > bufLen * 8){LogPos(1); return 0;}
    if(val != 0 && val != 1){LogPos(1); return 0;}

    sIdx = startBit >> 3;
    eIdx = (startBit + bitLen) >> 3;
    sBit = startBit % 8;
    eBit = (startBit + bitLen) % 8;
    if(val == 1) bVal = 0xff;

//    LogFile("startBit=%d, bitLen=%d, sIdx=%d, sBit=%d, eIdx=%d, eBit=%d, val=%d\n", startBit, bitLen, sIdx, sBit, eIdx, eBit, val);

#if 1
    if(eIdx > sIdx){
        memset(cBuf+sIdx+1, bVal, eIdx-sIdx-1);

        if(val == 0){
            cBuf[sIdx] &= 0xff << (8 - sBit);
            cBuf[eIdx] &= 0xff >> eBit;
        }else{
            cBuf[sIdx] |= 0xff >> sBit;
            cBuf[eIdx] |= 0xff << (8 - eBit);
        }
    }else{
        if(val == 0){
            cBuf[sIdx] &= (0xff << (8 - sBit)) | (0xff >> eBit);
        }else{
            cBuf[sIdx] |= (0xff << sBit) & (0xff >> (8 - eBit));
        }
    }
#else
    if(eIdx > sIdx){
        memset(cBuf+sIdx+1, bVal, eIdx-sIdx-1);

        if(val == 0){
            for(int i=sBit; i<8; i++)
                HCLR_BIT(cBuf[sIdx], i);
            for(int i=0; i<eBit; i++)
                HCLR_BIT(cBuf[eIdx], i);
        }else{
            for(int i=sBit; i<8; i++)
                HSET_BIT(cBuf[sIdx], i);
            for(int i=0; i<eBit; i++)
                HSET_BIT(cBuf[eIdx], i);
        }

    }else{
        if(val == 0){
            for(int i=sBit; i<eBit; i++)
                HCLR_BIT(cBuf[sIdx], i);
        }else{
            for(int i=sBit; i<eBit; i++)
                HSET_BIT(cBuf[sIdx], i);
        }
    }
#endif

    return 1;
}
#else
{
    int sIdx, eIdx;
    int sBit, eBit;
    char *cBuf = (char *)buf;

    if(!buf){LogPos(1); return 0;}
    if(startBit < 0 || bitLen < 0){LogPos(1);return 0;}
    if(startBit + bitLen > bufLen * 8){LogPos(1);return 0;}

    sIdx = startBit >> 3;
    eIdx = (startBit + bitLen) >> 3;
    sBit = startBit % 8;
    eBit = (startBit + bitLen - 1) % 8;

//    LogFile("startBit=%d, bitLen=%d, sIdx=%d, sBit=%d, eIdx=%d, eBit=%d, val=%d\n", startBit, bitLen, sIdx, sBit, eIdx, eBit, val);

    if(eIdx > sIdx){
        memset(cBuf+sIdx+1, val, eIdx-sIdx-1);

        cBuf[sIdx] = (cBuf[sIdx] & ~((0xff >> sBit))) | ((0xff >> sBit) & val);
        cBuf[eIdx] = (cBuf[eIdx] & ~(0xff << (7 - eBit))) | ((0xff << (7 - eBit)) & val);
    }else{
        cBuf[sIdx] = (cBuf[sIdx] & ~((0xff >> sBit) & (0xff << (7 - eBit)))) | (val & (0xff >> sBit) & (0xff << (7 - eBit)));
    }

    return 1;
}
#endif


void Pause()
{
    system("pause");
}

#if 0
int ntob(long long a, char *buf)
{
    int cnt = 1;
    int shift = 0;
    int mod = 8;
    int num = 0;
    char tmp[64] = {0};
    char *pbuf = buf;

    if(!buf) return 0;

    if(sizeof(a) > 4){
        cnt = 2;
        shift = 32;
    }

    for(int i=0; i<cnt; i++, shift-=32){
        int aint = a >> shift;
//        printf("a=0x%llx, aint=%x, shift=%d\n", a, aint, shift);
        if(aint == 0) continue;
        itoa(aint, tmp, 2);
        int len = strlen(tmp);
        int cnt = len % mod;
        if(cnt != 0){
            cnt = mod - cnt;
            memset(pbuf, '0', cnt);
            pbuf += cnt;
        }
        memcpy(pbuf, tmp, len);
        pbuf += len;
        num += cnt + len;
        mod = 32;
    }

    return num;
}
#endif

// ����β���ո񡢻س������з�;
void CStrip(char *s)
{
    int i,n;
    n = strlen(s);
    for(i=n-1; i>=0; i--){
        if(s[i]=='\r' || s[i]=='\n' || s[i]==' '){
            s[i]=0;
            continue;
        }
        break;
    }
}
