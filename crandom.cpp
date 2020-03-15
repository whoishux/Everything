
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#include "crandom.h"

#if 1

#define m 0x100000000LL
#define c 0xB16
#define a 0x5DEECE66DLL

static unsigned long long seed = 1;

double drand48(void)
{
    seed = (a * seed + c) & 0xFFFFFFFFFFFFLL;
    unsigned int x = seed >> 16;
    return  ((double)x / (double)m);

}

void srand48(unsigned int i)
{
    seed  = (((long long int)i) << 16) | rand();
}
#endif

void crand_init()
{

//#ifndef __GNUC__
    srand((unsigned)time(NULL));
//#else
//    srand48( time(NULL) );
//#endif

}

void crand_init_seed(int seed)
{

#ifndef __GNUC__
    srand((unsigned int)seed);
#else
    srand48( seed );
#endif

}

int crand()
{
    static int is_inited=0;
    if(!is_inited){
        crand_init();
        is_inited = 1;
    }
    return rand();
}

int crand2(int seed)
{
    static int is_inited=0;
    if(!is_inited){
        crand_init_seed(seed);
        is_inited = 1;
    }
    return rand();
}

// [0, 1]
double cdrand()
{
    double r=0.0;
#if 1
    static int is_inited=0;
    if(!is_inited){
        crand_init();
        is_inited = 1;
    }
#else
    crand_init();
#endif

#ifndef __GNUC__
    r = rand();
    r = r / RAND_MAX;
#else
    r = drand48();
#endif

    return r;
}

/*
 *���ȷֲ�����������Ȼ�
*/
double Uniform(double min, double max, long int *seed)
{
    double t = 0;
    *seed = 2045 * (*seed) + 1;
    *seed = *seed - (*seed / 1048576) * 1048576;
    t = (*seed) / 1048576.0;
    t = min + (max - min) * t;
    return t;
}


/* �������ȷֲ������, [min, max] */
double CAverageRandom(double min,double max)
{
    double r=0.0;
    if(min>=max){
        fprintf(stderr, "K2_AverageRandom(min, max) and min<max \n");
        return 0;
    }
#if 0
    r = min + (max - min)*cdrand();
#else
    int minInteger = (int)(min * 10000);
    int maxInteger = (int)(max * 10000);
    int randInteger = crand() * crand();
    int diffInteger = maxInteger - minInteger;
    int resultInteger = randInteger % diffInteger + minInteger;
    r = (resultInteger/10000.0);
#endif
    return r;
}

// [min, max)
int CAverageRandomInt(int min, int max)
{
    if(min > max) return 0;
    if(min == max) return min;

    return int(CAverageRandom(min, max));
}

/* ����˵����double miu���̣���̬��������ѧ����

                    double sigma���ң���̬�����ľ�����

                    double min,double max������������������ķ�Χ

    �����Ϸ�����ȡ ��=0����=0.2����Χ��-1~1����400����̬�����
 */


#define PI 3.141592653

/* �����ܶȺ��� */
double K2_Normal(double x,double miu,double sigma)
{
    if(sigma>0){
        return 1.0/sqrt(2*PI*sigma) * exp(-1*(x-miu)*(x-miu)/(2*sigma*sigma));
    }else{
        fprintf(stderr, "K2_Normal! sigma>0 \n");
        return 0;
    }
}

/* ������̬�ֲ������ */
double CNormalRandom(double miu, double sigma,double min,double max)
{
    double x;
    double dScope;
    double y;
    do
    {
    x = CAverageRandom(min,max);
    y = K2_Normal(x, miu, sigma);
    dScope = CAverageRandom(0, K2_Normal(miu,miu,sigma));
    }while( dScope > y);
    return x;
}

/*  ������miuΪ�м�ֵ��offset��������̬�ֲ������
    miu:�м�ֵ offset:����ֵ */
double CNormRand(double miu, double offset)
{
    double sgm;
    double boundary;

    sgm = offset/3.0;
//    boundary = offset+1;
    boundary = offset;

    return CNormalRandom(miu, sgm, miu-boundary, miu+boundary);
}
