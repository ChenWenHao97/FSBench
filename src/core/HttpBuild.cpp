#include<iostream>
#include<vector>
#include<mutex>
#include<string>
#include<getopt.h>
#include<stdio.h>
#include<unistd.h>
#include <regex>
#include <utility>
using namespace std;
class HttpBuild{
    public:
        int port;
        bool isLegalURL(string url)
        {
            string pattern{"(https?)://.*(.cn|.com|.htm|.html|.aspx?|.jsp|.php|.net|/|#)$"};
            regex re(pattern);
           return regex_match(url,re);
        //   正则表达式例子 https://blog.csdn.net/fengbingchun/article/details/54835571
        }
        int GetPort(string url)//解析url里面端口号，默认http为80
        {
            if(!isLegalURL(url))
                return -1;
            //  cout << "url"<<endl;    
            string pattern{"(.*):(.*):([0-9]{0,5})"};
            smatch res;
            regex re(pattern);
           if( regex_search(url,res,re))
              return stoi(res[3]);  
        
              
            return 80;

           
            
        }
        string GetHostByURL(string url)//使用gethostbyname函数得到hostname
        {

        }
        string BuildHttpRquest(string url);
        HttpBuild():port(80){}
};

int main()
{
    HttpBuild build;
   build.GetPort("http://blog.csdn.net:800/fengbingchun/");
   
}