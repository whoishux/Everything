#include <math.h>
#include <memory.h>
#include <stdlib.h>


#include "ccommon.h"
#include "cmath.h"

using namespace std;

/*
 *  ~(x & y) = ~x | ~y;
 *  ~(x | y) = ~x & ~y;
 *  ~(x + 1) = ~x - 1;
 *  ~(x - 1) = ~x + 1;
 *  ~(x ^ y) = ~x ^ y;
 *  ~(x + y) = ~x - y;
 *  ~(x - y) = ~x + y;
 *
    a.  -x = ~x + 1;
    b.  -x = ~(x - 1);
    c.  ~x = -x - 1;
    d.  -~x = x + 1;
    e.  ~-x = x - 1;
    f.  x + y = x - ~y - 1;
    g.  x + y = (x ^ y) + 2*(x & y);
    h.  x + y = (x | y) + (x & y);
    i.  x + y = 2*(x | y) - (x ^ y);
    j.  x - y = x + ~y + 1;
    k.  x - y = (x ^ y) - 2*(~x & y);
    l.  x - y = (x & ~y) - (~x & y);
    m.  x - y = 2*(x & -y) - (x ^ y);
    n.  x ^ y = (x | y) - (x & y);
    o.  x & ~y = (x | y) - y;
    p.  x & ~y = x - (x & y);
    q.  ~(x - y) = y - x - 1;
    r.  ~(x - y) = ~x + y;
    s.  ~(x ^ y) = (x & y) - (x | y) - 1;
    t.  ~(x ^ y) = (x & y) + ~(x | y);
    u.  x | y = (x & ~y) + y;
    v.  x & y = (~x | y) - ~x;

*/


// 计算bit1的num;
int PopCount(unsigned int x)
{
#if 0
    int count = 0;

    for (; x; count ++) {
        x &= x - 1;
    }

    return count;
#endif
#if 0
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
    x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
    x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);

    return x;
#endif
#if 0
    static const int ones[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2,\
     3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4,\
     3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2,\
     3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5,\
     5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5,\
     6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,\
     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3,\
     4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3,\
     3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5,\
     6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6,\
     5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

    return (ones[x & 0xff] + ones[(x >> 8) & 0xff] + ones[(x >> 16) & 0xff] + ones[(x >> 24) & 0xff]);
#endif
#if 0
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003f;
#endif
#if 0
    unsigned int n;
    n = (x >> 1) & 033333333333;    // Count bits in;
    x = x - n;                      // each 3-bit;
    n = (n >> 1) & 033333333333;    // field;
    x = x - n;
    x = (x + (x >> 3)) & 030707070707;  // 6-bit sums;
    return x % 63;                  // Add 6-bit sums;
#endif
#if 1
    unsigned int n;
    n = (x >> 1) & 0x77777777;      // Count bits in;
    x = x - n;                      // each 4-bit;
    n = (n >> 1) & 0x77777777;      // field;
    x = x - n;
    n = (n >> 1) & 0x77777777;
    x = x - n;
    x = (x + (x >> 4)) & 0x0f0f0f0f;    // Get byte sums;
    x = x*0x01010101;                   // Add the bytes;
    return x >> 24;
#endif
}

// 计算两个字组种群计数之差，pop(x) - pop(y);
int PopDiff(unsigned int x, unsigned int y)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    y = ~y;
    y = y - ((y >> 1) & 0x55555555);
    y = (y & 0x33333333) + ((y >> 2) & 0x33333333);
    x = x + y;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return (x & 0x0000007f) - 32;
}

// 比较pop(x)与pop(y)的大小;
int PopCmpr(unsigned int xp, unsigned int yp)
{
    unsigned int x, y;
    x = xp & ~yp;           // Clear bits where;
    y = yp & ~xp;
    while (1) {
        if(x == 0) return y | -(int)y;
        if(y == 0) return 1;
        x = x & (x - 1);    // Clear one bit;
        y = y & (y - 1);
    }
}

// 进位保留加法器(carry-save adder, CSA);
#define CSA(h, l, a, b, c)  \
{   \
    unsigned int u = a ^ b; \
    unsigned int v = c;     \
    h = (a & b) | (u & v);  \
    l = u ^ v;              \
}

// 求数组中的种群计数，每次处理两个字组元素;
int PopArray(unsigned int A[], int n)
{
#if 0
    int tot, i;
    unsigned int ones, twos;

    tot = 0;
    ones = 0;
    for(i=0; i<=n-2; i+=2){
        CSA(twos, ones, ones, A[i], A[i+1]);
        tot = tot + PopCount(twos);
    }
    tot = 2 * tot + PopCount(ones);

    // If there's a last one, add it in;
    if(n & 1) tot = tot + PopCount(A[i]);

    return tot;
#else
    int tot, i;
    unsigned int ones, twos, twosA, twosB,
            fours, foursA, foursB, eights;

    tot = 0;
    fours = twos = ones = 0;

    for(i=0; i<=n-8; i+=8){
        CSA(twosA, ones, ones, A[i], A[i+1]);
        CSA(twosB, ones, ones, A[i+2], A[i+3]);
        CSA(foursA, twos, twos, twosA, twosB);
        CSA(twosA, ones, ones, A[i+4], A[i+5]);
        CSA(twosB, ones, ones, A[i+6], A[i+7]);
        CSA(foursB, twos, twos, twosA, twosB);
        CSA(eights, fours, fours, foursA, foursB);
        tot += PopCount(eights);
    }
    tot = 8*tot + 4*PopCount(fours) + 2*PopCount(twos) + PopCount(ones);

    // Simly ad in the last 0 to 7 elements;
    for(i=i; i<n; i++) tot += PopCount(A[i]);

    return tot;
#endif
}

// 判断字组中值为‘1’的位元个数是奇数还是偶数;
int ParityCheck(unsigned int x)
{
#if 1
    unsigned int y;
    y = x ^ (x >> 1);
    y = y ^ (y >> 2);
    y = y ^ (y >> 4);
    y = y ^ (y >> 8);
    y = y ^ (y >> 16);
    return y;
#else
    x = x ^ (x >> 1);
    x = (x ^ (x >> 2)) & 0x11111111;
    x = x * 0x11111111;
    return (x >> 28) & 1;
#endif
}

// 计算x前导0的个数(大端);
unsigned int Nlz(unsigned int x)
{
 #if 0
    int n;

    if(x == 0) return 32;
    n = 1;
    if((x >> 16) == 0){n += 16; x <<= 16;}
    if((x >> 24) == 0){n += 8; x <<= 8;}
    if((x >> 28) == 0){n += 4; x <<= 4;}
    if((x >> 30) == 0){n += 2; x <<= 2;}
    n -= (x >> 31);
    return n;
#endif
#if 0
    unsigned int y;
    int n = 32;

    y = x >> 16; if(y != 0){n -= 16; x = y;}
    y = x >> 8; if(y != 0){n -= 8; x = y;}
    y = x >> 4; if(y != 0){n -= 4; x = y;}
    y = x >> 2; if(y != 0){n -= 2; x = y;}
    y = x >> 1; if(y != 0) return n - 2;

    return n - x;
#endif
#if 0
    static char table[256] = {
        0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    };
    return 32 - table[x & 0xff] - table[(x >> 8) & 0xff] - table[(x >> 16) & 0xff] - table[(x >> 24) & 0xff];
#endif
#if 0
    int y, n;
    n = 0;
    y = x;
    while(1){
        if(x < 0) return n;
        if(y == 0) return 32 - n;
        n++;
        x <<= 1;
        y >>= 1;
    }
#endif
#if 0
    int y, m, n;
    y = -(x >> 16);         // If left half of x is 0,
    m = (y >> 16) & 16;     // set n = 16. If left half
    n = 16 - m;             // is nonzero, set n = 0 and
    x = x >> m;             // shift x right 16.
                            // Now x is of the from 0000xxxx.
    y = x - 0x100;          // If positions 8-15 are 0,
    m = (y >> 16) & 8;      // add 8 to n and shift x left 8.
    n = n + m;
    x = x << m;

    y = x - 0x1000;         // If positions 12-15 are 0,
    m = (y >> 16) & 4;      // add 4 to n and shift x left 4.
    n = n + m;
    x = x << m;

    y = x - 0x4000;         // If positions 14-15 are 0,
    m = (y >> 16) & 2;      // add 2 to n and shift x left 2.
    n = n + m;
    x = x << m;

    y = x >> 14;            // Set y = 0, 1, 2, or 3.
    m = y & ~(y >> 1);      // Set m = 0, 1, 2, or w resp.
    return n + 2 - m;
#endif
#if 1
#define u   ' '
    static char table[64] = {
        32, 31, u, 16, u, 30, 3, u,     15, u, u, u, 29, 10, 2, u,
        u, u, 12, 14, 21, u, 19, u,     u, 28, u, 25, u, 9, 1, u,
        17, u, 4, u, u, u, 11, u,       13, 22, 20, u, 26, u, u, 18,
        5, u, u, 23, u, 27, u, 6,       u, 24, 7, u, 8, u, 0, u
    };
#undef  u
    x = x | (x >> 1);       // Propagate leftmost
    x = x | (x >> 2);       // 1-bit to the right.
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    x = x * 0x06eb14f9;     // Multiplier is 7*255**3.
    return table[x >> 26];
#endif
}

