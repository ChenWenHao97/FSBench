#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
using namespace std;
class BenchResult{
    public:
        int a;
        double b;
        float c;
};
class Universal{
   public:
    Universal(const Universal&)=delete;//删除复制构造函数
    Universal& operator=(const Universal&)=delete;
    static Universal& get_instance(){
        static Universal instance;
        return instance;
    }
     vector<BenchResult> &GetResult() {
         return Result;
     }
    void SumResult(BenchResult tmp){
        lock_guard<mutex> locker{ThreadLock};
        Result.emplace_back(tmp);

    }
    private:
        Universal()=default;
        mutex ThreadLock;
        vector<BenchResult> Result;
};
void func()
{
    BenchResult tmp{1,2.0,3.8};
    Universal::get_instance().SumResult(tmp);//get_instance必须加括号
    cout <<"leaving"<<endl;
}
int main()
{
    vector<shared_ptr<thread> > threads;
    int Clients = 10;
    threads.reserve(Clients);
    for(int i = 0;i < Clients;i++)
    {
        threads.emplace_back(make_shared<thread>(func));
    }
    for(auto &i:threads)
        i->join();


    return 0;
}