#pragma once
#include <getopt.h>
#include <unistd.h>
#include <vector>

// #include "Parser.hpp"
class BenchRes
{
public:
    BenchRes();
    void GetResult(std::vector<BenchRes> &res);

    //每个线程拥有
    int IsFinished;
    int64_t ConnectTime; //连接时间
    int64_t DealTime;    //处理时间
    int64_t WaitTime;    //等待时间
    int64_t singleTime;  //单个总时间
    int IsFailed;        //连接失败数
    int ResponseLength;  //每个回应

    //最终结果
    int TotalFinished;
    int64_t TotalTime;
    int TotalFailed;
    double PersecondRequest; //每秒连接数
    double PerRequesttime;   //每个连接秒数
    int TotalResponse;

};
