#ifndef CMATH_H
#define CMATH_H

#include <vector>
#include <math.h>

using std::vector;

/******************** MacroFunc **************************/
// 关闭最靠右的位元（0101 1110=>0101 1100），可判断无符号整数是不是2的幂或0，即结果是否为0;
#define TurnOffLastRightBit(x)          ((x) & ((x)-1))
// 打开最靠右的位元（0101 0111=>0101 1111）;
#define TurnOnLastRightBit(x)           ((x) | ((x)+1))
// 关闭字组尾部的1（0101 0111=>0101 0000），可判断无符号整数是不是2^n-1或0，也可判断某个数的所有位元是否均为1，即结果是否为0;
#define TurnOffTailBit(x)               ((x) & ((x)+1))
// 打开字组尾部的0（0101 1000=>0101 1111）;
#define TurnOnTailBit(x)                ((x) | ((x)-1))
// 转换x最靠右且值为0的位于变为1，其余位元置0（1010 0111=>0000 1000），若x中没有0的位元，则结果为0;
#define TrunLastRightBit0(x)            (~(x) & ((x)+1))
// 转换x最靠右且为1的位元变为0，其余位元置1（1010 1000=>1111 0111），若x中没有1的位元，则结果的每个位元均是1;
#define TrunLastRightBit1(x)            (~(x) | ((x)-1))
// 转换x尾部所有为0的位元变为1，其余位元置0（1010 0111=>1111 1000），若x没有0的位元，则结果为0;
#define TrunTailBit0(x)                 (~(x) & ((x)-1))
// 转换x尾部所有为1的位元变为0，其余位元置1（1010 0111=>1111 1000），若x没有1的位元，则结果的每个位元均是1;
#define TrunTailBit1(x)                 (~(x) | ((x)+1))
// 获取x最靠右且为1的位元，其余位元置0（0101 1000=>0000 1000），若x没有1的位元，则结果为0;
#define GetLastRightBit1(x)             ((x) & (~(x)+1))
// 获取x最靠右且为0的位元，其余位元置1（0101 0111=>1111 0111），若x没有0的位元，则结果为1;
#define GetLastRightBit0(x)             ((x) | (~(x)-1))
// 获取x最靠右且为1的位元及其右侧均置1，其左侧均置0（0101 1000=>0000 1111），若x没有1的位元，则结果的每个位元均是1;
#define GetTailBit1(x)                  ((x) ^ ((x)-1))
// 获取x最靠右且为0的位元及其右侧均置1，其左侧均置0（0101 0111=>0000 1111），若x没有0的位元，则结果的每个位元均是1;
#define GetTailBit0(x)                  ((x) ^ ((x)+1))
// 设置x右侧连续出现且为1的位元置0（0101 1100=>0100 0000）; /* ((((x) & -(x)) + (x)) & (x)) */
#define TurnOffRightContinueBit(x)      ((((x) | ((x)-1)) + 1) & (x))
// 交换不同寄存器中相应的位段;
#define SwapBitField(x, y, mask)        {(x) = (x) ^ (y); (y) = (y) ^ ((x) & (mask)); (x) = (x) ^ (y);}

//绝对值函数;
#define ABS(x)                          (((x) ^ ((signed)(x) >> 31)) - ((signed)(x) >> 31))
#define NABS(x)                         (((signed)(x) >> 31) - ((x) ^ ((signed)(x) >> 31)))
// 平均值函数;
#define AVEF(x, y)                      (((x) & (y)) + ((unsigned)((x) ^ (y)) >> 1))
#define AVEC(x, y)                      (((x) | (y)) - (((x) ^ (y)) >> 1))
// 符号函数;
#define SIGN(x)                         (((x) >> 31) | ((unsigned)(-(x)) >> 31))
// 比较函数;
#define CMP(x, y)                       (((x) > (y)) - ((x) < (y)))

// 二进制转格雷码;
#define B2G(b, g)                       (g) = ((b) ^ ((unsigned)(b) >> 1))
#define G2B(g, b)   \
{   \
    (b) = (g) ^ ((unsigned)(g) >> 1);   \
    (b) = (b) ^ ((unsigned)(b) >> 2);   \
    (b) = (b) ^ ((unsigned)(b) >> 4);   \
    if(sizeof(g) > 1) (b) = (b) ^ ((unsigned)(b) >> 8);   \
    if(sizeof(g) > 2) (b) = (b) ^ ((unsigned)(b) >> 16);   \
}

#define PI              3.141592657
#define SIN(x)          sin((x) * PI / 180)
#define COS(x)          cos((x) * PI / 180)
#define TAN(x)          tan((x) * PI / 180)
#define ASIN(x)         (asin(x) / PI * 180)
#define ACOS(x)         (acos(x) / PI * 180)
#define ATAN(x)         (atan(x) / PI * 180)