// 计算x尾部0的个数(小端);
unsigned int Ntz(unsigned int x)
{
#if 0
   int n;

   if(x == 0) return 32;
   n = 1;
   if((x & 0x0000ffff) == 0){n += 16; x >>= 16;}
   if((x & 0x000000ff) == 0){n += 8; x >>= 8;}
   if((x & 0x0000000f) == 0){n += 4; x >>= 4;}
   if((x & 0x00000003) == 0){n += 2; x >>= 2;}
   return n - (x & 1);
#endif
#if 0
   unsigned y, bz, b4, b3, b2, b1, b0;

   y = x & -x;              // Isolate rightmost 1-bit;
   bz = y ? 0 : 1;          // 1 if y = 0;
   b4 = (y & 0x0000ffff) ? 0 : 16;
   b3 = (y & 0x00ff00ff) ? 0 : 8;
   b2 = (y & 0x0f0f0f0f) ? 0 : 4;
   b1 = (y & 0x33333333) ? 0 : 2;
   b0 = (y & 0x55555555) ? 0 : 1;
   return bz + b4 + b3 + b2 + b1 + b0;
#endif
#if 0
#define u   ' '
    static char table[64] = {
        32, 0, 1, 12, 2, 6, u, 13,     3, u, 7, u, u, u, u, 14,
        10, 4, u, u, 8, u, u, 25,     u, u, u, u, u, 25, 27, 15,
        31, 11, 5, u, u, u, u, u,       9, u, u, 24, u, u, 20, 26,
        30, u, u, u, u, 23, u, 19,       29, u, 22, 18, 28, 17, 16, u
    };
#undef  u
//    x = (x & -x) * 0x0450fbaf;  // 0x0450fbaf=17*65*655;
    x = (x & -x);
    x = (x << 4) + x;       // x = x*17;
    x = (x << 6) + x;       // x = x*65;
    x = (x << 16) - x;      // x = x*65535;
    return table[x >> 26];
#endif
#if 1
    static char table[32] = {
        0, 1, 2, 24, 3, 19, 6, 25,      22, 4, 20, 10, 16, 7, 12, 26,
        31, 23, 18, 5, 21, 9, 15, 11,   30, 17, 8, 14, 29, 13, 28, 27
    };
    if(x == 0) return 32;
    x = (x & -(int)x) * 0x04d7651f;      // 0x04d7651f=2047*5*256+1;
    return table[x >> 27];
#endif
}

void Ld_Gosper(int(*f)(int), int x0, int *mu_l, int *mu_u, int *lambda)
{
    int xn, k, m, kmax, n, lgl;
    int T[33];

    T[0] = x0;
    xn = x0;
    for(n=1; ; n++){
        xn = f(xn);
        kmax = 31 - Nlz(n);         // Floor(log2n);
        for(k=0; k<=kmax; k++){
            if(xn == T[k]) goto L;
        }
        T[Ntz(n+1)] = xn;           // No match;
    }
L:
    // Compute m = max{i | i < n and ntz(i+1) = k};
    m = ((((n >>k) - 1) | 1) << k) - 1;
    *lambda = n - m;
    lgl = 31 - Nlz(*lambda - 1);    // Ceil(log2 lambda) - 1;
    *mu_u = m;                      // Upper bound on mu;
    *mu_l = m - Max(1, 1 << lgl) + 1;// Lower bound on mu;
}

// 寻找最左侧0值字节(大端);
int ZByteL(unsigned int x)
{
#if 1
    if((x >> 24) == 0) return 0;
    else if((x & 0x00ff0000) == 0) return 1;
    else if((x & 0x0000ff00) == 0) return 2;
    else if((x & 0x000000ff) == 0) return 3;
    else return 4;
#endif
#if 0
    unsigned int y;
    int n;

    // Original byte: 00 80 other;
    y = (x & 0x7f7f7f) + 0x7f7f7f7f;
    y = ~(y | x | 0x7f7f7f7f);
    n = Ntz(y) >> 3;
    return n;
#endif
#if 0
    unsigned int y;
    int t1, t2, t3, t4;

    y = (x & 0x7f7f7f7f) + 0x7f7f7f7f;
    y = y | x;              // Leading 1 on nonzero bytes;
    t1 = y >> 31;           // t1 = a;
    t2 = (y >> 23) & t1;    // t2 = ab;
    t3 = (y >> 15) & t2;    // t3 = abc;
    t4 = (y >> 7) & t3;     // t4 = abcd;
    return t1 + t2 + t3 + t4;
#endif
}

// 寻找最右侧0值字节(小端);
int ZByteR(unsigned int x)
{
#if 0
    if((x >> 24) == 0) return 3;
    else if((x & 0x00ff0000) == 0) return 2;
    else if((x & 0x0000ff00) == 0) return 1;
    else if((x & 0x000000ff) == 0) return 0;
    else return 4;
#endif
#if 1
    unsigned int y;
    int t1, t2, t3, t4;

    y = (x & 0x7f7f7f7f) + 0x7f7f7f7f;
    y = y | x;              // Leading 1 on nonzero bytes;
//    PrintBitNum(y);
    t1 = (y >> 7) & 0x01;   // t1 = d;
//    PrintBitNum(t1);
    t2 = (y >> 15) & t1;    // t2 = cd;
//    PrintBitNum(t2);
    t3 = (y >> 23) & t2;    // t3 = bcd;
//    PrintBitNum(t3);
    t4 = (y >> 31) & t3;    // t4 = abcd;
//    PrintBitNum(t4);
    return t1 + t2 + t3 + t4;
#endif
}

// 寻找首个给定长度的全1位串(大端);
int Ffstr1(unsigned int x, int n)
{
#if 0
    int k, p;

    p = 0;
    while(x != 0){
        k = Nlz(x);         // Skip over initial 0's;
        x = x << k;         // (if any);
        p = p + k;
        k = Nlz(~x);        // Count first/next group of 1's;
        if(k >= n)          // If enough,
            return p;       // return;
        x = x << k;         // Not enough 1's, skip over;
        p = p + k;          // them;
    }
    return 32;
#else
    int s;
    while(n > 1){
        s = n >> 1;
        x = x & (x << s);
        n = n - s;
    }
    return Nlz(x);
#endif
}

// 寻找全长“全1位串”的长度;
int Maxstr1(unsigned int x)
{
    int k;
    for(k=0; x != 0; k++) x = x & 2*x;
    return k;
}

// 寻找最长“全1位串”的长度及起始位置;
int Fmaxstr1(unsigned int x, int *apos)
{
    unsigned int y;
    int s;

    if(x == 0){*apos = 32; return 0;}
    y = x & (x << 1);
    if(y == 0){s = 1; goto L1;}
    x = y & (y << 2);
    if(x == 0){s = 2; x = y; goto L2;}
    y = x & (x << 4);
    if(y == 0){s = 4; goto L4;}
    x = y & (y << 8);
    if(x == 0){s = 8; x = y; goto L8;}
    if(x == 0xfff8000){*apos = 0; return 32;}
    s = 16;

L16:
    y = x & (x << 8);
    if(y != 0){s = s + 8; x = y;}
L8:
    y = x & (x << 4);
    if(y != 0){s = s + 4; x = y;}
L4:
    y = x & (x << 2);
    if(y != 0){s = s + 2; x = y;}
L2:
    y = x & (x << 1);
    if(y != 0){s = s + 1; x = y;}
L1:
    *apos = Nlz(x);
    return s;
}

//寻找最短“全1位串”的长度与起始位置;
int Fminstr1(unsigned int x, int *apos)
{
    int k;
    unsigned int b, e;

    if(x == 0){*apos = 32; return 0;}
    b = ~(x >> 1) & x;      // 0-1 transitions;
    e = x & ~(x << 1);      // 0-1 transitions;
    for(k=1; (b&e)==0; k++) e = e << 1;
    *apos = Nlz(b & e);
    return k;
}

// 计算比x值大且位元1的个数相同的数;
int Snoob(int x)
{
    unsigned int smallest, ripple, ones;
                                    // x = xxx0 1111 0000;
    smallest = x & -x;              //     0000 0001 0000;
    ripple = x + smallest;          //     xxx1 0000 0000;
    ones = x ^ ripple;              //     0001 1111 0000;
    ones = (ones >> 2) / smallest;  //     0000 0000 0111;
    return ripple | ones;           //     xxx1 0000 0111;
}

