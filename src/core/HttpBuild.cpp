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
#include"Parser.hpp"
using namespace std;
class HttpBuild{
    public:
        int port;
        bool isLegalURL(string url)
        {
            if(url.size()>1500)
            {
                cout <<"url长度过长"<<endl;
                return  false;
            }
            string pattern{"(https?)://.*(.cn|.com|.htm|.html|.aspx?|.jsp|.php|.net||#)$"};
            regex re(pattern);
            // cout<<"isLegal"<<endl;
            //  cout<<regex_match(url,re)<<endl;
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
                return "";
            string pattern1{"(http?)://(.*)/"};
            string pattern2{"(http?)://(.*)"};
            smatch res;
            regex re1(pattern1);
            regex re2(pattern2);
            if(regex_search(url,res,re1))
            {
                cout<<res[2]<<endl;
                return res[2];
            }
            else if(regex_search(url,res,re2))
            {
                 cout<<res[2]<<endl;
                return res[2];
            }
        }
        string GetIpByURL(string url)//使用gethostbyname函数得到hostname
        {
            const char *name =GetHostByURL(url).c_str();
            struct hostent *hptr;
 
            hptr = gethostbyname(name);
            if (hptr == NULL) 
            {
                 cout<<"gethostbyname error for host:"<<name<<":"<<hstrerror(h_errno)<<endl;
                 return "";
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
        
                    return "";
        }
        string GetCatalogue(string url)
        {
             if(!isLegalURL(url))
                return "";
            // cout << "url22"<<endl;    
            string pattern{"(https?)://.*(.cn|.com|.htm|.html|.aspx?|.jsp|.php|.net)/(.*)"};
            smatch res;
            regex re(pattern);
            //  cout << "url333"<<endl;    
        
           if(regex_search(url,res,re))
           {
               string str(res[3]);
               return "/"+str;
           }
            return "/";//默认访问根
        }
        string BuildHttpRquest(string url)
        {
            string HttpRquest="";
            string catalogue = GetCatalogue(url);

            //  cout<<"buildHTTp"<<endl;

            string Referer = "Referer:"+url+"\n";
            string AcceptLanguage="Accept-Language: zh-cn\n";
            string AcceptCharst = "Accept-Charset: GB2312,utf-8;q=0.7,*;q=0.7\n";
            string AcceptEncoding="Accept-Encoding: gzip, deflate\n";
            string UserAgent = "Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727; TheWorld)\n";
            //  cout<<"buildHTTp"<<endl;

            string Host = GetHostByURL(url);
            string Longcon = "Connection: Keep-Alive\n";

            //  cout<<"buildHTTp"<<endl;


            int port_=GetPort(url);
            // string Shortcon = 
            bool http10 = Parser::get_instance().http10;
            //  cout<<"buildHTTp3"<<endl;

            bool http11 = Parser::get_instance().http11;

            switch(Parser::get_instance().method)
            {
               case METHOD_GET: HttpRquest =HttpRquest+"GET ";break;
	           case METHOD_HEAD:HttpRquest =HttpRquest+"HEAD ";break;
	           case METHOD_OPTIONS: HttpRquest =HttpRquest+"OPTIONS ";break;
	           case METHOD_TRACE: HttpRquest =HttpRquest+"TRACE ";break;
            }
            // cout<<"buildHTTp"<<endl;
            if(http10)
            {
                HttpRquest +="HTTP/1.0\n";
                HttpRquest+=Referer;
                HttpRquest+="Host:"+Host;
                // +to_string(port_);
                // cout<<"buildHTTp"<<endl;

                HttpRquest+=UserAgent;
                HttpRquest+=AcceptLanguage;
                HttpRquest+=AcceptEncoding;
                HttpRquest+=AcceptCharst;
                HttpRquest+=Longcon;


                
                cout<<HttpRquest<<endl;
            }
            else if(http11)
            {
                HttpRquest = HttpRquest+catalogue+"HTTP/1.1";
                cout<<HttpRquest<<endl;
            }
            return HttpRquest;
        }
        HttpBuild():port(80){}
};

int main(int argc,char**argv)
{
    string url=Parser::get_instance().handle(argc,argv);
    HttpBuild build;
    build.BuildHttpRquest(url);
//   "http://www.baidu.com"
   
}