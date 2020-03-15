#ifndef CMATH_H
#define CMATH_H

#include <vector>
#include <math.h>

using std::vector;

/******************** MacroFunc **************************/
// �ر���ҵ�λԪ��0101 1110=>0101 1100�������ж��޷��������ǲ���2���ݻ�0��������Ƿ�Ϊ0;
#define TurnOffLastRightBit(x)          ((x) & ((x)-1))
// ����ҵ�λԪ��0101 0111=>0101 1111��;
#define TurnOnLastRightBit(x)           ((x) | ((x)+1))
// �ر�����β����1��0101 0111=>0101 0000�������ж��޷��������ǲ���2^n-1��0��Ҳ���ж�ĳ����������λԪ�Ƿ��Ϊ1��������Ƿ�Ϊ0;
#define TurnOffTailBit(x)               ((x) & ((x)+1))
// ������β����0��0101 1000=>0101 1111��;
#define TurnOnTailBit(x)                ((x) | ((x)-1))
// ת��x�����ֵΪ0��λ�ڱ�Ϊ1������λԪ��0��1010 0111=>0000 1000������x��û��0��λԪ������Ϊ0;
#define TrunLastRightBit0(x)            (~(x) & ((x)+1))
// ת��x�����Ϊ1��λԪ��Ϊ0������λԪ��1��1010 1000=>1111 0111������x��û��1��λԪ��������ÿ��λԪ����1;
#define TrunLastRightBit1(x)            (~(x) | ((x)-1))
// ת��xβ������Ϊ0��λԪ��Ϊ1������λԪ��0��1010 0111=>1111 1000������xû��0��λԪ������Ϊ0;
#define TrunTailBit0(x)                 (~(x) & ((x)-1))
// ת��xβ������Ϊ1��λԪ��Ϊ0������λԪ��1��1010 0111=>1111 1000������xû��1��λԪ��������ÿ��λԪ����1;
#define TrunTailBit1(x)                 (~(x) | ((x)+1))
// ��ȡx�����Ϊ1��λԪ������λԪ��0��0101 1000=>0000 1000������xû��1��λԪ������Ϊ0;
#define GetLastRightBit1(x)             ((x) & (~(x)+1))
// ��ȡx�����Ϊ0��λԪ������λԪ��1��0101 0111=>1111 0111������xû��0��λԪ������Ϊ1;
#define GetLastRightBit0(x)             ((x) | (~(x)-1))
// ��ȡx�����Ϊ1��λԪ�����Ҳ����1����������0��0101 1000=>0000 1111������xû��1��λԪ��������ÿ��λԪ����1;
#define GetTailBit1(x)                  ((x) ^ ((x)-1))
// ��ȡx�����Ϊ0��λԪ�����Ҳ����1����������0��0101 0111=>0000 1111������xû��0��λԪ��������ÿ��λԪ����1;
#define GetTailBit0(x)                  ((x) ^ ((x)+1))
// ����x�Ҳ�����������Ϊ1��λԪ��0��0101 1100=>0100 0000��; /* ((((x) & -(x)) + (x)) & (x)) */
#define TurnOffRightContinueBit(x)      ((((x) | ((x)-1)) + 1) & (x))
// ������ͬ�Ĵ�������Ӧ��λ��;
#define SwapBitField(x, y, mask)        {(x) = (x) ^ (y); (y) = (y) ^ ((x) & (mask)); (x) = (x) ^ (y);}

//����ֵ����;
#define ABS(x)                          (((x) ^ ((signed)(x) >> 31)) - ((signed)(x) >> 31))
#define NABS(x)                         (((signed)(x) >> 31) - ((x) ^ ((signed)(x) >> 31)))
// ƽ��ֵ����;
#define AVEF(x, y)                      (((x) & (y)) + ((unsigned)((x) ^ (y)) >> 1))
#define AVEC(x, y)                      (((x) | (y)) - (((x) ^ (y)) >> 1))
// ���ź���;
#define SIGN(x)                         (((x) >> 31) | ((unsigned)(-(x)) >> 31))
// �ȽϺ���;
#define CMP(x, y)                       (((x) > (y)) - ((x) < (y)))

// ������ת������;
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
// ͳ��ֵΪ1��λԪ��;
int PopCount(unsigned int x);
// Hamming Distance;
#define HammDist(x, y)      PopCount(x ^ y)
// ��������������Ⱥ����֮�pop(x) - pop(y);
int PopDiff(unsigned int x, unsigned int y);
// �Ƚ�pop(x)��pop(y)�Ĵ�С;
int PopCmpr(unsigned int xp, unsigned int yp);
// �������е���Ⱥ������ÿ�δ�����������Ԫ��;
int PopArray(unsigned int A[], int n);
// �ж�������ֵΪ��1����λԪ��������������ż��;
int ParityCheck(unsigned int x);
// ����xǰ��0�ĸ���;
unsigned int Nlz(unsigned int x);
// ����xβ��0�ĸ���;
unsigned int Ntz(unsigned int x);
// Ѱ�������0ֵ�ֽ�;
int ZByteL(unsigned int x);
// Ѱ�����Ҳ�0ֵ�ֽ�(С��);
int ZByteR(unsigned int x);
// Ѱ���׸��������ȵ�ȫ1λ��;
int Ffstr1(unsigned int x, int n);
// Ѱ�����ȫ1λ�����ĳ��ȼ���ʼλ��;
int Fmaxstr1(unsigned int x, int *apos);
//Ѱ����̡�ȫ1λ�����ĳ�������ʼλ��;
int Fminstr1(unsigned int x, int *apos);
// �����xֵ����λԪ1�ĸ�����ͬ����;
int Snoob(int x);
// ����ֵ�ϵ�/�µ�Ϊ2����֪���ݵı���;
unsigned int Flp2(unsigned int x);
unsigned int Clp2(unsigned int x);
// ������ϴ��;
// ����abcd efgh ijkl mnop ABCD EFGH IJKL MNOP
//    aAbB cCdD eEfF gGhH iIjJ kKlL mMnN oOpP
unsigned OuterPerfectShuffle(unsigned int x);
// ����������;
unsigned OuterPerfectUnshuffle(unsigned int x);
// ������ϴ��;
// ����abcd efgh ijkl mnop ABCD EFGH IJKL MNOP
//    AaBb CcDd EeFf GgHh IiJj KkLl MmNn OoPp
unsigned InnerPerfectShuffle(unsigned int x);
// ����������;
unsigned InnerPerfectUnshuffle(unsigned int x);
// ѹ����չ��;
unsigned int Compress(unsigned int x, unsigned int m);
unsigned int Expand(unsigned int x, unsigned int m);
unsigned int CompressLeft(unsigned int x, unsigned int m);
unsigned int ExpandLeft(unsigned int x, unsigned int m);
// ����(sheep and goats operation, SAG);
#define SAG(x, m)       (CompressLeft(x, m) | Compress(x, ~m))

unsigned int CRC32(unsigned char *message);
unsigned int CheckBits(unsigned int u);
int Correct(unsigned int pr, unsigned int *ur);

// ��תλԪ;
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