unsigned int Flp2(unsigned int x)
{
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

unsigned int Clp2(unsigned int x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

// 外完美洗牌;
// 例：abcd efgh ijkl mnop ABCD EFGH IJKL MNOP
//    aAbB cCdD eEfF gGhH iIjJ kKlL mMnN oOpP
unsigned OuterPerfectShuffle(unsigned int x)
{
#if 1
    x = (x & 0x0000ff00) << 8 | (x >> 8) & 0x0000ff00 | x & 0xff0000ff;
    // 最后三步只对字组的左右两个办字组内部分别乱序排列位元;
    x = (x & 0x00f000f0) << 4 | (x >> 4) & 0x00f000f0 | x & 0xf00ff00f;
    // 最后两步只对每个字节内部分别乱序排列位元;
    x = (x & 0x0c0c0c0c) << 2 | (x >> 2) & 0x0c0c0c0c | x & 0xc3c3c3c3;
    x = (x & 0x22222222) << 1 | (x >> 1) & 0x22222222 | x & 0x99999999;
    return x;
#else
    unsigned int t;
    t = (x ^ (x >> 8)) & 0x0000ff00; x = x ^ t ^ (t << 8);
    t = (x ^ (x >> 4)) & 0x00f000f0; x = x ^ t ^ (t << 4);
    t = (x ^ (x >> 2)) & 0x0c0c0c0c; x = x ^ t ^ (t << 2);
    t = (x ^ (x >> 1)) & 0x22222222; x = x ^ t ^ (t << 1);
    return x;
#endif
}

// 外完美理牌;
unsigned OuterPerfectUnshuffle(unsigned int x)
{
    unsigned int t;
    t = (x ^ (x >> 1)) & 0x22222222; x = x ^ t ^ (t << 1);
    t = (x ^ (x >> 2)) & 0x0c0c0c0c; x = x ^ t ^ (t << 2);
    t = (x ^ (x >> 4)) & 0x00f000f0; x = x ^ t ^ (t << 4);
    t = (x ^ (x >> 8)) & 0x0000ff00; x = x ^ t ^ (t << 8);
    return x;
}

// 内完美洗牌;
// 例：abcd efgh ijkl mnop ABCD EFGH IJKL MNOP
//    AaBb CcDd EeFf GgHh IiJj KkLl MmNn OoPp
unsigned InnerPerfectShuffle(unsigned int x)
{
    x = (x >> 16) | (x << 16);
#if 1
    x = (x & 0x0000ff00) << 8 | (x >> 8) & 0x0000ff00 | x & 0xff0000ff;
    // 最后三步只对字组的左右两个办字组内部分别乱序排列位元;
    x = (x & 0x00f000f0) << 4 | (x >> 4) & 0x00f000f0 | x & 0xf00ff00f;
    // 最后两步只对每个字节内部分别乱序排列位元;
    x = (x & 0x0c0c0c0c) << 2 | (x >> 2) & 0x0c0c0c0c | x & 0xc3c3c3c3;
    x = (x & 0x22222222) << 1 | (x >> 1) & 0x22222222 | x & 0x99999999;
    return x;
#else
    unsigned int t;
    t = (x ^ (x >> 8)) & 0x0000ff00; x = x ^ t ^ (t << 8);
    t = (x ^ (x >> 4)) & 0x00f000f0; x = x ^ t ^ (t << 4);
    t = (x ^ (x >> 2)) & 0x0c0c0c0c; x = x ^ t ^ (t << 2);
    t = (x ^ (x >> 1)) & 0x22222222; x = x ^ t ^ (t << 1);
    return x;
#endif
}

// 内完美理牌;
unsigned InnerPerfectUnshuffle(unsigned int x)
{
    unsigned int t;
    t = (x ^ (x >> 1)) & 0x22222222; x = x ^ t ^ (t << 1);
    t = (x ^ (x >> 2)) & 0x0c0c0c0c; x = x ^ t ^ (t << 2);
    t = (x ^ (x >> 4)) & 0x00f000f0; x = x ^ t ^ (t << 4);
    t = (x ^ (x >> 8)) & 0x0000ff00; x = x ^ t ^ (t << 8);
    x = (x >> 16) | (x << 16);
    return x;
}

unsigned int Compress(unsigned int x, unsigned int m)
{
#if 0
    unsigned int r, s, b;       // Result, shift, mask bit;

    r = 0;
    s = 0;
    do{
        b = m & 1;
        r = r | ((x & b) << s);
        s = s + b;
        x = x >> 1;
        m = m >> 1;
    }while(m != 0);
    return r;
#else
    // 平行后缀法;
    unsigned int mk, mp, mv, t;
    int i;

    x = x & m;              // Clear irrelevant bits;
    mk = ~m << 1;           // We will count 0's to right;

    for(i=0; i<5; i++){
        mp = mk ^ (mk << 1);            // Parallel suffix;
        mp = mp ^ (mp << 2);
        mp = mp ^ (mp << 4);
        mp = mp ^ (mp << 8);
        mp = mp ^ (mp << 16);
        mv = mp & m;                    // Bits to move;
        m = m ^ mv | (mv >> (1 << i));  // Compress m;
        t = x & mv;
        x = x ^ t | (t >> (1 << i));    // Compress x;
        mk = mk & ~mp;
    }
    return x;
#endif
}

unsigned int CompressLeft(unsigned int x, unsigned int m)
{
    // 平行后缀法;
    unsigned int mk, mp, mv, t;
    int i;

    x = x & m;              // Clear irrelevant bits;
    mk = ~m >> 1;           // We will count 0's to right;

    for(i=0; i<5; i++){
        mp = mk ^ (mk >> 1);            // Parallel suffix;
        mp = mp ^ (mp >> 2);
        mp = mp ^ (mp >> 4);
        mp = mp ^ (mp >> 8);
        mp = mp ^ (mp >> 16);
        mv = mp & m;                    // Bits to move;
        m = m ^ mv | (mv << (1 << i));  // Compress m;
        t = x & mv;
        x = x ^ t | (t << (1 << i));    // Compress x;
        mk = mk & ~mp;
    }
    return x;
}

unsigned int Expand(unsigned int x, unsigned int m)
{
    unsigned int m0, mk, mp, mv, t;
    unsigned array[5];
    int i;

    m0 = m;         // Save original mask;
    mk = ~m << 1;   // We will count 0's to right;

    for(i=0; i<5; i++){
        mp = mk ^ (mk << 1);        // Parallel suffix;
        mp = mp ^ (mp << 2);
        mp = mp ^ (mp << 4);
        mp = mp ^ (mp << 8);
        mp = mp ^ (mp << 16);
        mv = mp & m;                // Bits to move;
        array[i] = mv;
        m = (m ^ mv) | (mv >> (1 << i));    // Compress m;
        mk = mk & ~mp;
    }

    for(i=4; i>=0; i--){
        mv = array[i];
        t = x << (1 << i);
        x = (x & ~mv) | (t & mv);
    }
    return x & m0;      // Crear out extraneous bits;
}

unsigned int ExpandLeft(unsigned int x, unsigned int m)
{
    unsigned int m0, mk, mp, mv, t;
    unsigned array[5];
    int i;

    m0 = m;         // Save original mask;
    mk = ~m >> 1;   // We will count 0's to right;

    for(i=0; i<5; i++){
        mp = mk ^ (mk >> 1);        // Parallel suffix;
        mp = mp ^ (mp >> 2);
        mp = mp ^ (mp >> 4);
        mp = mp ^ (mp >> 8);
        mp = mp ^ (mp >> 16);
        mv = mp & m;                // Bits to move;
        array[i] = mv;
        m = (m ^ mv) | (mv << (1 << i));    // Compress m;
        mk = mk & ~mp;
    }

    for(i=4; i>=0; i--){
        mv = array[i];
        t = x >> (1 << i);
        x = (x & ~mv) | (t & mv);
    }
    return x & m0;      // Crear out extraneous bits;
}

// 带符号的多字整数乘法;
void Mulmns(unsigned short w[], unsigned short u[], unsigned short v[], int m, int n)
{
    unsigned int k, t, b;
    int i, j;

    for(i=0; i<m; i++) w[i] = 0;

    for(j=0; j<n; j++){
        k = 0;
        for(i=0; i<m; i++){
            t = u[i] * v[j] + w[i+j] + k;
            w[i+j] = t;
            k = t >> 16;
        }
        w[j+m] = k;
    }

    if((short)u[m-1] < 0){
        b = 0;
        for(j=0; j<n; j++){
            t = w[j+m] - v[j] - b;
            w[j+m] - t;
            b = t >> 31;
        }
    }
    if((short)v[n-1] < 0){
        b = 0;
        for(i=0; i<m; i++){
            t = w[i+n] - u[i] - b;
            w[i+n] = t;
            b = t >> 31;
        }
    }

    return;
}

// 两个带符号数之积的高权重部分;
int MultiplyHighSigned(int u, int v)
{
    unsigned int u0, v0, w0;
    int u1, v1, w1, w2, t;

    u0 = u & 0xffff; u1 = u >> 16;
    v0 = v & 0xffff; v1 = v >> 16;
    w0 = u0 * v0;
    t = u1 * v0 + (w0 >> 16);
    w1 = t & 0xffff;
    w2 = t >> 16;
    w1 = u0 * v1 + w1;
    return u1 * v1 + w2 + (w1 >> 16);
}

unsigned int CRC32(unsigned char *message)
{
#if 1
    // 基本CRC-32算法;
    int i, j;
    unsigned int byte, crc;

    i = 0;
    crc = 0xffffffff;
    while(message[i] != 0){
        byte = message[i];          // Get next byte;
        byte = RevBit(byte);        // 32-bit reversal;
        for(j=0; j<=7; j++){
            if((int)(crc ^ byte) < 0)
                crc = (crc << 1) ^ 0x04c11db7;
            else
                crc = crc << 1;
            byte = byte << 1;       // Ready next msg bit;
        }
        i = i + 1;
    }
    return RevBit(~crc);
#endif
#if 0
    // 每次处理一个位元;
    int i, j;
    unsigned int byte, crc, mask;

    i = 0;
    crc = 0xffffffff;
    while(message[i] != 0){
        byte = message[i];
        crc = crc ^ byte;
        for(j=7; j>=0; j--){
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xedb88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
#endif
#if 0
    // 查表法实现CRC算法;
    int i, j;
    unsigned int byte, crc, mask;
    static unsigned int table[256];

    // Set up the table, if necessay;
    if(table[1] == 0){
        for(byte=0; byte<=255; byte++){
            crc = byte;
            for(j=7; j>=0; j--){
                mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xedb88320 & mask);
            }
            table[byte] = crc;
        }
    }

    // Through with table setup, now calculate the CRC;
    i = 0;
    crc = 0xffffffff;
    while((byte = message[i]) != 0){
        crc = (crc >> 8) ^ table[(crc ^ byte) & 0xff];
        i = i + 1;
    }
    return ~crc;
#endif
}

// 计算校验位;
unsigned int CheckBits(unsigned int u)
{
    /* Computes the six parity check bits fr the "infomation"
       bits are p[5:0]. On sending, an overall parity bit will
       be prepended to p (by another process).

       Bit  Checks these bits of u
       p[0] 0, 1, 3, 5, ..., 31 (0 and the odd positions).
       p[1] 0, 2-3, 6-7, ..., 30-31 (0 and positions xxx1x).
       p[2] 0, 4-7, 12-15, 20-23, 28-31 (0 and positions xx1xx).
       p[3] 0, 8-15, 24-31 (0 and positions x1xxx).
       p[4] 0, 16-31 (0 and positions 1xxxx).
       p[5] 1-31
    */

    unsigned int p0, p1, p2, p3, p4, p5, p6, p;
    unsigned int t1, t2, t3;

    // First calculate p[5:0] ignoring u[0]
    p0 = u ^ (u >> 2);
    p0 = p0 ^ (p0 >> 4);
    p0 = p0 ^ (p0 >> 8);
    p0 = p0 ^ (p0 >> 16);       // p0 is in posn 1.

    t1 = u ^ (u >> 1);
    p1 = t1 ^(t1 >> 4);
    p1 = p1 ^(p1 >> 8);
    p1 = p1 ^(p1 >> 16);        // p1 is in posn 2.

    t2 = t1 ^ (t1 >> 2);
    p2 = t2 ^ (t2 >> 8);
    p2 = p2 ^ (p2 >> 16);       // p2 is in posn 4.

    t3 = t2 ^ (t2 >> 4);
    p3 = t3 ^ (t3 >> 16);       // p3 is in posn 8.

    p4 = t3 ^ (t3 >> 8);        // p4 is in posn 16.

    p5 = p4 ^ (p4 >> 16);       // p5 is in posn 0.

    p = ((p0 >> 1) & 1) | ((p1 >> 1) & 2) | ((p2 >> 2) & 4) |
            ((p3 >> 5) & 8) | ((p4 >> 12) & 16) | ((p5 & 1) << 15);

    p = p ^ (-(int)(u & 1) & 0x3f);  // Now account for u[0].
    return p;
}

// 接收者执行的校验操作;
int Correct(unsigned int pr, unsigned int *ur)
{
    /* This function looks at the received seven check
       bits and 32 information bits (pr and ur) and
       determines how many errors occurred (under the
       presumption that it must be 0, 1, or 2). It returns
       with 0, 1, or 2, meaning that no errors, one error, or
       two errors occurred. It corrects the information word
       received (ur) if there was the one error in it.
     */

    unsigned int po, p, syn, b;

    po = ParityCheck(pr ^ *ur);         // Compute overall parity of the received data.
    p = CheckBits(*ur);                 // Calculate check bits for the received info.
    syn = p ^ (pr & 0x3f);              // Syndrome (exclusive of overall parity bit).

    if(po == 0){
        if(syn == 0) return 0;          // If no errors, return 0.
        else return 2;                  // Two errors, return 2.
    }

    // One error occureed. If syn has zero or one bits set, then the error is in the
    // check bits or the overall parity bit (no correction required).
    if(((syn - 1) & syn) == 0) return 1;

    // One error, and syn bits 5:0 tell where it is in ur.

    b = syn - 31 - (syn >> 5);          // Map syn to range 0 to 31.
    // if(syn == 0x1f) b = 0;           // (these two lines equiv.
    // else b = syn & 0x1f;             // to the one line above.)
    *ur = *ur ^ (1 << b);               // Correct the bit.

    return 1;
}

/* 获取矩阵相对位移;
*/
#define MatrixIdx2Dim(x, y, w, b)           (((x) * (b)) * (w) + (y))
#define MatrixIdx3Dim(x, y, z, w, b)        (((x) * (b) + (z)) * (w) + (y))
#define MatrixIdx2Dim8(x, y, w)             MatrixIdx2Dim(x, y, w, 8)
#define MatrixIdx3Dim8(x, y, z, w)          MatrixIdx3Dim(x, y, z, w, 8)
#define MatrixIdx2Dim32(x, y, w)            MatrixIdx2Dim(x, y, w, 32)
#define MatrixIdx3Dim32(x, y, z, w)         MatrixIdx3Dim(x, y, z, w, 32)

#define GetSeekArray(a, src, offset)	\
{	\
    (a)[0] = (src)[0*offset];			\
    (a)[1] = (src)[1*offset];			\
    (a)[2] = (src)[2*offset];			\
    (a)[3] = (src)[3*offset];			\
    (a)[4] = (src)[4*offset];			\
    (a)[5] = (src)[5*offset];			\
    (a)[6] = (src)[6*offset];			\
    (a)[7] = (src)[7*offset];			\
}

#define SetSeekArray(a, dst, offset)	\
{	\
    (dst)[0 * offset] = (a)[0];			\
    (dst)[1 * offset] = (a)[1];			\
    (dst)[2 * offset] = (a)[2];			\
    (dst)[3 * offset] = (a)[3];			\
    (dst)[4 * offset] = (a)[4];			\
    (dst)[5 * offset] = (a)[5];			\
    (dst)[6 * offset] = (a)[6];			\
    (dst)[7 * offset] = (a)[7];			\
}



// 反转位元;
// 30RISC;
unsigned int RevBit(unsigned int x)
{
    x = (x & 0x55555555) << 1 | (x >> 1) & 0x55555555;
    x = (x & 0x33333333) << 2 | (x >> 2) & 0x33333333;
    x = (x & 0x0f0f0f0f) << 4 | (x >> 4) & 0x0f0f0f0f;
    x = (x << 24) | ((x & 0xff00) << 8) | ((x >>8) & 0xff00) | (x >> 24);
    return x;
}

// shlr, 25RISC;
unsigned int KnuthRevBit(unsigned int x)
{
    unsigned int t;
    x = ROTATE_LEFT32(x, 15); // shlr(x, 15)
    t = (x ^ (x >> 10)) & 0x003f801f; x = (t | (t << 10)) ^ x;
    t = (x ^ (x >> 4)) & 0x0e038421; x = (t | (t << 4)) ^ x;
    t = (x ^ (x >> 2)) & 0x22488842; x = (t | (t << 2)) ^ x;
    return x;
}

unsigned long long KnuthRevBit(unsigned long long x)
{
    unsigned long long t;
#if 1
    // 24 RISC;
    x = (x << 32) | (x >> 32); // swap register halves.
    x = (x & 0x0001ffff0001ffffLL) << 15 | (x & 0xfffe0000fffe0000LL) >> 17; // rotate left 15.
    t = (x ^ (x >> 10)) & 0x003f801f003f801fLL;
    x = (t | (t << 10)) ^ x;
    t = (x ^ (x >> 4)) & 0x0e0384210e038421LL;
    x = (t | (t << 4)) ^ x;
    t = (x ^ (x >> 2)) & 0x2248884222488842LL;
    x = (t | (t << 2)) ^ x;
#else
    // 25 RISC;
    x = (x << 31) | (x >> 33);
    t = (x ^ (x >> 20)) & 0x00000fff800007ffLL;
    x = (t | (t << 20)) ^ x;
    t = (x ^ (x >> 8)) & 0x00f8000f80700807LL;
    x = (t | (t << 8)) ^ x;
    t = (x ^ (x >> 4)) & 0x0808708080807008LL;
    x = (t | (t << 4)) ^ x;
    t = (x ^ (x >> 2)) & 0x1111111111111111LL;
    x = (t | (t << 2)) ^ x;
#endif
    return x;
}

unsigned int RevMask(unsigned int x)
{
    static unsigned char table[256] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
        0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
        0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
        0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
        0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
        0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
        0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
        0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
        0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
        0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
        0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
        0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
        0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
        0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
        0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
        0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
        0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
    };
    unsigned int r = 0;

    for(int i=3; i>=0; i--){
        r = (r << 8) + table[x & 0xff];
        x = x >> 8;
    }
    return r;
}

// RISC 40 + 21 + 40
void TransPose8(unsigned char a[8], int m, int n, unsigned char b[8])
{
    unsigned long long x = 0;
    int i;

    // Load 8 bytes from the input array and pack them into x.
    for(i=0; i<=7; i++){
        x = (x << 8) | a[m*i];
    }

printf("x=0x%016llx\n", x);
    x = x & 0xaa55aa55aa55aa55LL        |
       (x & 0x00aa00aa00aa00aaLL) << 7  |
       (x >> 7) & 0x00aa00aa00aa00aaLL;
    x = x & 0xcccc3333cccc3333LL        |
       (x & 0x0000cccc0000ccccLL) << 14 |
       (x >> 14) & 0x0000cccc0000ccccLL;
    x = x & 0xf0f0f0f00f0f0f0fLL        |
       (x & 0x00000000f0f0f0f0LL) << 28 |
       (x >> 28) & 0x00000000f0f0f0f0LL;
printf("x=0x%016llx\n", x);

    // Store result into output array b.
    for(i=7; i>=0; i--){
        b[n*i] = x; x = x >> 8;
    }
}

#define TransPosMatrx8(x)   \
    x = x & 0xaa55aa55aa55aa55LL | (x & 0x00aa00aa00aa00aaLL) << 7 | (x >> 7) & 0x00aa00aa00aa00aaLL;   \
    x = x & 0xcccc3333cccc3333LL | (x & 0x0000cccc0000ccccLL) << 14 | (x >> 14) & 0x0000cccc0000ccccLL; \
    x = x & 0xf0f0f0f00f0f0f0fLL | (x & 0x00000000f0f0f0f0LL) << 28 | (x >> 28) & 0x00000000f0f0f0f0LL; \
//    x = BLSwap64(x);


#define TInt(a)        (int)(a)
#define TLLInt(a)      (long long)(a)

#if 0
#define TransIntArrToLL(a, lln1, lln2, lln3, lln4) \
{   \
    lln1 =  (TLLInt(GetIntByte0((a)[0])) << 56) |    \
            (TLLInt(GetIntByte0((a)[1])) << 48) |    \
            (TLLInt(GetIntByte0((a)[2])) << 40) |    \
            (TLLInt(GetIntByte0((a)[3])) << 32) |    \
            (TLLInt(GetIntByte0((a)[4])) << 24) |    \
            (TLLInt(GetIntByte0((a)[5])) << 16) |    \
            (TLLInt(GetIntByte0((a)[6])) << 8)  |    \
            (TLLInt(GetIntByte0((a)[7]))); \
    lln2 =  (TLLInt(GetIntByte1((a)[0])) << 56) |    \
            (TLLInt(GetIntByte1((a)[1])) << 48) |    \
            (TLLInt(GetIntByte1((a)[2])) << 40) |    \
            (TLLInt(GetIntByte1((a)[3])) << 32) |    \
            (TLLInt(GetIntByte1((a)[4])) << 24) |    \
            (TLLInt(GetIntByte1((a)[5])) << 16) |    \
            (TLLInt(GetIntByte1((a)[6])) << 8)  |    \
            (TLLInt(GetIntByte1((a)[7]))); \
    lln3 =  (TLLInt(GetIntByte2((a)[0])) << 56) |    \
            (TLLInt(GetIntByte2((a)[1])) << 48) |    \
            (TLLInt(GetIntByte2((a)[2])) << 40) |    \
            (TLLInt(GetIntByte2((a)[3])) << 32) |    \
            (TLLInt(GetIntByte2((a)[4])) << 24) |    \
            (TLLInt(GetIntByte2((a)[5])) << 16) |    \
            (TLLInt(GetIntByte2((a)[6])) << 8)  |    \
            (TLLInt(GetIntByte2((a)[7]))); \
    lln4 =  (TLLInt(GetIntByte3((a)[0])) << 56) |    \
            (TLLInt(GetIntByte3((a)[1])) << 48) |    \
            (TLLInt(GetIntByte3((a)[2])) << 40) |    \
            (TLLInt(GetIntByte3((a)[3])) << 32) |    \
            (TLLInt(GetIntByte3((a)[4])) << 24) |    \
            (TLLInt(GetIntByte3((a)[5])) << 16) |    \
            (TLLInt(GetIntByte3((a)[6])) << 8)  |    \
            (TLLInt(GetIntByte3((a)[7]))); \
}

#define TransLLToIntArr(lln1, lln2, lln3, lln4, a)  \
{   \
    SetIntByte0((a)[0], GetLLIntByte7(lln1));    \
    SetIntByte0((a)[1], GetLLIntByte6(lln1));    \
    SetIntByte0((a)[2], GetLLIntByte5(lln1));    \
    SetIntByte0((a)[3], GetLLIntByte4(lln1));    \
    SetIntByte0((a)[4], GetLLIntByte3(lln1));    \
    SetIntByte0((a)[5], GetLLIntByte2(lln1));    \
    SetIntByte0((a)[6], GetLLIntByte1(lln1));    \
    SetIntByte0((a)[7], GetLLIntByte0(lln1));    \
    SetIntByte1((a)[0], GetLLIntByte7(lln2));    \
    SetIntByte1((a)[1], GetLLIntByte6(lln2));    \
    SetIntByte1((a)[2], GetLLIntByte5(lln2));    \
    SetIntByte1((a)[3], GetLLIntByte4(lln2));    \
    SetIntByte1((a)[4], GetLLIntByte3(lln2));    \
    SetIntByte1((a)[5], GetLLIntByte2(lln2));    \
    SetIntByte1((a)[6], GetLLIntByte1(lln2));    \
    SetIntByte1((a)[7], GetLLIntByte0(lln2));    \
    SetIntByte2((a)[0], GetLLIntByte7(lln3));    \
    SetIntByte2((a)[1], GetLLIntByte6(lln3));    \
    SetIntByte2((a)[2], GetLLIntByte5(lln3));    \
    SetIntByte2((a)[3], GetLLIntByte4(lln3));    \
    SetIntByte2((a)[4], GetLLIntByte3(lln3));    \
    SetIntByte2((a)[5], GetLLIntByte2(lln3));    \
    SetIntByte2((a)[6], GetLLIntByte1(lln3));    \
    SetIntByte2((a)[7], GetLLIntByte0(lln3));    \
    SetIntByte3((a)[0], GetLLIntByte7(lln4));    \
    SetIntByte3((a)[1], GetLLIntByte6(lln4));    \
    SetIntByte3((a)[2], GetLLIntByte5(lln4));    \
    SetIntByte3((a)[3], GetLLIntByte4(lln4));    \
    SetIntByte3((a)[4], GetLLIntByte3(lln4));    \
    SetIntByte3((a)[5], GetLLIntByte2(lln4));    \
    SetIntByte3((a)[6], GetLLIntByte1(lln4));    \
    SetIntByte3((a)[7], GetLLIntByte0(lln4));    \
}

#else
#define TransIntArrToLL(a, lln1, lln2, lln3, lln4) \
{   \
    unsigned long long int llt1, llt2, llt3, llt4;  \
    llt1 = (TLLInt((a)[0]) << 32) + (a)[4];         \
    llt2 = (TLLInt((a)[1]) << 32) + (a)[5];         \
    llt3 = (TLLInt((a)[2]) << 32) + (a)[6];         \
    llt4 = (TLLInt((a)[3]) << 32) + (a)[7];         \
            \
    lln1 = ((llt1 & 0x000000FF000000FF) << 0x18) |  \
           ((llt2 & 0x000000FF000000FF) << 0x10) |  \
           ((llt3 & 0x000000FF000000FF) << 0x08) |  \
           ((llt4 & 0x000000FF000000FF) << 0x00);   \
            \
    lln2 = ((llt1 & 0x0000FF000000FF00) << 0x10) |  \
           ((llt2 & 0x0000FF000000FF00) << 0x08) |  \
           ((llt3 & 0x0000FF000000FF00) << 0x00) |  \
           ((llt4 & 0x0000FF000000FF00) >> 0x08);   \
            \
    lln3 = ((llt1 & 0x00FF000000FF0000) << 0x08) |  \
           ((llt2 & 0x00FF000000FF0000) >> 0x00) |  \
           ((llt3 & 0x00FF000000FF0000) >> 0x08) |  \
           ((llt4 & 0x00FF000000FF0000) >> 0x10);   \
            \
    lln4 = ((llt1 & 0xFF000000FF000000) >> 0x00) |  \
           ((llt2 & 0xFF000000FF000000) >> 0x08) |  \
           ((llt3 & 0xFF000000FF000000) >> 0x10) |  \
           ((llt4 & 0xFF000000FF000000) >> 0x18);   \
}

#define TransLLToIntArr(lln1, lln2, lln3, lln4, a)  \
{   \
    long long int llt1, llt2, llt3, llt4;           \
    llt1 =      \
        ((lln1 & 0x000000FF000000FF) << 0) |        \
        ((lln2 & 0x000000FF000000FF) << 8) |        \
        ((lln3 & 0x000000FF000000FF) << 16)|        \
        ((lln4 & 0x000000FF000000FF) << 24);        \
    llt2 =      \
        ((lln1 & 0x0000FF000000FF00) >> 8) |        \
        ((lln2 & 0x0000FF000000FF00) >> 0) |        \
        ((lln3 & 0x0000FF000000FF00) << 8) |        \
        ((lln4 & 0x0000FF000000FF00) << 16);        \
    llt3 =      \
        ((lln1 & 0x00FF000000FF0000) >> 16)|        \
        ((lln2 & 0x00FF000000FF0000) >> 8) |        \
        ((lln3 & 0x00FF000000FF0000) << 0) |        \
        ((lln4 & 0x00FF000000FF0000) << 8);         \
    llt4 =      \
        ((lln1 & 0xFF000000FF000000) >> 24)|        \
        ((lln2 & 0xFF000000FF000000) >> 16)|        \
        ((lln3 & 0xFF000000FF000000) >> 8) |        \
        ((lln4 & 0xFF000000FF000000) >> 0);         \
    (a)[7] = TInt(llt1);            \
    (a)[3] = TInt(llt1 >> 32);      \
    (a)[6] = TInt(llt2);            \
    (a)[2] = TInt(llt2 >> 32);      \
    (a)[5] = TInt(llt3);            \
    (a)[1] = TInt(llt3 >> 32);      \
    (a)[4] = TInt(llt4);            \
    (a)[0] = TInt(llt4 >> 32);      \
}
#endif

void TransPoseInt32(unsigned int a[32])
{
#if 1
    long long int ln00, ln01, ln02, ln03;
    long long int ln10, ln11, ln12, ln13;
    long long int ln20, ln21, ln22, ln23;
    long long int ln30, ln31, ln32, ln33;
#if 0
    void PrintMatrixWHStr(unsigned char *d, int w, int h);
    printf("a[0]=%x\n", a[0]);
    PrintMatrixWHStr((unsigned char*)a, 4, 32);
#endif

    TransIntArrToLL(a, ln00, ln01, ln02, ln03);
    TransIntArrToLL(a+8, ln10, ln11, ln12, ln13);
    TransIntArrToLL(a+16, ln20, ln21, ln22, ln23);
    TransIntArrToLL(a+24, ln30, ln31, ln32, ln33);

#if 0
    printf("ln1=%llx, ln2=%llx, ln3=%llx, ln4=%llx\n", ln00, ln01, ln02, ln03);
    PrintMatrixWHStr((unsigned char*)&ln00, 1, 8);
    PrintMatrixWHStr((unsigned char*)&ln01, 1, 8);
    PrintMatrixWHStr((unsigned char*)&ln02, 1, 8);
    PrintMatrixWHStr((unsigned char*)&ln03, 1, 8);
#endif

    TransPosMatrx8(ln00);
    TransPosMatrx8(ln01);
    TransPosMatrx8(ln02);
    TransPosMatrx8(ln03);

    TransPosMatrx8(ln10);
    TransPosMatrx8(ln11);
    TransPosMatrx8(ln12);
    TransPosMatrx8(ln13);

    TransPosMatrx8(ln20);
    TransPosMatrx8(ln21);
    TransPosMatrx8(ln22);
    TransPosMatrx8(ln23);

    TransPosMatrx8(ln30);
    TransPosMatrx8(ln31);
    TransPosMatrx8(ln32);
    TransPosMatrx8(ln33);

    unsigned int *b = a;
    memset(b, 0, sizeof(int)*32);
    TransLLToIntArr(ln00, ln10, ln20, ln30, b);
    TransLLToIntArr(ln01, ln11, ln21, ln31, b+8);
    TransLLToIntArr(ln02, ln12, ln22, ln32, b+16);
    TransLLToIntArr(ln03, ln13, ln23, ln33, b+24);

#if 0
    printf("ln1=%llx, ln2=%llx, ln3=%llx, ln4=%llx\n", ln00, ln01, ln02, ln03);
    PrintMatrixWHStr((unsigned char*)&ln00, 1, 8);
    PrintMatrixWHStr((unsigned char*)&ln01, 1, 8);
    PrintMatrixWHStr((unsigned char*)&ln02, 1, 8);
    PrintMatrixWHStr((unsigned char*)&ln03, 1, 8);
    printf("b[0]=%x\n", b[0]);
    PrintMatrixWHStr((unsigned char*)b, 4, 32);
#endif
#else
    unsigned int b[32] = {0};
    unsigned char *src = (unsigned char *)a;
    unsigned char *dst = (unsigned char *)b;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int idx1 = MatrixIdx2Dim8(i, j, 4);
            int idx2 = MatrixIdx2Dim8(j, i, 4);
            printf("i=%d, j=%d, idx1=%d, idx2=%d\n", i, j, idx1, idx2);
            TransPose8(src + idx1, 4, 4, dst + idx2);
        }
    }
    for(int i=0; i<32; i++) a[i] = b[i];
#endif
}

