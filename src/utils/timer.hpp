#include<iostream>
#include<chrono>
using namespace std::chrono;
class Timer{
public:
    Timer():m_begin(high_resolution_clock::now()){}
    void reset(){m_begin = high_resolution_clock::now();}
    template<typename Duration=milliseconds>
    int64_t time_()const//微妙
    {
        return duration_cast<Duration>(high_resolution_clock::now()-m_begin).count();
    }
    int64_t time_seconds() const
    {
        return time_<seconds>();
    }
    int64_t time_micro() const
    {
        return time_<microseconds>();
    }
private:
    time_point<high_resolution_clock> m_begin;
};

// void func()
// {
//     cout<<"hello world"<<endl;
// }
