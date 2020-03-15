#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "ccommon.h"
#include "cdes.h"

// Table - s1
const  unsigned char s1[4][16] = {
    14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
    0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
    4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
    15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 };

// Table - s2
const unsigned char s2[4][16] = {
    15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
    3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
    0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
    13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 };

// Table - s3
const unsigned char s3[4][16] = {
    10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
    13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
    13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
    1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 };

// Table - s4
const unsigned char s4[4][16] = {
    7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
    13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
    10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
    3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 };

// Table - s5
const unsigned char s5[4][16] = {
    2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
    14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
    4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
    11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 };

// Table - s6
const unsigned char s6[4][16] = {
    12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
    10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
    9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
    4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 };

// Table - s7
const unsigned char s7[4][16] = {
    4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
    13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
    1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
    6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 };

// Table - s8
const unsigned char s8[4][16] = {
    13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
    1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
    7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
    2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 };

// Table - Shift
const unsigned char shift[17] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 0 };

// Table - Binary
const unsigned char binary[64] = {
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1,
    0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1,
    1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
    1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1 };

const unsigned char SubstitutionMatrixIP[64] = {
    58,50,42,34,26,18,10,2,
    60,52,44,36,28,20,12,4,
    62,54,46,38,30,22,14,6,
    64,56,48,40,32,24,16,8,
    57,49,41,33,25,17,9,1,
    59,51,43,35,27,19,11,3,
    61,53,45,37,29,21,13,5,
    63,55,47,39,31,23,15,7
};

const unsigned char SubstitutionInverseMatrixIP[64] = {
    40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,
    38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,
    36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,
    34,2,42,10,50,18,58,26,33,1,41, 9,49,17,57,25
};


const unsigned char SubstitutionMatrixPC1[56] = {
    57,49,41,33,25,17,9,1,
    58,50,42,34,26,18,10,2,
    59,51,43,35,27,19,11,3,
    60,52,44,36,63,55,47,39,
    31,23,15,7,62,54,46,38,
    30,22,14,6,61,53,45,37,
    29,21,13,5,28,20,12,4
};

const unsigned char SubstitutionMatrixPC2[48] = {
    14,17,11,24,1,5,
    3,28,15,6,21,10,
    23,19,12,4,26,8,
    16,7,27,20,13,2,
    41,52,31,37,47,55,
    30,40,51,45,33,48,
    44,49,39,56,34,53,
    46,42,50,36,29,32
};

const unsigned char SubstitutionMatrixE[48] = {
    32,1,2,3,4,5,
    4,5,6,7,8,9,
    8,9,10,11,12,13,
    12,13,14,15,16,17,
    16,17,18,19,20,21,
    20,21,22,23,24,25,
    24,25,26,27,28,29,
    28,29,30,31,32,1
};

const unsigned char SubstitutionMatrixP[32] = {
    16,7,20,21,29,12,28,17,
    1,15,23,26,5,18,31,10,
    2,8,24,14,32,27,3,9,
    19,13,30,6,22,11,4,25
};

void Bit64ToByte64(unsigned char src[8], unsigned char dst[64])
{
    unsigned char *pDst = dst;
    for(int i=0; i<8; i++){
        unsigned char tmp = src[i];
        pDst[0] = BitValH(tmp, 0, 1);
        pDst[1] = BitValH(tmp, 1, 1);
        pDst[2] = BitValH(tmp, 2, 1);
        pDst[3] = BitValH(tmp, 3, 1);
        pDst[4] = BitValH(tmp, 4, 1);
        pDst[5] = BitValH(tmp, 5, 1);
        pDst[6] = BitValH(tmp, 6, 1);
        pDst[7] = BitValH(tmp, 7, 1);
        pDst += 8;
    }
}

void Byte64ToBit64(unsigned char src[64], unsigned char dst[8])
{
    unsigned char *pSrc = src;

    for(int i=0; i<8; i++){
        BitCopy8BitH(pSrc[0], 7, dst[i], 0, 1);
        BitCopy8BitH(pSrc[1], 7, dst[i], 1, 1);
        BitCopy8BitH(pSrc[2], 7, dst[i], 2, 1);
        BitCopy8BitH(pSrc[3], 7, dst[i], 3, 1);
        BitCopy8BitH(pSrc[4], 7, dst[i], 4, 1);
        BitCopy8BitH(pSrc[5], 7, dst[i], 5, 1);
        BitCopy8BitH(pSrc[6], 7, dst[i], 6, 1);
        BitCopy8BitH(pSrc[7], 7, dst[i], 7, 1);
        pSrc += 8;
    }
}