#define SwapIntArr(a)   \
{   \
    a[0] = BLSwap32(a[0]); \
    a[1] = BLSwap32(a[1]); \
    a[2] = BLSwap32(a[2]); \
    a[3] = BLSwap32(a[3]); \
    a[4] = BLSwap32(a[4]); \
    a[5] = BLSwap32(a[5]); \
    a[6] = BLSwap32(a[6]); \
    a[7] = BLSwap32(a[7]); \
    a[8] = BLSwap32(a[8]); \
    a[9] = BLSwap32(a[9]); \
    a[10] = BLSwap32(a[10]); \
    a[11] = BLSwap32(a[11]); \
    a[12] = BLSwap32(a[12]); \
    a[13] = BLSwap32(a[13]); \
    a[14] = BLSwap32(a[14]); \
    a[15] = BLSwap32(a[15]); \
    a[16] = BLSwap32(a[16]); \
    a[17] = BLSwap32(a[17]); \
    a[18] = BLSwap32(a[18]); \
    a[19] = BLSwap32(a[19]); \
    a[20] = BLSwap32(a[20]); \
    a[21] = BLSwap32(a[21]); \
    a[22] = BLSwap32(a[22]); \
    a[23] = BLSwap32(a[23]); \
    a[24] = BLSwap32(a[24]); \
    a[25] = BLSwap32(a[25]); \
    a[26] = BLSwap32(a[26]); \
    a[27] = BLSwap32(a[27]); \
    a[28] = BLSwap32(a[28]); \
    a[29] = BLSwap32(a[29]); \
    a[30] = BLSwap32(a[30]); \
    a[31] = BLSwap32(a[31]);    \
}

