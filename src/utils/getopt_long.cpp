#include<iostream>
#include <getopt.h>
#include<stdio.h>
using namespace std;
volatile int timerexpired=0;
int speed=0;
int failed=0;
int bytes=0;
/* globals */
int http10=1; /* 0 - http/0.9, 1 - http/1.0, 2 - http/1.1 */
/* Allow: GET, HEAD, OPTIONS, TRACE */
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.5"
int method=METHOD_GET;//全局变量，指定http方法
int clients=1;//并发数
int force=0;//是否等待服务器应答。默认为步等待
int force_reload=0;
int proxyport=80;//代理服务器渡口号。默认为80
char *proxyhost=NULL;//代理服务器地址
int benchtime=30;//运行多久。 默认为30s.可以通过 -t 指定
/* internal */
int mypipe[2];

static void usage(void)
{
   fprintf(stderr,
	"webbench [option]... URL\n"
	"  -f|--force               Don't wait for reply from server.\n"
	"  -r|--reload              Send reload request - Pragma: no-cache.\n"
	"  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
	"  -p|--proxy <server:port> Use proxy server for request.\n"
	"  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"

	"  -1|--http10              Use HTTP/1.0 protocol.\n"
	"  -2|--http11              Use HTTP/1.1 protocol.\n"
	"  --get                    Use GET request method.\n"
	"  --head                   Use HEAD request method.\n"
	"  --options                Use OPTIONS request method.\n"
	"  --trace                  Use TRACE request method.\n"
	"  -?|-h|--help             This information.\n"
	"  -V|--version             Display program version.\n"
	);
};
static const struct option long_options[]=
{
	 {"force",no_argument,&force,1},
	 {"reload",no_argument,&force_reload,1},
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


int main(int argc,char **argv)
{
    int op;
    int options_index=0;
    while(op=getopt_long(argc,argv,"c:t:?",long_options,&options_index)!=-1){
        switch(op)
        {
            case 'c':
                clients=atoi(optarg);cout<<clients<<endl;break;
            case '?':
                usage();break;
            default:
                break;
        }
    }
    if(optind==argc){
         printf("webbench: Missing URL!\n");
         usage();
    }
   cout<<argv[optind]<<endl;
				


    return 0;
}