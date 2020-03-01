#include "../utils/timer.hpp"
#include "ClientSocket.hpp"
#include "../http/HttpBuild.hpp"
#include"Alarm.hpp"
// #include "Socket.hpp"
// #include "SocketException.hpp"
#include <thread>

using namespace std;
void thread_handle(const string url, int i) //一个soket一个线程
{
    try
    {
       
        BenchRes res; //结果
        HttpBuild build;
        Timer connect_;
        ClientSocket client_socket(build.GetIpByURL(url), build.GetPort(url));
        res.ConnectTime = connect_.time_micro(); //连接时间

        Timer deal_;
        client_socket << build.BuildHttpRequest(url);
        res.IsFinished = 1;
        res.DealTime = deal_.time_micro(); //处理时间

        string reply;
        Timer wait_;
        client_socket >> reply;
        res.ResponseLength = reply.size();

        res.WaitTime = wait_.time_micro(); //等待时间
        Parser::get_instance().SumResult(res);
        // cout<<"thread::"<<i<<endl;
    }
    catch (SocketException &e)
    {
        BenchRes failed;
        failed.IsFailed = 1;
        Parser::get_instance().SumResult(failed);
        cout << "Exception was caught:" << e.description() << endl;
    }
}
int main(int argc, char **argv)
{
    try
    {   cout<<"正在解析中，请稍后!"<<endl;
        string url = Parser::get_instance().handle(argc, argv);
        Alarm setruntime(Parser::get_instance().runtime);
        vector<thread> thread_list;
        thread_list.reserve(Parser::get_instance().clients);
        int dealclient;
        for (int i = 1; i <= Parser::get_instance().clients; i++)
        {
            if(Parser::get_instance().isexpired)
            {
                // cout<<"从for循环退出"<<endl;
                dealclient = i;
                cout<<"!!!!!!!!!!!!!!!dealclient:!!!!!!!!!"<<dealclient<<endl;

                break;
            }
            thread tmp(thread_handle, url, i);
            if(tmp.joinable())
                thread_list.emplace_back(move(tmp));
            // thread_list.emplace_back(thread_handle, std::cref(url), i);
        }
        int i = 0;
        for (auto &thread_:thread_list)//几下线程数，防止一直在等待join,陷入死循环
        {

            thread_.join();
            cout<<"join:"<<++i<<endl;

        }
        HttpBuild build;
        cout << "服务器hostname:" << build.GetIpByURL(url) << endl;
        cout << "端口号:        " << build.GetPort(url) << endl;
        cout << "访问路径:      " << build.GetCatalogue(url) << endl;
        cout << endl;
        BenchRes finalresult;
        vector<BenchRes> res = Parser::get_instance().GetParserResult();
        // cout<<"结尾处"<<endl;
        // finalresult.GetResult(res);
    }
    catch (SocketException &e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }

    return 0;
}