// RISC 1696
void TransPose32(unsigned int a[32])
{

    int j, k;
    unsigned int m, t;

    SwapIntArr(a);

    m = 0x0000ffff;
    for (j = 16; j != 0; j = j >> 1, m = m ^ (m << j)) {
//        printf("j=%d m=%08x\n", j, m);
        for (k = 0; k<32; k = (k + j + 1) & ~j) {
//            printf("swap(a%d, a%d, %d, m);\n", k, k+j, j);
            t = (a[k] ^ (a[k + j] >> j)) & m;
            a[k] = a[k] ^ t;
            a[k + j] = a[k + j] ^ (t << j);
        }
    }

    SwapIntArr(a);
}

void TransPose32(unsigned int a[32], int seek)
{
    int j, k;
    unsigned int m, t;

    for(int i=0; i<32; i++) a[i*seek] = BLSwap32(a[i*seek]);

    m = 0x0000ffff;
    for(j=16; j!=0; j=j>>1, m=m^(m<<j)){
        for(k=0; k<32; k=(k+j+1) & ~j){
            int idx = k * seek;
            int idx1 = (k+j) * seek;
            t = (a[idx] ^ (a[idx1] >> j)) & m;
            a[idx] = a[idx] ^ t;
            a[idx1] = a[idx1] ^ (t << j);
        }
    }

    for(int i=0; i<32; i++) a[i*seek] = BLSwap32(a[i*seek]);
}

