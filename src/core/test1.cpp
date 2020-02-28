#include "../utils/timer.hpp"
#include "ClientSocket.cpp"
#include "ClientSocket.hpp"
#include "HttpBuild.hpp"
// #include "./Parser.hpp"
#include "Socket.cpp"
#include "Socket.hpp"
#include "SocketException.hpp"
#include <thread>
#include <memory>
using namespace std;

void thread_handle(const string &url, const string &request_string, int request_port, int i)
{
  try
  {   
    Timer connect_timepoint;
    ClientSocket client_socket { url, request_port };
    if (Parser::get_instance().force == 1) {
      client_socket.Set_Non_Blocking(true);
    }

    Timer deal_;
    client_socket << request_string;
    cout << "第" << i << "个线程处理花费时间:" << deal_.time_micro() << endl;
    BenchRes res;
    res.IsFinished = 1;
    res.DealTime = deal_.time_micro();
    res.IsFailed = 0;          //连接失败数
    res.PercecondRequests = 0; //美秒连接数
    string reply;
    Timer wait_;
    client_socket >> reply;
    cout << "reply:" << reply << endl;
    cout << "第" << i << "个 等待花费时间:" << wait_.time_micro() << endl;
    Parser::get_instance().SumResult(res);
    cout << "!!!线程" << i << "退出!!!" << endl;
  }
  catch (SocketException &e)
  {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }
}
int main(int argc, char **argv)
{
  try
  {
    string url = Parser::get_instance().handle(argc, argv);

    HttpBuild builder;
    std::string request_string = builder.BuildHttpRequest(url);
    int request_port = builder.GetPort(url);

    // cout << "连接花费时间:" << connect_.time_seconds() << endl;

    std::vector<std::thread> thread_list;
    for (int i = 0; i < Parser::get_instance().clients; i++)
    {
      thread_list.emplace_back(
          thread_handle, std::cref(url), std::cref(request_string), request_port, i);
    }

    for (auto &i : thread_list)
    {
      i.join();
    }
  }
  catch (SocketException &e)
  {
    std::cout << "Exception was caught:" << e.description() << "\n";
  }

  return 0;
}