#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<future>
#include"timer.hpp"
using namespace std;
vector<thread> thread_list;

class Singleton
{
public:
    ~Singleton(){
        std::cout<<"destructor called!"<<std::endl;
    }
    
    Singleton(const Singleton&)=delete;//删除复制构造函数
    Singleton& operator=(const Singleton&)=delete;
    static Singleton& get_instance(){
        static Singleton instance;
        return instance;
    }
    vector<int> res;
    void add(vector<int> &tmp){
       mutex thread_lock;
       lock_guard<mutex> locker(thread_lock);
        for(int i = 0;i < tmp.size();i++){
            res[i] = res[i]+tmp[i];
        }

    }
   
private:
    Singleton():res(10){//构造函数初始华
        std::cout<<"constructor called!"<<std::endl;
    }
    
     
};

void func(Timer& tmp)
{
    // vector<int> tmp{1,2,3,4,5,6};
    //  Singleton& ins = Singleton::get_instance();
    // ins.add(tmp);
    cout <<"leaving"<<endl;
    cout <<tmp.time_micro()<<endl;
}

int main()
{
    int client;
    cin>>client;
    for(int i = 0;i < client;i++)
    {
        Timer deal_;
        thread tmp{func,ref(deal_)};
        thread_list.push_back(move(tmp));
    }
    for(auto& thread_ : thread_list)
    {
        thread_.join();
    }
}