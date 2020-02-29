#include <iostream>
#include <getopt.h>
#include <mutex>
#include <string>
#include <unistd.h>
#include <vector>
#include<algorithm>
using namespace std;
class BenchRes
{
    public:
    //每个线程拥有
    int IsFinished;
    int64_t ConnectTime; //连接时间
    int64_t DealTime;    //处理时间
    int64_t WaitTime;    //等待时间
    int64_t singleTime;  //单个总时间
    int IsFailed;       //连接失败数
    int ResponseLength;  //每个回应

    //最终结果
    int TotalFinished;
    int64_t TotalTime;
    int TotalFailed;
    double PersecondRequest; //每秒连接数
    double PerRequesttime;//每个连接秒数
    int TotalResponse;
    double  Transferedrate;//传输速率;

    BenchRes() : IsFinished(1), ConnectTime(0), DealTime(0), WaitTime(0), singleTime(0),IsFailed(0),
                ResponseLength(0),TotalResponse(0)
    {
    }
    void GetResult(vector<BenchRes>& res)
    {
        BenchRes total;
        for(auto i: res)
        {
            total.TotalFinished+=i.IsFinished;
            total.TotalFailed+=i.IsFailed;
            total.TotalTime+=i.ConnectTime+i.DealTime+i.WaitTime;
            total.Transferedrate+=i.ResponseLength;
        }
        total.PersecondRequest =  (total.TotalFailed+total.TotalFinished)/total.TotalTime;
        total.PerRequesttime = total.TotalTime/(total.TotalFailed+total.TotalFinished);
        total.Transferedrate /= total.TotalTime;

        cout<<"测试花费总时间:"<<total.TotalTime<<endl;
        cout<<"总请求数:     "<<total.TotalFailed+total.TotalFinished<<endl;
        cout<<"完成请求数：   "<<total.TotalFinished<<endl;
        cout<<"失败请求数：   "<<total.TotalFailed<<endl;
        cout<<"响应数据长度总和:"<<total.TotalResponse<<endl;
        cout<<"传输速率:      "<<total.Transferedrate<<endl;
        cout<<endl;




        for(auto i:res)//单个http请求的总时间
        {
            i.singleTime = i.ConnectTime+i.DealTime+i.WaitTime;
        }
        //连接时间从小到大排序
        
        cout<<"连 接 时 间(ms)"<<endl;
                    cout<<"最小"    <<"中等"<<"最大时间"<<"平均时间"<<endl;
        sort(res.begin(),res.end(),[](BenchRes a,BenchRes b){return a.ConnectTime<b.ConnectTime;});
        cout<<"连接:"<<res[0].ConnectTime<<" "
                    <<res[(total.TotalFailed+total.TotalFinished)/2].ConnectTime<<" "
                    <<res[res.size()-1].ConnectTime<<" ";
                    double connectaverage = 0;
                    for(auto i:res)
                    {
                        connectaverage+=i.ConnectTime;
                    }
                    cout<<connectaverage/res.size()<<endl;
        sort(res.begin(),res.end(),[](BenchRes a,BenchRes b){return a.DealTime<b.DealTime;});
        cout<<"处理:"<<res[0].DealTime<<" "
                    <<res[(total.TotalFailed+total.TotalFinished)/2].DealTime<<" "
                    <<res[res.size()-1].DealTime<<" ";
                    double dealaverage = 0;
                    for(auto i:res)
                    {
                        dealaverage+=i.DealTime;
                    }
                    cout<<dealaverage/res.size()<<endl;
        sort(res.begin(),res.end(),[](BenchRes a,BenchRes b){return a.WaitTime<b.WaitTime;});
        cout<<"等待:"<<res[0].WaitTime<<" "
                    <<res[(total.TotalFailed+total.TotalFinished)/2].WaitTime<<" "
                    <<res[res.size()-1].DealTime<<" ";
                    double waitaverage = 0;
                    for(auto i:res)
                    {
                        waitaverage+=i.WaitTime;
                    }
                    cout<<waitaverage/res.size()<<endl;
        sort(res.begin(),res.end(),[](BenchRes a,BenchRes b){return a.singleTime<b.singleTime;});

        cout<<"所有连接"<<res[0].singleTime<<" "
                    <<res[(total.TotalFailed+total.TotalFinished)/2].singleTime<<" "
                    <<res[res.size()-1].singleTime<<" ";
                    double singleaverage = 0;
                    for(auto i:res)
                    {
                        singleaverage+=i.singleTime;
                    }
                    cout<<singleaverage/res.size()<<endl;
                    cout<<endl;





        cout<<"请求处理时间分布(ms)"<<endl;//有问题
        cout<<"50%      "<<res[(total.TotalFailed+total.TotalFinished)/2].singleTime<<endl;
        cout<<"60%      "<<res[(total.TotalFailed+total.TotalFinished)*0.6].singleTime<<endl;
        cout<<"70%      "<<res[(total.TotalFailed+total.TotalFinished)*0.7].singleTime<<endl;;
        cout<<"80%      "<<res[(total.TotalFailed+total.TotalFinished)*0.8].singleTime<<endl;
        cout<<"90%      "<<res[(total.TotalFailed+total.TotalFinished)*0.9].singleTime<<endl;
        cout<<"100%     "<<res[(total.TotalFailed+total.TotalFinished)].singleTime<<endl;
    }
};
