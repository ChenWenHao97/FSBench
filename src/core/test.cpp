#include "../utils/timer.hpp"
#include "ClientSocket.cpp"
#include "ClientSocket.hpp"
#include "HttpBuild.hpp"
// #include "./Parser.hpp"
#include "Socket.cpp"
#include "Socket.hpp"
#include "SocketException.hpp"
#include <thread>
using namespace std;
void thread_handle(string &url, int i)//一个soket一个线程，HTTP/1.0
{
    HttpBuild build;
    Timer connect_;
    ClientSocket client_socket(build.GetIpByURL(url), build.GetPort(url));

    cout << "第" << i << "个线程" << endl;
    Timer deal_;
    client_socket << build.BuildHttpRequest(url);
    cout << "第" << i << "个线程处理花费时间:" << deal_.time_micro() << endl;
    BenchRes res;
    res.finished = 1;
    res.dealTime = deal_.time_micro();
    res.failed = 0;            //连接失败数
    res.TotalnConnectNum = 1;  //总共连接数
    res.TotalHtml = 0;         //总共HTML数
    res.percecondRequests = 0; //美秒连接数
    string reply;
    Timer wait_;
    client_socket >> reply;
    cout << "第" << i << "个 等待花费时间:" << wait_.time_micro() << endl;
    Parser::get_instance().SumResult(res);
}
int main(int argc, char **argv)
{
    try
    {
        string url = Parser::get_instance().handle(argc, argv);

        // cout << "连接花费时间:" << connect_.time_seconds() << endl;
        vector<thread> thread_list;
        for (int i = 1; i <= Parser::get_instance().clients; i++)
        {
            thread tmp(thread_handle, ref(url), i);
            thread_list.push_back(move(tmp)); //不能复制thread类对象
        }
        for (auto &thread_ : thread_list)
        {
            thread_.join();
        }
    }catch (SocketException &e)
    {
        std::cout << "Exception was caught:" << e.description() << "\n";
    }

    return 0;
}