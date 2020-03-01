#include<signal.h>
#include <unistd.h>
#include<time.h>
// #include"Parser.hpp"
#include<iostream>
class Alarm{
    public:
    static void sig_alarm(int signal)
    {
        Parser::get_instance().isexpired = true;
        // std::cout<<"时间到了"<<std::endl;
    }
    void alarm_handle(int runtime)
    {
         struct sigaction sa;
        sa.sa_handler=sig_alarm;
        sa.sa_flags=0;
        if(sigaction(SIGALRM,&sa,NULL))
        {
            std::cout<<"alarm wrong"<<std::endl;
        }
         alarm(runtime); 
    }
    Alarm(int runtime){
        alarm_handle(runtime);
    }
};