#define TP32Swap(a0, a1, j, m) t = (a0 ^ (a1 >> j)) & m; a0 = a0 ^ t; a1 = a1 ^ (t << j);
void TransPose32(unsigned int a[32], unsigned int b[32])
{
    unsigned int m, t;
    unsigned int a0, a1, a2, a3, a4, a5, a6, a7,
            a8, a9, a10, a11, a12, a13, a14, a15,
            a16, a17, a18, a19, a20, a21, a22, a23,
            a24, a25, a26, a27, a28, a29, a30, a31;

    a0 = a[0]; a1 = a[1]; a2 = a[2]; a3 = a[3];
    a4 = a[4]; a5 = a[5]; a6 = a[6]; a7 = a[7];
    a8 = a[8]; a9 = a[9]; a10 = a[10]; a11 = a[11];
    a12 = a[12]; a13 = a[13]; a14 = a[14]; a15 = a[15];
    a16 = a[16]; a17 = a[17]; a18 = a[18]; a19 = a[19];
    a20 = a[20]; a21 = a[21]; a22 = a[22]; a23 = a[23];
    a24 = a[24]; a25 = a[25]; a26 = a[26]; a27 = a[27];
    a28 = a[28]; a29 = a[29]; a30 = a[30]; a31 = a[31];

    m = 0x0000ffff;
    TP32Swap(a0, a16, 16, m);
    TP32Swap(a1, a17, 16, m);
    TP32Swap(a2, a18, 16, m);
    TP32Swap(a3, a19, 16, m);
    TP32Swap(a4, a20, 16, m);
    TP32Swap(a5, a21, 16, m);
    TP32Swap(a6, a22, 16, m);
    TP32Swap(a7, a23, 16, m);
    TP32Swap(a8, a24, 16, m);
    TP32Swap(a9, a25, 16, m);
    TP32Swap(a10, a26, 16, m);
    TP32Swap(a11, a27, 16, m);
    TP32Swap(a12, a28, 16, m);
    TP32Swap(a13, a29, 16, m);
    TP32Swap(a14, a30, 16, m);
    TP32Swap(a15, a31, 16, m);

    m = 0x00ff00ff;
    TP32Swap(a0, a8, 8, m);
    TP32Swap(a1, a9, 8, m);
    TP32Swap(a2, a10, 8, m);
    TP32Swap(a3, a11, 8, m);
    TP32Swap(a4, a12, 8, m);
    TP32Swap(a5, a13, 8, m);
    TP32Swap(a6, a14, 8, m);
    TP32Swap(a7, a15, 8, m);
    TP32Swap(a16, a24, 8, m);
    TP32Swap(a17, a25, 8, m);
    TP32Swap(a18, a26, 8, m);
    TP32Swap(a19, a27, 8, m);
    TP32Swap(a20, a28, 8, m);
    TP32Swap(a21, a29, 8, m);
    TP32Swap(a22, a30, 8, m);
    TP32Swap(a23, a31, 8, m);

    m = 0x0f0f0f0f;
    TP32Swap(a0, a4, 4, m);
    TP32Swap(a1, a5, 4, m);
    TP32Swap(a2, a6, 4, m);
    TP32Swap(a3, a7, 4, m);
    TP32Swap(a8, a12, 4, m);
    TP32Swap(a9, a13, 4, m);
    TP32Swap(a10, a14, 4, m);
    TP32Swap(a11, a15, 4, m);
    TP32Swap(a16, a20, 4, m);
    TP32Swap(a17, a21, 4, m);
    TP32Swap(a18, a22, 4, m);
    TP32Swap(a19, a23, 4, m);
    TP32Swap(a24, a28, 4, m);
    TP32Swap(a25, a29, 4, m);
    TP32Swap(a26, a30, 4, m);
    TP32Swap(a27, a31, 4, m);

    m = 0x33333333;
    TP32Swap(a0, a2, 2, m);
    TP32Swap(a1, a3, 2, m);
    TP32Swap(a4, a6, 2, m);
    TP32Swap(a5, a7, 2, m);
    TP32Swap(a8, a10, 2, m);
    TP32Swap(a9, a11, 2, m);
    TP32Swap(a12, a14, 2, m);
    TP32Swap(a13, a15, 2, m);
    TP32Swap(a16, a18, 2, m);
    TP32Swap(a17, a19, 2, m);
    TP32Swap(a20, a22, 2, m);
    TP32Swap(a21, a23, 2, m);
    TP32Swap(a24, a26, 2, m);
    TP32Swap(a25, a27, 2, m);
    TP32Swap(a28, a30, 2, m);
    TP32Swap(a29, a31, 2, m);

    m = 0x55555555;
    TP32Swap(a0, a1, 1, m);
    TP32Swap(a2, a3, 1, m);
    TP32Swap(a4, a5, 1, m);
    TP32Swap(a6, a7, 1, m);
    TP32Swap(a8, a9, 1, m);
    TP32Swap(a10, a11, 1, m);
    TP32Swap(a12, a13, 1, m);
    TP32Swap(a14, a15, 1, m);
    TP32Swap(a16, a17, 1, m);
    TP32Swap(a18, a19, 1, m);
    TP32Swap(a20, a21, 1, m);
    TP32Swap(a22, a23, 1, m);
    TP32Swap(a24, a25, 1, m);
    TP32Swap(a26, a27, 1, m);
    TP32Swap(a28, a29, 1, m);
    TP32Swap(a30, a31, 1, m);

#if 0
    b[0] = a0; b[1] = a1; b[2] = a2; b[3] = a3;
    b[4] = a4; b[5] = a5; b[6] = a6; b[7] = a7;
    b[8] = a8; b[9] = a9; b[10] = a10; b[11] = a11;
    b[12] = a12; b[13] = a13; b[14] = a14; b[15] = a15;
    b[16] = a16; b[17] = a17; b[18] = a18; b[19] = a19;
    b[20] = a20; b[21] = a21; b[22] = a22; b[23] = a23;
    b[24] = a24; b[25] = a25; b[26] = a26; b[27] = a27;
    b[28] = a28; b[29] = a29; b[30] = a30; b[31] = a31;
#else
    b[0] = a24; b[1] = a25; b[2] = a26; b[3] = a27;
    b[4] = a28; b[5] = a29; b[6] = a30; b[7] = a31;
    b[8] = a16; b[9] = a17; b[10] = a18; b[11] = a19;
    b[12] = a20; b[13] = a21; b[14] = a22; b[15] = a23;
    b[16] = a8; b[17] = a9; b[18] = a10; b[19] = a11;
    b[20] = a12; b[21] = a13; b[22] = a14; b[23] = a15;
    b[24] = a0; b[25] = a1; b[26] = a2; b[27] = a3;
    b[28] = a4; b[29] = a5; b[30] = a6; b[31] = a7;
#endif
    SwapIntArr(b);
}


