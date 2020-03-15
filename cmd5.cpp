#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "ccommon.h"
#include "cmd5.h"



#define F(x, y, z)		((x & y) | ((~x) & z))
#define G(x, y, z)		((x & z) | (y & (~z)))
#define H(x, y, z)		(x ^ y ^ z)
#define I(x, y, z)		(y ^ (x | (~z)))

#define FF(a, b, c, d, Mj, s, ti)	a = b + (ROTATE_LEFT32(a + F(b, c, d) + Mj + ti, s))
#define GG(a, b, c, d, Mj, s, ti)	a = b + (ROTATE_LEFT32(a + G(b, c, d) + Mj + ti, s))
#define HH(a, b, c, d, Mj, s, ti)	a = b + (ROTATE_LEFT32(a + H(b, c, d) + Mj + ti, s))
#define II(a, b, c, d, Mj, s, ti)	a = b + (ROTATE_LEFT32(a + I(b, c, d) + Mj + ti, s))

/* 以大端字节序来表示，他们分别为：
A=0x01234567，
B=0x89ABCDEF，
C=0xFEDCBA98，
D=0x76543210。
*/
#define AB  0x01234567
#define BB  0x89ABCDEF
#define CB  0xFEDCBA98
#define DB  0x76543210

#define AL	0x67452301
#define BL	0xefcdab89
#define CL	0x98badcfe
#define DL	0x10325476

//#define IsLittleEndian()       ({int a=1; *(char *)&a == a;})

//向左位移数
const unsigned int s[]={7,12,17,22,7,12,17,22,7,12,17,22,7,
        12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
        15,21,6,10,15,21,6,10,15,21,6,10,15,21};

