#include "Parser.hpp"
#include "BenchRes.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <mutex>

using std::cout;
using std::endl;

BenchRes::BenchRes() : IsFinished(1), ConnectTime(0), DealTime(0), WaitTime(0), singleTime(0), IsFailed(0),
                       ResponseLength(0), TotalResponse(0), TotalFailed(0), TotalTime(0), TotalFinished(0),
                       PerRequesttime(0), PersecondRequest(0){}

void BenchRes::GetResult(std::vector<BenchRes> &res)
{

    BenchRes total;
    for (auto i : res)
    {
        total.TotalFinished += i.IsFinished;
        total.TotalFailed += i.IsFailed;
        total.TotalTime += i.ConnectTime + i.DealTime + i.WaitTime;
        total.TotalResponse += i.ResponseLength;
    }
    total.PersecondRequest = (total.TotalFailed + total.TotalFinished) / total.TotalTime;
    total.PerRequesttime = total.TotalTime / (total.TotalFailed + total.TotalFinished);

    std::cout << "测试花费总时间:  " << total.TotalTime << std::endl;

    std::cout << "总请求数:        " << Parser::get_instance().clients << std::endl;
    std::cout << "完成请求数：     " << Parser::get_instance().clients - total.TotalFailed << endl;
    std::cout << "失败请求数：     " << total.TotalFailed << endl;
    std::cout << "响应数据长度总和:" << total.TotalResponse << endl;
    std::cout << "传输速率:        " << total.TotalResponse % total.TotalTime << " bytes/ms" << endl;
    std::cout << endl;

    //连接时间从小到大排序
    cout << "连接时间(ms)" << endl;
    cout << "     最小            "
         << "中等            "
         << "最大时间            "
         << "平均时间            " << endl;
    std::sort(res.begin(), res.end(), [](BenchRes a, BenchRes b) { return a.ConnectTime < b.ConnectTime; });
    cout << "连接:" << res[0].ConnectTime << "            "
         << res[(total.TotalFailed + total.TotalFinished) / 2].ConnectTime << "            "
         << res[res.size() - 1].ConnectTime << "            ";
    double connectaverage = 0;
    for (auto i : res)
    {
        connectaverage += i.ConnectTime;
    }
    cout << connectaverage / res.size() << endl;
    std::sort(res.begin(), res.end(), [](BenchRes a, BenchRes b) { return a.DealTime < b.DealTime; });
    cout << "处理:" << res[0].DealTime << "            "
         << res[(total.TotalFailed + total.TotalFinished) / 2].DealTime << "            "
         << res[res.size() - 1].DealTime << "            ";
    double dealaverage = 0;
    for (auto i : res)
    {
        dealaverage += i.DealTime;
    }
    cout << dealaverage / res.size() << endl;
    std::sort(res.begin(), res.end(), [](BenchRes a, BenchRes b) { return a.WaitTime < b.WaitTime; });
    cout << "等待:" << res[0].WaitTime << "               "
         << res[(total.TotalFailed + total.TotalFinished) / 2].WaitTime << "                  "
         << res[res.size() - 1].DealTime << "        ";
    double waitaverage = 0;
    for (auto i : res)
    {
        waitaverage += i.WaitTime;
    }
    cout << waitaverage / res.size() << endl;

    for (auto &i : res) //单个http请求的总时间,改变i值，得用引用
    {
        i.singleTime = i.ConnectTime + i.DealTime + i.WaitTime;
    }
    std::sort(res.begin(), res.end(), [](BenchRes a, BenchRes b) { return a.singleTime < b.singleTime; });

    cout << "所有:" << res[0].singleTime << "           "
         << res[(total.TotalFailed + total.TotalFinished) / 2].singleTime << "            "
         << res[res.size() - 1].singleTime << "            ";
    double singleaverage = 0;
    for (auto i : res)
    {
        singleaverage += i.singleTime;
    }
    cout << singleaverage / res.size() << endl;
    cout << endl;

    cout << "请求处理时间分布(ms)" << endl; //有问题
    cout << "50%      " << res[(total.TotalFailed + total.TotalFinished) / 2 - 1].singleTime << endl;
    cout << "60%      " << res[(total.TotalFailed + total.TotalFinished) * 0.6 - 1].singleTime << endl;
    cout << "70%      " << res[(total.TotalFailed + total.TotalFinished) * 0.7 - 1].singleTime << endl;
    ;
    cout << "80%      " << res[(total.TotalFailed + total.TotalFinished) * 0.8 - 1].singleTime << endl;
    cout << "90%      " << res[(total.TotalFailed + total.TotalFinished) * 0.9 - 1].singleTime << endl;
    cout << "100%     " << res[(total.TotalFailed + total.TotalFinished - 1)].singleTime << endl;
}