void TransPoseMatrix(unsigned int *src, int w, int h, unsigned int *dst)
{
    int cw = w;
    int ch = h / 32;

    for(int i=0; i<ch; i++){
        for(int j=0; j<cw; j++){
#if 0
            int idx = MatrixIdx2Dim32(j, i, ch);
            // 按32个Int为一个数据单元，进行单元级的矩阵转置;
            int idx1 = MatrixIdx3Dim32(i, j, 0, cw); //i*cw*32 + j;
            int idx2 = MatrixIdx3Dim32(j, i, 0, ch); //j*ch*32 + i;
            for(int k=0; k<32; k++){
//                printf("i=%d, j=%d, k=%d, idx1=%d, idx2=%d, cw=%d, ch=%d\n", i, j, k, idx1, idx2, cw, ch);
                dst[idx2] = src[idx1];
                idx1 += cw;
                idx2 += ch;
            }
            // 对32个Int进行32bit的矩阵转置;
            TransPose32(dst+idx, ch);
#elseif 0
            unsigned int tmp[32] = { 0 };
            int idx1 = MatrixIdx3Dim32(i, j, 0, cw);
            unsigned int *pSrcOffset = src + idx1;
            for (int k = 0; k<32; k++) {
//                tmp[k] = pSrcOffset[0];
//                pSrcOffset += cw;
                tmp[k] = src[idx1];
                idx1 += cw;
            }
//            TransPose32(tmp);
            TransPoseInt32(tmp);
//            TransPose32(tmp, tmp);
            int idx2 = MatrixIdx3Dim32(j, i, 0, ch);
            unsigned int *pDstOffset = dst + idx2;
            for (int k = 0; k<32; k++) {
//                pDstOffset[0] = tmp[k];
//                pDstOffset += ch;
                dst[idx2] = tmp[k];
                idx2 += ch;
            }
#else
            unsigned int tmp[32] = { 0 };
            int idx1 = MatrixIdx3Dim32(i, j, 0, cw);
            unsigned int *pSrcOffset = src + idx1;
#if 0
            tmp[0] = pSrcOffset[0*cw];
            tmp[1] = pSrcOffset[1*cw];
            tmp[2] = pSrcOffset[2*cw];
            tmp[3] = pSrcOffset[3*cw];
            tmp[4] = pSrcOffset[4*cw];
            tmp[5] = pSrcOffset[5*cw];
            tmp[6] = pSrcOffset[6*cw];
            tmp[7] = pSrcOffset[7*cw];
            tmp[8] = pSrcOffset[8*cw];
            tmp[9] = pSrcOffset[9*cw];
            tmp[10] = pSrcOffset[10*cw];
            tmp[11] = pSrcOffset[11*cw];
            tmp[12] = pSrcOffset[12*cw];
            tmp[13] = pSrcOffset[13*cw];
            tmp[14] = pSrcOffset[14*cw];
            tmp[15] = pSrcOffset[15*cw];
            tmp[16] = pSrcOffset[16*cw];
            tmp[17] = pSrcOffset[17*cw];
            tmp[18] = pSrcOffset[18*cw];
            tmp[19] = pSrcOffset[19*cw];
            tmp[20] = pSrcOffset[20*cw];
            tmp[21] = pSrcOffset[21*cw];
            tmp[22] = pSrcOffset[22*cw];
            tmp[23] = pSrcOffset[23*cw];
            tmp[24] = pSrcOffset[24*cw];
            tmp[25] = pSrcOffset[25*cw];
            tmp[26] = pSrcOffset[26*cw];
            tmp[27] = pSrcOffset[27*cw];
            tmp[28] = pSrcOffset[28*cw];
            tmp[29] = pSrcOffset[29*cw];
            tmp[30] = pSrcOffset[30*cw];
            tmp[31] = pSrcOffset[31*cw];
#else
            GetSeekArray(tmp+0, pSrcOffset, cw); pSrcOffset += cw * 8;
            GetSeekArray(tmp+8, pSrcOffset, cw); pSrcOffset += cw * 8;
            GetSeekArray(tmp+16, pSrcOffset, cw); pSrcOffset += cw * 8;
            GetSeekArray(tmp+24, pSrcOffset, cw); pSrcOffset += cw * 8;
#endif

//            TransPose32(tmp);
//            TransPoseInt32(tmp);
            TransPose32(tmp, tmp);
            int idx2 = MatrixIdx3Dim32(j, i, 0, ch);
            unsigned int *pDstOffset = dst + idx2;
#if 0
            pDstOffset[0*ch] = tmp[0];
            pDstOffset[1*ch] = tmp[1];
            pDstOffset[2*ch] = tmp[2];
            pDstOffset[3*ch] = tmp[3];
            pDstOffset[4*ch] = tmp[4];
            pDstOffset[5*ch] = tmp[5];
            pDstOffset[6*ch] = tmp[6];
            pDstOffset[7*ch] = tmp[7];
            pDstOffset[8*ch] = tmp[8];
            pDstOffset[9*ch] = tmp[9];
            pDstOffset[10*ch] = tmp[10];
            pDstOffset[11*ch] = tmp[11];
            pDstOffset[12*ch] = tmp[12];
            pDstOffset[13*ch] = tmp[13];
            pDstOffset[14*ch] = tmp[14];
            pDstOffset[15*ch] = tmp[15];
            pDstOffset[16*ch] = tmp[16];
            pDstOffset[17*ch] = tmp[17];
            pDstOffset[18*ch] = tmp[18];
            pDstOffset[19*ch] = tmp[19];
            pDstOffset[20*ch] = tmp[20];
            pDstOffset[21*ch] = tmp[21];
            pDstOffset[22*ch] = tmp[22];
            pDstOffset[23*ch] = tmp[23];
            pDstOffset[24*ch] = tmp[24];
            pDstOffset[25*ch] = tmp[25];
            pDstOffset[26*ch] = tmp[26];
            pDstOffset[27*ch] = tmp[27];
            pDstOffset[28*ch] = tmp[28];
            pDstOffset[29*ch] = tmp[29];
            pDstOffset[30*ch] = tmp[30];
            pDstOffset[31*ch] = tmp[31];
#else
            SetSeekArray(tmp+0, pDstOffset, ch); pDstOffset += ch * 8;
            SetSeekArray(tmp+8, pDstOffset, ch); pDstOffset += ch * 8;
            SetSeekArray(tmp+16, pDstOffset, ch); pDstOffset += ch * 8;
            SetSeekArray(tmp+24, pDstOffset, ch); pDstOffset += ch * 8;
#endif

#endif
        }
    }
}

void TransPoseInternalMatrix(unsigned int *a, int w, int h, unsigned int *t, int aOffset, int tOffset)
{
    int cw = w;
    int ch = h / 32;

    for(int i=0; i<ch; i++){
        for(int j=0; j<cw; j++){
            int idx = MatrixIdx2Dim32(j, i, tOffset);
            for(int k=0; k<32; k++){
                int idx1 = MatrixIdx3Dim32(i, j, k, aOffset);
                int idx2 = MatrixIdx3Dim32(j, i, k, tOffset);
//                printf("i=%d, j=%d, k=%d, idx1=%d, idx2=%d, cw=%d, ch=%d\n", i, j, k, idx1, idx2, cw, ch);
                t[idx2] = a[idx1];
            }
            TransPose32(t+idx, tOffset);
        }
    }
}

