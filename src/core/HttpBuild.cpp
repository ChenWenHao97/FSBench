#include<iostream>
#include<vector>
#include<mutex>
#include<string>
#include<getopt.h>
#include<stdio.h>
#include<unistd.h>
#include <regex>
#include <utility>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
            return 80;//默认端口号
            
        }
        string GetHostByURL(string url)
        {
            if(!isLegalURL(url))
                return nullptr;
            string pattern{"(http?)://(.*)/"};
            smatch res;
            regex re(pattern);
            if(regex_search(url,res,re))
             return res[2];
        }
        string GetIpByURL(string url)//使用gethostbyname函数得到hostname
        {
            const char *name =GetHostByURL(url).c_str();
            struct hostent *hptr;
 
            hptr = gethostbyname(name);
            if (hptr == NULL) 
            {
                 cout<<"gethostbyname error for host:"<<name<<":"<<hstrerror(h_errno)<<endl;
                 return nullptr;
            }  

            //输出主机的规范名
            // printf("\tofficial: %s\n", hptr->h_name);
         
            //输出主机的别名
            char **pptr;
            char str[INET_ADDRSTRLEN];
            // for (pptr=hptr->h_aliases; *pptr!=NULL; pptr++) {
            //     printf("\ttalias: %s\n", *pptr);
            // }
         
            //输出ip地址
            switch (hptr->h_addrtype) 
            {
                case AF_INET:
                    pptr = hptr->h_addr_list;
                    // for (; *pptr!=NULL; pptr++) 
                    // {
                    //     printf("\taddress: %s\n",
                    //             inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));
                    // }
                    return inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
                    break;
                default:
                    cout<<"unknown address type"<<endl;
                    break;
            }
        
                    return nullptr;
        }
             string BuildHttpRquest(string url)
        {

        }
        HttpBuild():port(80){}
};

int main()
{
    HttpBuild build;
   build.GetIpByURL("http://www.baidu.com/");
   
}