/******************** TemplateFunc **************************/
template<typename T>
int CSort(int len, T *d);

template<typename T>
bool CSortMax(int len, int *idx,  T *d);

template<typename T>
bool CSortMin(int len, int *idx,  T *d);

template<typename T>
bool DiffVal(vector<T> &data, vector<double> &diff, int step);

template<typename T>
double Mean(vector<T> &data);

template<typename T>
int CheckLineDataAbnormityPoint(vector<T> &data, vector<int> &point);

template<typename T, typename T1>
bool CountCellDataSum(int cellWid, int wid, int hgt, T *data, T1 *sumData);

template<typename T, typename T1>
bool GetProbabilityDistributionData(int cellWid, int wid, int hgt, T *in, int maxVal, T1 *out, double *ratio=NULL);

template<typename T>
T GCD(T a, T b);

template<typename T>
T LCM(T a, int b);


/**********************************************/
// 统计值为1的位元数;
int PopCount(unsigned int x);
// Hamming Distance;
#define HammDist(x, y)      PopCount(x ^ y)
// 计算两个字组种群计数之差，pop(x) - pop(y);
int PopDiff(unsigned int x, unsigned int y);
// 比较pop(x)与pop(y)的大小;
int PopCmpr(unsigned int xp, unsigned int yp);
// 求数组中的种群计数，每次处理两个字组元素;
int PopArray(unsigned int A[], int n);
// 判断字组中值为‘1’的位元个数是奇数还是偶数;
int ParityCheck(unsigned int x);
// 计算x前导0的个数;
unsigned int Nlz(unsigned int x);
// 计算x尾部0的个数;
unsigned int Ntz(unsigned int x);
// 寻找最左侧0值字节;
int ZByteL(unsigned int x);
// 寻找最右侧0值字节(小端);
int ZByteR(unsigned int x);
// 寻找首个给定长度的全1位串;
int Ffstr1(unsigned int x, int n);
// 寻找最长“全1位串”的长度及起始位置;
int Fmaxstr1(unsigned int x, int *apos);
//寻找最短“全1位串”的长度与起始位置;
int Fminstr1(unsigned int x, int *apos);
// 计算比x值大且位元1的个数相同的数;
int Snoob(int x);
// 将数值上调/下调为2的已知次幂的倍数;
unsigned int Flp2(unsigned int x);
unsigned int Clp2(unsigned int x);
// 外完美洗牌;
// 例：abcd efgh ijkl mnop ABCD EFGH IJKL MNOP
//    aAbB cCdD eEfF gGhH iIjJ kKlL mMnN oOpP
unsigned OuterPerfectShuffle(unsigned int x);
// 外完美理牌;
unsigned OuterPerfectUnshuffle(unsigned int x);
// 内完美洗牌;
// 例：abcd efgh ijkl mnop ABCD EFGH IJKL MNOP
//    AaBb CcDd EeFf GgHh IiJj KkLl MmNn OoPp
unsigned InnerPerfectShuffle(unsigned int x);
// 内完美理牌;
unsigned InnerPerfectUnshuffle(unsigned int x);
// 压缩与展开;
unsigned int Compress(unsigned int x, unsigned int m);
unsigned int Expand(unsigned int x, unsigned int m);
unsigned int CompressLeft(unsigned int x, unsigned int m);
unsigned int ExpandLeft(unsigned int x, unsigned int m);
// 分羊法(sheep and goats operation, SAG);
#define SAG(x, m)       (CompressLeft(x, m) | Compress(x, ~m))

unsigned int CRC32(unsigned char *message);
unsigned int CheckBits(unsigned int u);
int Correct(unsigned int pr, unsigned int *ur);

// 反转位元;
unsigned int RevBit(unsigned int x);
unsigned int KnuthRevBit(unsigned int x);
unsigned long long KnuthRevBit(unsigned long long x);
unsigned int RevMask(unsigned int x);

void TransPose8(unsigned char a[8], int m, int n, unsigned char b[8]);
void TransPose32(unsigned int a[32]);
void TransPose32(unsigned int a[32], int seek);
void TransPose32(unsigned int a[32], unsigned int b[32]);
void TransPoseMatrix8(unsigned int *src, int w, int h, unsigned int *dst);
void TransPoseChar(unsigned char *dst, unsigned char *src, int w, int h);
void TransPoseMatrix(unsigned int *src, int w, int h, unsigned int *dst);
void TransPoseInternalMatrix(unsigned int *a, int w, int h, unsigned int *t, int aOffset, int tOffset);
int TransPoseBigMatrix(unsigned int *a, int w, int h, unsigned int *t, int maxMatrixSize);

#include "ctemplatefunc.h"

#endif // CMATH_H