int TransPoseBigMatrix(unsigned int *a, int w, int h, unsigned int *t, int maxMatrixSize)
{
    unsigned int *tmpdst = NULL;

    if(!a || !t){LogPos(1); return 0;}
    if(w <= 0 || h <= 0 || (h % 32 != 0)){LogPos(1); return 0;}

    tmpdst = MCalloc(unsigned int, maxMatrixSize/sizeof(int) + 1);
    if(!tmpdst){LogPos(1); return 0;}

    int cell = 32;
    int mSize = w * h * sizeof(int);
    int loopNum = ceil((double)mSize / maxMatrixSize);
    int cw = w;
    int ch = h / cell;
    int lh = ceil(1.0 * ch / loopNum);

    printf("maxMatrixSize=%.3f, mSize=%.3f, loopNum=%d, cw=%d, ch=%d, lh=%d, w=%d, h=%d\n",
           (double)maxMatrixSize/SIZE_1M, (double)mSize/SIZE_1M, loopNum, cw, ch, lh, w, h);

    unsigned int *ap = a;
    int offx = 0;
    for(int i=0; i<loopNum; i++){
        printf("i=%d, cw=%d, lh=%d\n", i, cw, lh);
        TransPoseMatrix(ap, cw, lh*cell, tmpdst);
#if 0
        char *GetMatrixStr(unsigned int *d, int w, int h);
        int CheckMatrix(char *a, char *b, int w, int h);
        void PrintMatrixWH(unsigned int *d, int w, int h);
        char *mStr1 = GetMatrixStr(ap, cw, lh*cell);
        char *mStr2 = GetMatrixStr(tmpdst, lh, cw*cell);
        int fNum = CheckMatrix(mStr1, mStr2, cw*cell, lh*cell);
        printf("fNum=%d\n", fNum);
#endif
        for(int j=0; j<cw*cell; j++){
#if 0
            int dOffset = j * ch + offx;
            int sOffset = lh * j;
//            printf("doff=%d, soff=%d\n", dOffset, sOffset);
            memcpy(t+dOffset, tmpdst+sOffset, lh*sizeof(int));
#else
            unsigned int *pDstOff = t + offx;
            unsigned int *pSrcOff = tmpdst;
            int lSize = lh * sizeof(int);
            for (int j = 0; j<cw*cell; j++) {
                memcpy(pDstOff, pSrcOff, lSize);
                pDstOff += ch;
                pSrcOff += lh;
            }
#endif
        }
        offx += lh;
        ap += cw * lh * cell;
        if(offx + lh > ch) lh = ch - offx;
    }

    MFree(tmpdst);
    return 1;
}

/*
    w: 整数宽;
    h: 数据行数;
*/
void TransPoseMatrix8(unsigned int *src, int w, int h, unsigned int *dst)
{
    const int BitNumPerCell = 32;
    int cw = w;
    int ch = h / BitNumPerCell;

    for(int i=0; i<ch; i++){
        for(int j=0; j<cw; j++){
            int idx1 = MatrixIdx3Dim32(i, j, 0, cw);
            int idx2 = MatrixIdx3Dim32(j, i, 0, ch);
            unsigned int *pSrcOffset = src + idx1;
            unsigned int *pDstOffset = dst + idx2;
            long long int ln[4][4] = {0};
#if 1
            unsigned long long data0, data1, data2, data3, data4, data5, data6, data7;
            for (int k = 0; k<4; k++) {
                data0 = pSrcOffset[4 * cw] + ((unsigned long long)pSrcOffset[0 * cw] << 32);
                data1 = pSrcOffset[5 * cw] + ((unsigned long long)pSrcOffset[1 * cw] << 32);
                data2 = pSrcOffset[6 * cw] + ((unsigned long long)pSrcOffset[2 * cw] << 32);
                data3 = pSrcOffset[7 * cw] + ((unsigned long long)pSrcOffset[3 * cw] << 32);

                data4 =
                    ((data0 & 0x000000FF000000FF) << 0x18) |
                    ((data1 & 0x000000FF000000FF) << 0x10) |
                    ((data2 & 0x000000FF000000FF) << 0x08) |
                    ((data3 & 0x000000FF000000FF) << 0x00);

                data5 =
                    ((data0 & 0x0000FF000000FF00) << 0x10) |
                    ((data1 & 0x0000FF000000FF00) << 0x08) |
                    ((data2 & 0x0000FF000000FF00) << 0x00) |
                    ((data3 & 0x0000FF000000FF00) >> 0x08);

                data6 =
                    ((data0 & 0x00FF000000FF0000) << 0x08) |
                    ((data1 & 0x00FF000000FF0000) >> 0x00) |
                    ((data2 & 0x00FF000000FF0000) >> 0x08) |
                    ((data3 & 0x00FF000000FF0000) >> 0x10);

                data7 =
                    ((data0 & 0xFF000000FF000000) >> 0x00) |
                    ((data1 & 0xFF000000FF000000) >> 0x08) |
                    ((data2 & 0xFF000000FF000000) >> 0x10) |
                    ((data3 & 0xFF000000FF000000) >> 0x18);

                TransPosMatrx8(data4);
                TransPosMatrx8(data5);
                TransPosMatrx8(data6);
                TransPosMatrx8(data7);
                ln[k][0] = data4;
                ln[k][1] = data5;
                ln[k][2] = data6;
                ln[k][3] = data7;
                pSrcOffset += 8*cw;
            }
            for(int k=0; k<4; k++) {
                data0 = ln[0][k];
                data1 = ln[1][k];
                data2 = ln[2][k];
                data3 = ln[3][k];

                data4 =
                    ((data0 & 0x000000FF000000FF) << 0x00) |
                    ((data1 & 0x000000FF000000FF) << 0x08) |
                    ((data2 & 0x000000FF000000FF) << 0x10) |
                    ((data3 & 0x000000FF000000FF) << 0x18);

                data5 =
                    ((data0 & 0x0000FF000000FF00) >> 0x08) |
                    ((data1 & 0x0000FF000000FF00) >> 0x00) |
                    ((data2 & 0x0000FF000000FF00) << 0x08) |
                    ((data3 & 0x0000FF000000FF00) << 0x10);

                data6 =
                    ((data0 & 0x00FF000000FF0000) >> 0x10) |
                    ((data1 & 0x00FF000000FF0000) >> 0x08) |
                    ((data2 & 0x00FF000000FF0000) << 0x00) |
                    ((data3 & 0x00FF000000FF0000) << 0x08);

                data7 =
                    ((data0 & 0xFF000000FF000000) >> 0x18) |
                    ((data1 & 0xFF000000FF000000) >> 0x10) |
                    ((data2 & 0xFF000000FF000000) >> 0x08) |
                    ((data3 & 0xFF000000FF000000) >> 0x00);

                pDstOffset[7 * ch] = (unsigned int)data4;
                pDstOffset[3 * ch] = data4 >> 32;
                pDstOffset[6 * ch] = (unsigned int)data5;
                pDstOffset[2 * ch] = data5 >> 32;
                pDstOffset[5 * ch] = (unsigned int)data6;
                pDstOffset[1 * ch] = data6 >> 32;
                pDstOffset[4 * ch] = (unsigned int)data7;
                pDstOffset[0 * ch] = data7 >> 32;

                pDstOffset += 8*ch;
            }
#else
            unsigned int a[8] = {0};
            unsigned long long lln1, lln2, lln3, lln4;
            for (int k = 0; k<4; k++) {
                a[0] = pSrcOffset[0*cw];
                a[1] = pSrcOffset[1*cw];
                a[2] = pSrcOffset[2*cw];
                a[3] = pSrcOffset[3*cw];
                a[4] = pSrcOffset[4*cw];
                a[5] = pSrcOffset[5*cw];
                a[6] = pSrcOffset[6*cw];
                a[7] = pSrcOffset[7*cw];
                pSrcOffset += 8 * cw;

                TransIntArrToLL(a, lln1, lln2, lln3, lln4);
//                printf("lln1=0x%016llx\n", lln1);
//                printf("lln2=0x%016llx\n", lln2);
//                printf("lln3=0x%016llx\n", lln3);
//                printf("lln4=0x%016llx\n", lln4);
                TransPosMatrx8(lln1);
                TransPosMatrx8(lln2);
                TransPosMatrx8(lln3);
                TransPosMatrx8(lln4);
                ln[k][0] = lln1;
                ln[k][1] = lln2;
                ln[k][2] = lln3;
                ln[k][3] = lln4;
            }

            for(int k=0; k<4; k++) {
                lln1 = ln[0][k];
                lln2 = ln[1][k];
                lln3 = ln[2][k];
                lln4 = ln[3][k];
                TransLLToIntArr(lln1, lln2, lln3, lln4, a);

                pDstOffset[0*ch] = a[0];
                pDstOffset[1*ch] = a[1];
                pDstOffset[2*ch] = a[2];
                pDstOffset[3*ch] = a[3];
                pDstOffset[4*ch] = a[4];
                pDstOffset[5*ch] = a[5];
                pDstOffset[6*ch] = a[6];
                pDstOffset[7*ch] = a[7];
                pDstOffset += 8*ch;
            }
#endif
        }
    }
}

void TransPoseChar(unsigned char *dst, unsigned char *src, int w, int h)
{
    const int BitNumPerCell = 8;
    int cw = w;
    int ch = h / BitNumPerCell;

    for(int i=0; i<ch; i++){
        for(int j=0; j<cw; j++){
            unsigned char c_src[BitNumPerCell] = { 0 };
            unsigned char c_dst[BitNumPerCell] = { 0 };
            int idx1 = MatrixIdx3Dim8(i, j, 0, cw);
            for (int k = 0; k<BitNumPerCell; k++) {
                c_src[k] = src[idx1];
                idx1 += cw;
            }
            TransPose8(c_src, 1, 1, c_dst);
            int idx2 = MatrixIdx3Dim8(j, i, 0, ch);
            for (int k = 0; k<BitNumPerCell; k++) {
                dst[idx2] = c_dst[k];
                idx2 += ch;
            }
        }
    }
}
