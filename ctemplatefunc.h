#ifndef CTEMPLATEFUNC_H
#define CTEMPLATEFUNC_H

#include <vector>
#include <map>
#include <algorithm>
#include <exception>

#include "ccommon.h"

using namespace std;

template<typename T>
int CSort(int len, T *d)
{
    int i, j;
    int isLeft, isRight;

    if(!d) return -1;
    if(len < 2) return 0;
    if(len == 2){
        UpdateMinVal(d[0], d[1]);
        return 0;
    }

    i = 0;
    j = len - 2;
    isLeft = 1;
    isRight = 1;
    while(i<j){
        if(isLeft){
            if(d[i] < d[len-1]){
                i++;
            }else{
                isLeft = 0;
            }
        }
        if(isRight){
            if(d[j] >= d[len-1]){
                j--;
                continue;
            }else{
                isRight = 0;
            }
        }
        if(!isLeft && !isRight){
            SwapVal(d[i], d[j]);
            i++;
            j--;
            isLeft = 1;
            isRight = 1;
        }
    }
//    LogFile("isLeft=%d, isRight=%d, i=%d, j=%d\n", isLeft, isRight, i, j);

    bool isJ=false;
    if(d[j] > d[len-1]){
        SwapVal(d[j], d[len-1]);
        isJ = true;
    }
    if(d[j+1] > d[len-1]){
        SwapVal(d[j+1], d[len-1]);
    }

    if(!isJ) j++;
    if(j > 2){
//        LogFile("\nBefore: len=%d\n", j);
        CSort(j, d);
    }
    if(len-j > 2){
//        LogFile("\nAfter: len=%d\n", len-j);
        CSort(len-j, &d[j]);
    }

    return 0;
}

template<typename T>
bool CSortMin(int len, int *idx,  T *d)
{
    if(!idx || !d) return false;
    if(len < 2) return true;

    for(int i=0; i<len-1; i++){
        for(int j=i+1; j<len; j++){
            if(d[idx[j]] < d[idx[i]])
                SwapVal(idx[j], idx[i]);
        }
    }

    return true;
}

template<typename T>
bool CSortMax(int len, int *idx,  T *d)
{
    if(!idx || !d) return false;
    if(len < 2) return true;

    for(int i=0; i<len-1; i++){
        for(int j=i+1; j<len; j++){
            if(d[idx[j]] > d[idx[i]])
                SwapVal(idx[j], idx[i]);
        }
    }

    return true;
}

template<typename T>
bool DiffVal(vector<T> &data, vector<double> &diff, int step)
{
    int max = data.size() - step;

    if(data.size() <= step) return false;
    if(step < 0 || step >= data.size()) return false;

    diff.clear();
    for(int i=0; i<max; i++){
        double val = data[i+step] - data[i];
        diff.push_back(val);
    }

    return true;
}

template<typename T>
double Mean(vector<T> &data)
{
    double sum = 0;

    try{
        for(int i=0; i<data.size(); i++)
            sum += data[i];
        return sum / data.size();
    }catch(exception e){
        throw e;
    }
}

// 检查线性数据的异常点;
template<typename T>
int CheckLineDataAbnormityPoint(vector<T> &data, vector<int> &point)
{
    vector<double> diff1;
    vector<double> diff2;
    vector<double> diff3;
    double dmean1 = 0;
    double dmean2 = 0;
    vector<bool> mark;
    int startPos = -1;
    map<int, int> sequentialPoints;


    if(data.size() <= 2) return 0;

    // 计算一阶差分;
    DiffVal(data, diff1, 1);

    // 计算平均差值;
    dmean1 = Mean(diff1);
    for(int i=0; i<diff1.size(); i++){
        LogFile("diff1[%d]=%.3f\n", i, diff1[i]);
    }
    LogFile("dmean1=%.3f\n\n", dmean1);

    // 找出差异值与平均差值趋近0的点，并计算平均值;
    for(int i=0; i<diff1.size(); i++){
        double val = fabs(diff1[i] - dmean1);
        diff2.push_back(val);
        LogFile("diff2[%d]=%.3f\n", i, val);
    }
    dmean2 = Mean(diff2);
    LogFile("dmean2=%.3f\n\n", dmean2);

    // 根据dmean2划分规律内外的点;
    mark.resize(data.size(), false);
    for(int i=0; i<data.size(); i++){
        if(i == 0) mark[i] = false;
        if(i > 0 &&  diff2[i-1] < 2*dmean2){
            mark[i] = true;
            diff3.push_back(diff1[i-1]);
        }
        if(mark[i] && startPos < 0) startPos = i;
        LogFile("mark[%d]=%d\n", i, (int)mark[i]);
    }
    dmean1 = Mean(diff3);
    LogFile("dmean1=%.3f\n\n", dmean1);

    // 统计连续识别点，去除少数划分到规律内的错误点;
    bool lastIsMatch = false;
    int lastPoint = 0;
    for(int i=0; i<mark.size(); i++){
        if(lastIsMatch && mark[i]) sequentialPoints[lastPoint]++;
        if(!lastIsMatch && mark[i]){
            lastPoint = i;
            sequentialPoints[i] = 1;
        }
        lastIsMatch = mark[i];
    }
    LogFile("sp.sz=%d\n", sequentialPoints.size());

    // 找到最大连续区域，以此区域向前、向后重新匹配;
    int maxval = 0;
    for(map<int, int>::iterator it=sequentialPoints.begin(); it!=sequentialPoints.end(); it++){
        if(it->second > maxval){
            maxval = it->second;
            startPos = it->first;
        }
    }
    LogFile("startpos=%d, maxval=%d\n", startPos, maxval);

    //线性还原，提取异常点，当前思路有未处理情况，需重新设计算法; 20180903
    double lastVal = data[startPos] - dmean1;
    for(int i=startPos; i<data.size(); i++){
        double val1 = fabs(data[i] - lastVal - dmean1);
        double val2 = fabs(fabs(data[i] - lastVal) - dmean1);
        LogFile("mark[%d]=%d, data=%d, lastval=%.3f, val1=%.3f, val2=%.3f\n", i, (int)mark[i], data[i], lastVal, val1, val2);
        if(val1 < 2*dmean2 || val2 < 2*dmean2){
            mark[i] = true;
            lastVal = data[i];
        }else{
           mark[i] = false;
           lastVal += dmean1;
        }
    }

    lastVal = data[startPos];
    for(int i=startPos-1; i>=0; i--){
        double val1 = fabs(data[i] - lastVal - dmean1);
        double val2 = fabs(fabs(data[i] - lastVal) - dmean1);
        LogFile("mark[%d]=%d, data=%d, lastval=%.3f, val1=%.3f, val2=%.3f\n", i, (int)mark[i], data[i], lastVal, val1, val2);
        if(val1 < 2*dmean2 || val2 < 2*dmean2){
            mark[i] = true;
            lastVal = data[i];
        }else{
            mark[i] = false;
            lastVal -= dmean1;
        }
    }

    for(int i=0; i<mark.size(); i++){
        if(!mark[i]) point.push_back(i);
    }

    return point.size();
}