//常量ti unsigned int(abs(sin(i+1))*(2pow32))
const unsigned int t[]={
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8,
        0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193,
        0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51,
        0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905,
        0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681,
        0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60,
        0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244,
        0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
        0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314,
        0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

const char str16[]="0123456789abcdef";

int IsLittleEndian()
{
    int a=1;

    return *(char *)&a == a;
}

void UCharToUInt(int size, unsigned char *input, unsigned int *output)
{
    int i;

    if(IsLittleEndian()){
        for(i=0; i<size; i++){
            output[i>>2] |= input[i] << (i % 4) * 8;
        }
    }else{
        for(i=0; i<size; i++){
            output[i>>2] |= input[i] << (24 - (i % 4) * 8);
        }
    }

}

void UIntToUChar(int size, unsigned int *input, unsigned char *output)
{
    int i, j;

    if(IsLittleEndian()){
        for(i=0, j=0; i<size; i++, j+=4){
            output[j+0] = input[i] & 0xff;
            output[j+1] = (input[i] >> 8) & 0xff;
            output[j+2] = (input[i] >> 16) & 0xff;
            output[j+3] = (input[i] >> 24) & 0xff;
        }
    }else{
        for(i=0, j=0; i<size; i++, j+=4){
            output[j+0] = (input[i] >> 24) & 0xff;
            output[j+1] = (input[i] >> 16) & 0xff;
            output[j+2] = (input[i] >> 8) & 0xff;
            output[j+3] = input[i] & 0xff;
        }
    }
}


void MD5ProcessM(unsigned int M[16], unsigned int *tmpA, unsigned int *tmpB, unsigned int *tmpC, unsigned int *tmpD)
{
    unsigned int a, b, c, d;

    a = *tmpA;
    b = *tmpB;
    c = *tmpC;
    d = *tmpD;

    FF(a, b, c, d, M[ 0], s[0], t[0]);
    FF(d, a, b, c, M[ 1], s[1], t[1]);
    FF(c, d, a, b, M[ 2], s[2], t[2]);
    FF(b, c, d, a, M[ 3], s[3], t[3]);
    FF(a, b, c, d, M[ 4], s[4], t[4]);
    FF(d, a, b, c, M[ 5], s[5], t[5]);
    FF(c, d, a, b, M[ 6], s[6], t[6]);
    FF(b, c, d, a, M[ 7], s[7], t[7]);
    FF(a, b, c, d, M[ 8], s[8], t[8]);
    FF(d, a, b, c, M[ 9], s[9], t[9]);
    FF(c, d, a, b, M[10], s[10], t[10]);
    FF(b, c, d, a, M[11], s[11], t[11]);
    FF(a, b, c, d, M[12], s[12], t[12]);
    FF(d, a, b, c, M[13], s[13], t[13]);
    FF(c, d, a, b, M[14], s[14], t[14]);
    FF(b, c, d, a, M[15], s[15], t[15]);

    GG(a, b, c, d, M[ 1], s[16], t[16]);
    GG(d, a, b, c, M[ 6], s[17], t[17]);
    GG(c, d, a, b, M[11], s[18], t[18]);
    GG(b, c, d, a, M[ 0], s[19], t[19]);
    GG(a, b, c, d, M[ 5], s[20], t[20]);
    GG(d, a, b, c, M[10], s[21], t[21]);
    GG(c, d, a, b, M[15], s[22], t[22]);
    GG(b, c, d, a, M[ 4], s[23], t[23]);
    GG(a, b, c, d, M[ 9], s[24], t[24]);
    GG(d, a, b, c, M[14], s[25], t[25]);
    GG(c, d, a, b, M[ 3], s[26], t[26]);
    GG(b, c, d, a, M[ 8], s[27], t[27]);
    GG(a, b, c, d, M[13], s[28], t[28]);
    GG(d, a, b, c, M[ 2], s[29], t[29]);
    GG(c, d, a, b, M[ 7], s[30], t[30]);
    GG(b, c, d, a, M[12], s[31], t[31]);

    HH(a, b, c, d, M[ 5], s[32], t[32]);
    HH(d, a, b, c, M[ 8], s[33], t[33]);
    HH(c, d, a, b, M[11], s[34], t[34]);
    HH(b, c, d, a, M[14], s[35], t[35]);
    HH(a, b, c, d, M[ 1], s[36], t[36]);
    HH(d, a, b, c, M[ 4], s[37], t[37]);
    HH(c, d, a, b, M[ 7], s[38], t[38]);
    HH(b, c, d, a, M[10], s[39], t[39]);
    HH(a, b, c, d, M[13], s[40], t[40]);
    HH(d, a, b, c, M[ 0], s[41], t[41]);
    HH(c, d, a, b, M[ 3], s[42], t[42]);
    HH(b, c, d, a, M[ 6], s[43], t[43]);
    HH(a, b, c, d, M[ 9], s[44], t[44]);
    HH(d, a, b, c, M[12], s[45], t[45]);
    HH(c, d, a, b, M[15], s[46], t[46]);
    HH(b, c, d, a, M[ 2], s[47], t[47]);

    II(a, b, c, d, M[ 0], s[48], t[48]);
    II(d, a, b, c, M[ 7], s[49], t[49]);
    II(c, d, a, b, M[14], s[50], t[50]);
    II(b, c, d, a, M[ 5], s[51], t[51]);
    II(a, b, c, d, M[12], s[52], t[52]);
    II(d, a, b, c, M[ 3], s[53], t[53]);
    II(c, d, a, b, M[10], s[54], t[54]);
    II(b, c, d, a, M[ 1], s[55], t[55]);
    II(a, b, c, d, M[ 8], s[56], t[56]);
    II(d, a, b, c, M[15], s[57], t[57]);
    II(c, d, a, b, M[ 6], s[58], t[58]);
    II(b, c, d, a, M[13], s[59], t[59]);
    II(a, b, c, d, M[ 4], s[60], t[60]);
    II(d, a, b, c, M[11], s[61], t[61]);
    II(c, d, a, b, M[ 2], s[62], t[62]);
    II(b, c, d, a, M[ 9], s[63], t[63]);

    *tmpA += a;
    *tmpB += b;
    *tmpC += c;
    *tmpD += d;
}

int MD5PadData(UInt64 length, int size, unsigned char *str, unsigned int M0[16], unsigned int M1[16])
{

    UCharToUInt(size, str, M0);

    if(IsLittleEndian()){
        M0[size>>2] |= 0x80 << (size % 4) * 8;
    }else{
        M0[size>>2] |= 0x80 << (24 - (size % 4) * 8);
    }

    if(size < 56){
        *(UInt64 *)&M0[14] = length*8;
        return 1;
    }else{
        *(UInt64 *)&M1[14] = length*8;
        return 2;
    }
}

int MD5EncodeByte(int isEnd, UInt64 tSize, int strSize, unsigned char *str,
                  unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    int i;
    int gNum;   // group num
    int rNum;   // remain num
    unsigned int M0[16]={0}, M1[16]={0};
    unsigned char *sp=NULL;
    int ret=0;

    gNum = strSize / 64;
    rNum = strSize % 64;

    if(!isEnd && rNum) return -1;

    for(i=0; i<gNum; i++){
        memset(M0, 0, sizeof(M0));
        sp = str + i * 64;
        UCharToUInt(64, sp, M0);
        MD5ProcessM(M0, a, b, c, d);
    }

    //
    if(isEnd){
        sp = str + gNum * 64;
        memset(M0, 0, sizeof(M0));
        memset(M1, 0, sizeof(M1));
//        LogFile("tsize=%lld, strSize=%d, gNum=%d, rNum=%d\n", tSize, strSize, gNum, rNum);
        ret = MD5PadData(tSize, rNum, sp, M0, M1);
//        LogFile("ret=%d\n", ret);
//        for(i=0; i<16; i++){
//            char *p = (char *) (M0 + i);
//            LogFile("M0[%d]=%x, %x %x %x %x\n", i, M0[i],
//                   p[0]&0xff, p[1]&0xff, p[2]&0xff, p[3]&0xff);
//        }

        MD5ProcessM(M0, a, b, c, d);
//        LogFile("a=%x, b=%x, c=%x, d=%x\n", *a, *b, *c, *d);
        if(ret > 1) MD5ProcessM(M1, a, b, c, d);
    }

    return 0;
}

void UCharToHexStr(int size, const unsigned char* input, unsigned char *output)
{
    int i,j;
    const char MapByteToHex[16] =
    {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
     };

    for (i=0, j=0; i < size; i++)
    {
        unsigned int a = input[i] / 16;
        unsigned int b = input[i] % 16;
        output[j++] = MapByteToHex[a];
        output[j++] = MapByteToHex[b];
    }

}

void MD5InitMagicVal(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    if(IsLittleEndian()){
        *a = AL; *b = BL; *c = CL; *d = DL;
    }else{
        *a = AB; *b = BB; *c = CB; *d = DB;
    }
}

int MD5EncodeStr(int size, unsigned char *str, unsigned char md5Val[32])
{
    unsigned int magicVal[4]={0};
    unsigned char ret[16]={0};

    if(size < 0) return -1;
    if(size > 0 && !str) return -1;

    // init a, b, c, d
    MD5InitMagicVal(&magicVal[0], &magicVal[1], &magicVal[2], &magicVal[3]);
//    LogFile("a=%x, b=%x, c=%x, d=%x\n", magicVal[0], magicVal[1], magicVal[2], magicVal[3]);

//    char *p = (char *) magicVal;
//    LogFile("%x, %x, %x, %x\n", p[0], p[1], p[2], p[3]);

    MD5EncodeByte(1, size, size, str, &magicVal[0], &magicVal[1], &magicVal[2], &magicVal[3]);

    UIntToUChar(4, magicVal, ret);
//    LogFile("magicVal: \n");
//    for(int i=0; i<16; i++) LogFile("%x ", ret[i]);
//    LogFile("\n");

    UCharToHexStr(sizeof(ret), ret, md5Val);
//    LogFile("md5Val: \n");
//    for(int i=0; i<32; i++) LogFile("%c", md5Val[i]);
//    LogFile("\n");

    return 0;
}


int MD5EncodeFile(const char *filename, unsigned char md5Val[32])
{
    int readSize;
    FILE *fp=NULL;
    unsigned int magicVal[4]={0};
    unsigned char ret[16]={0};
    unsigned char buf[1024];
    Int64 fileSize=0;
    int isEnd=0;

    if(!filename) return -1;

    // init a, b, c, d
    MD5InitMagicVal(&magicVal[0], &magicVal[1], &magicVal[2], &magicVal[3]);

    fp = fopen(filename, "rb");
    if(!fp) return -1;

//    _fseeki64(fp, 0, SEEK_END);

    while(!feof(fp)){
        readSize = fread(buf, sizeof(char), sizeof(buf), fp);
        fileSize += readSize;

        if(readSize != sizeof(buf)) isEnd = 1;

        MD5EncodeByte(isEnd, fileSize, readSize, buf,
                      &magicVal[0], &magicVal[1],
                &magicVal[2], &magicVal[3]);
    }

    if(!isEnd){
        MD5EncodeByte(1, fileSize, 0, buf,
                      &magicVal[0], &magicVal[1],
                &magicVal[2], &magicVal[3]);
    }

    UIntToUChar(4, magicVal, ret);

    UCharToHexStr(sizeof(ret), ret, md5Val);
    LogFile("md5Val: \n");
    for(int i=0; i<32; i++) LogFile("%c", md5Val[i]);
    LogFile("\n");

    if(fp) fclose(fp);
    return 0;
}
