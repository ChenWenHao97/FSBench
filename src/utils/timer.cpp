#include<iostream>
#include<chrono>
using namespace std::chrono;
using namespace std;
class Timer{
    public:
        Timer():m_begin(high_resolution_clock::now()){}
        void reset(){m_begin = high_resolution_clock::now();}
        template<typename Duration=milliseconds>
        int64_t elapsed()const//微妙
        {
            return duration_cast<Duration>(high_resolution_clock::now()-m_begin).count();
        }
        int64_t elapsed_seconds() const
        {
            return elapsed<seconds>();
        }
        int64_t elapsed_micro() const
        {
            return elapsed<microseconds>();
        }
    private:
        time_point<high_resolution_clock> m_begin;
};
void func()
{
    cout<<"hello world"<<endl;
}
int main()
{
    Timer t;
    func();
    cout<<t.elapsed_micro()<<endl;
}