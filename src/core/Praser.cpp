#include<iostream>
#include<vector>
#include<mutex>
#include<string>
#include<getopt.h>
#include<stdio.h>
#include<unistd.h>
using namespace std;
constexpr int METHOD_GET=0;
constexpr int METHOD_HEAD=1;
constexpr int METHOD_OPTIONS=2;
constexpr int METHOD_TRACE=3;
constexpr const char* PROGRAM_VERSION="1.5";
class BenchRes{
    public:
        string hostname;
        int port;
        int finished;
        int failed;//连接失败数
        int TotalnConnectNum;//总共连接数
        int TotalHtml;//总共HTML数
        int percecondRequests;//美秒连接数
};
class Parser{
    public:
        Parser(const Parser&)=delete;
        Parser& operator=(const Parser&)=delete;
        static Parser& get_instance(void)
        {
            static Parser instance;
            return instance;
        }
        void SumResult(BenchRes tmp)
        {
            lock_guard<mutex> locker(ThreadLock);
            Result.emplace_back(tmp);
        }
        int force;
        int reload;
        int runtime;//second
        string proxy;//代理
        int clients;//连接数
        bool http10;
        bool http11;
        int method;
        bool help;
        bool version;
        int op;
        int options_index=0;
        string url;
        static void usage(void);
        const struct option long_options[14]={
            {"force",no_argument,&force,1},
	        {"reload",no_argument,&reload,1},
	        {"time",required_argument,NULL,'t'},
	        {"proxy",required_argument,NULL,'p'},
	        {"clients",required_argument,NULL,'c'},
	        {"http10",no_argument,NULL,'1'},
	        {"http11",no_argument,NULL,'2'},
	        {"get",no_argument,&method,METHOD_GET},
	        {"head",no_argument,&method,METHOD_HEAD},
	        {"options",no_argument,&method,METHOD_OPTIONS},
	        {"trace",no_argument,&method,METHOD_TRACE},
            {"help",no_argument,NULL,'?'},
	        {"version",no_argument,NULL,'V'},
	        {NULL,0,NULL,0}
        };
        string handle(int argc,char **argv);
    private:
        Parser():force(0),reload(0),runtime(30),clients(1){}
        vector<BenchRes> Result;
        mutex ThreadLock;
        
};
void Parser::usage(void)
{
     fprintf(stderr,
	"webbench [option]... URL\n"
	"  -f|--force               Don't wait for reply from server.\n"
	"  -r|--reload              Send reload request - Pragma: no-cache.\n"  
	"  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
	"  -p|--proxy <server:port> Use proxy server for request.\n"
	"  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
	"  -0|--http10              Use HTTP/1.0 protocol.\n"
	"  -1|--http11              Use HTTP/1.1 protocol.\n"
	"  --get                    Use GET request method.\n"
	"  --head                   Use HEAD request method.\n"
	"  --options                Use OPTIONS request method.\n"
	"  --trace                  Use TRACE request method.\n"
	"  -?|-h|--help             This information.\n"
	"  -V|--version             Display program version.\n"
	);
}
string Parser::handle(int argc,char **argv)
{
    int op;
    int options_index;
    while((op=getopt_long(argc,argv,"frt:p:c:12?v",long_options,&options_index))!=-1)
    {
        switch(op)
        {
            
            case 'f':
                force = 1;break;
            case 'r':
                reload = 1;break;
            case 't':
                runtime = atoi(optarg);break;
            case 'p': break;

            case 'c':
                clients=atoi(optarg);;break;
            case 0:
                http10=true,http11 = false;break;
            case 1:
                http10 = false,http11=true;break;
            case '?':
                usage();break;
            default:
                cout << "输入错误!"<<endl;
                usage();
                break;
        }
    }
    if(optind==argc)
    {
         cout<<"没有URL!"<<endl;
         usage();
         return nullptr;
    }
    switch(method)
    {
        case METHOD_GET:cout<<"GET ";break;
        case METHOD_HEAD:cout<<"HEAD ";break;
        case METHOD_OPTIONS:cout<<"OPTIONS ";break;
        case METHOD_TRACE:cout<<"TRACE" ;break;

    }
    cout<<argv[optind]<<endl;
    return argv[optind];
				
}
// int main(int argc,char **argv)
// {
//    string URL= Parser::get_instance().handle(argc,argv);
//    cout <<"uRL"+URL<<endl;
// }