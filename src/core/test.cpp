#include"SocketException.hpp"
#include"Parser.hpp"
#include"HttpBuild.hpp"
#include"ClientSocket.hpp"
#include"ClientSocket.cpp"
#include"Socket.hpp"
#include"Socket.cpp"
int main ( int argc, char** argv )
{
  try
    {
        string url=Parser::get_instance().handle(argc,argv);
        HttpBuild build;

        //"http://www.baidu.com"    
    //  std::cout<<build.GetPort(url)<<std::endl;
      ClientSocket client_socket (build.GetIpByURL(url), build.GetPort(url));
      // ClientSocket client_socket (build.GetIpByURL(url), 443);

      std::string reply;

      try
	{
	  client_socket << build.BuildHttpRequest(url);
	  client_socket >> reply;
	}
      catch ( SocketException& ) {}

      std::cout << "We received this response from the server:\n\"" << reply << "\"\n";

    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}