inline void Substitution(int size, const unsigned char matrix[], unsigned char src[], unsigned char dst[])
{
    for(int i=0; i<size; i++){
        int idx = matrix[i] - 1;
        dst[i] = src[idx];
    }
}

inline void RotateShift(unsigned char mode, int size, unsigned char data[], int move)
{
    for(int i=0; i<move; i++){
        if(mode == 1){
            unsigned char tmp = data[0];
            for(int j=0; j<size-1; j++)
                data[j] = data[j+1];
            data[size-1] = tmp;
        }else{
            unsigned char tmp = data[size-1];
            for(int j=size-2; j>=0; j--)
                data[j+1] = data[j];
            data[0] = tmp;
        }
    }
}

inline void S_Substitution(const unsigned char SMatrix[4][16], unsigned char input[6], unsigned char output[4])
{
    int row, col;
    int idx;

    row = (input[0] << 1) + input[5];
    col = (input[1] << 3) + (input[2] << 2) + (input[3] << 1) + input[4];

    idx = SMatrix[row][col] * 4;

    output[0] = binary[idx];
    output[1] = binary[idx+1];
    output[2] = binary[idx+2];
    output[3] = binary[idx+3];
}

void PrintBuf(int num, unsigned char data[])
{
    for(int i=0; i<num; i++)
        printf("%d", (data[i]));
    printf("\n");
}

void CDes(unsigned char key[8], unsigned char data[8], char mode)
{
    unsigned char bufout[64] = {0};
    unsigned char kwork[56] = {0};
    unsigned char worka[48] = {0};
    unsigned char kn[48] = {0};
    unsigned char dBuffer[64] = {0};
    unsigned char inKey[64] = {0};

    /* Convert from 64-bit key into 64-byte key */
    Bit64ToByte64(key, inKey);
//    PrintBuf(sizeof(inKey), inKey);

    /* Convert from 64-bit data into 64-byte data */
    Bit64ToByte64(data, dBuffer);
//    PrintBuf(sizeof(dBuffer), dBuffer);

    /* Initial Permutation of Data */
    Substitution(64, SubstitutionMatrixIP, dBuffer, bufout);
//    PrintBuf(sizeof(bufout), bufout);

    /* Initial Permutation of Key */
    Substitution(56, SubstitutionMatrixPC1, inKey, kwork);
//    PrintBuf(sizeof(kwork), kwork);

    /* 16 Iterations */
    for(int i=0; i<16; i++){
        int move = 0;
        /* Ln */
        for(int j=0; j<32; j++) dBuffer[j] = bufout[32+j];

//        printf("---------------%d------------------\n", i);

        /* Calculation of F(R, K) */
        /* KS Function Begin */
        if(mode == 1){
            move = shift[i];
        }else{
            move = shift[16-i];
        }
        RotateShift(mode, 28, kwork, move);
        RotateShift(mode, 28, kwork+28, move);
        /* Permute kwork - PC2 */
        Substitution(48, SubstitutionMatrixPC2, kwork, kn);
        /* KS Function End */
//        PrintBuf(sizeof(kwork), kwork);
//        PrintBuf(sizeof(kn), kn);

        /* Permute - E */
        Substitution(48, SubstitutionMatrixE, dBuffer, worka);

        /* worka XOR kn */
        for(int j=0; j<48; j++) worka[j] = worka[j] ^ kn[j];

//        PrintBuf(sizeof(worka), worka);

        /* 8 s-functions */
        S_Substitution(s1, worka, kn);
        S_Substitution(s2, worka+6, kn+4);
        S_Substitution(s3, worka+12, kn+8);
        S_Substitution(s4, worka+18, kn+12);
        S_Substitution(s5, worka+24, kn+16);
        S_Substitution(s6, worka+30, kn+20);
        S_Substitution(s7, worka+36, kn+24);
        S_Substitution(s8, worka+42, kn+28);

        /* Permute - P */
        Substitution(32, SubstitutionMatrixP, kn, worka);
//        PrintBuf(sizeof(worka), worka);

        /* bufout XOR worka */
        for(int j=0; j<32; j++) {
            bufout[j+32] = bufout[j] ^ worka[j];
            bufout[j] = dBuffer[j];
        }
    }

    /* Prepare Output */
    for(int i=0; i<32; i++) {
        SwapVal(bufout[i], bufout[32+i]);
    }

    Substitution(64, SubstitutionInverseMatrixIP, bufout, dBuffer);

    memset(data, 0, 8);
    Byte64ToBit64(dBuffer, data);
}



