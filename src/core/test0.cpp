#include "../utils/timer.hpp"
#include "ClientSocket.cpp"
#include "ClientSocket.hpp"
#include "HttpBuild.hpp"
// #include "./Parser.hpp"
#include "Socket.cpp"
#include "Socket.hpp"
#include "SocketException.hpp"
#include <thread>
#include "BenchRes.cpp"
using namespace std;
void thread_handle(const string &url, int i) //一个soket一个线程，HTTP/1.0
{
    try
    {
        BenchRes res; //结果
        HttpBuild build;
        Timer connect_;
        ClientSocket client_socket(build.GetIpByURL(url), build.GetPort(url));
        cout << "连接花费时间:" << connect_.time_seconds() << endl;
        cout << "第" << i << "个线程" << endl;
        res.ConnectTime = connect_.time_micro(); //连接时间
        Timer deal_;
        client_socket << build.BuildHttpRequest(url);
        cout << "第" << i << "个线程处理花费时间:" << deal_.time_micro() << endl;
        res.IsFinished = 1;
        res.DealTime = deal_.time_micro();
        // res.TotalConnectNum = 1;  //总共连接数
        // res.TotalHtml = 0;         //总共HTML数

        string reply;
        Timer wait_;
        client_socket >> reply;
        cout << "第" << i << "个 等待花费时间:" << wait_.time_micro() << endl;
        res.WaitTime = wait_.time_micro();
        Parser::get_instance().SumResult(res);
    }catch (SocketException &e)
    {
        BenchRes failed;
        failed.IsFailed = 1;
        cout << "Exception was caught:" << e.description() << endl;
    }
}
int main(int argc, char **argv)
{
    try
    {
        string url = Parser::get_instance().handle(argc, argv);
        vector<thread> thread_list;
        for (int i = 1; i <= Parser::get_instance().clients; i++)
        {
            thread_list.emplace_back(thread_handle, std::cref(url), i);
        }
        for (auto &thread_ : thread_list)
        {
            thread_.join();
        }
    }
    catch (SocketException &e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }

    return 0;
}