// 统计单元数据的和;
template<typename T, typename T1>
bool CountCellDataSum(int cellWid, int wid, int hgt, T *data, T1 *sumData)
{
    if(wid <= 0 || hgt <= 0 || cellWid <= 0) {LogPos(1); return false;}
    if(!data || !sumData){LogPos(1); return false;}

    int radius = cellWid / 2;

    for(int i=0; i<hgt; i++){
        for(int j=0; j<wid; j++){
            int idx = i*wid+j;
            if(i==0 || i==hgt-1 || j==0 || j==wid-1){
                sumData[idx] = 0;
            }else{
                int num = 0;
//                printf("i=%d, j=%d\n", i, j);
                for(int m=i-radius; m<=i+radius; m++){
                    for(int n=j-radius; n<=j+radius; n++){
                        int idx1 = m*wid+n;
//                        printf("m=%d, n=%d\n", m, n);
                        num += data[idx1];
                    }
                }
//                printf("num=%d\n", num);
                sumData[idx] = num;
            }
        }
    }

    return true;
}

// 获取概率分布数据;
template<typename T, typename T1>
bool GetProbabilityDistributionData(int cellWid, int wid, int hgt, T *in, int maxVal, T1 *out, double *ratio)
{
    if(wid <= 0 || hgt <= 0 || cellWid <= 0) {LogPos(1); return false;}
    if(!in || !out){LogPos(1); return false;}

    int radius = cellWid / 2;
    int cellNum = cellWid * cellWid;
    vector<T1> sumlist(cellNum, 0);
    vector<int> idxlist(cellNum, 0);
    vector<int> tmplist(cellNum, 0);
    for(int i=0; i<hgt; i++){
        for(int j=0; j<wid; j++){
            int idx = i*wid+j;
            if(i==0 || i==hgt-1 || j==0 || j==wid-1){
                out[idx] = 0;
                ratio != NULL ? ratio[idx] = 0 : 0;
                continue;
            }else{
                double r = 1.0 * in[idx] / maxVal;
                int needCellNum = r * cellNum;
                ratio != NULL ? ratio[idx] = r : 0;
                if(needCellNum <= 0) continue;
//                printf("i=%d, j=%d\n", i, j);
                int k = 0;
                for(int m=i-radius; m<=i+radius; m++){
                    for(int n=j-radius; n<=j+radius; n++){
                        int idx1 = m*wid+n;
                        sumlist[k] = (in[idx1]);
                        idxlist[k] = idx1;
                        tmplist[k] = k;
                        k++;
                    }
                }

                CSortMax(k, tmplist.data(), sumlist.data());
//                PrintDataI(k, 1, tmplist.data());
                for(int m=0; m<needCellNum; m++){
                    int idx1 = tmplist[m];
                    int idx2 = idxlist[idx1];
                    out[idx2] = sumlist[idx1];
                }
            }
        }
    }

    return true;
}

// 最大公约数;
template<typename T>
T GCD(T a, T b)
{
    while(b ^= a ^= b ^= a %= b);
    return a;
}

// 最小公倍数;
template<typename T>
T LCM(T a, int b)
{
    return a * b / GCD(a, b);
}

#endif // CTEMPLATEFUNC_H
