#pragma once
#include <getopt.h>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "BenchRes.hpp"

constexpr int METHOD_GET = 0;
constexpr int METHOD_HEAD = 1;
constexpr int METHOD_OPTIONS = 2;
constexpr int METHOD_TRACE = 3;
constexpr const char *PROGRAM_VERSION = "1.5";

class Parser
{
public:
    static Parser &get_instance(void)
    {
        static Parser instance;
        return instance;
    }
    void SumResult(BenchRes tmp)
    {
        std::lock_guard<std::mutex> locker(ThreadLock);
        Result.emplace_back(tmp);
    }
    std::vector<BenchRes> GetParserResult(void)
    {
        return Result;
    }
    int force;
    int reload;
    int runtime;             // second
    volatile bool isexpired; //是否超时，加了volatile防止多线程出现问题???
    std::string ProxyServer; //代理
    int clients;             //连接数
    bool http10;             //默认http10,GET\POST\DELETE
    bool http11;             // OPTIONS\PUT\DELETE,\TRACE\CONNECT
    int method;
    std::string proxy;
    bool help;
    bool version;
    int op;
    // int options_index = 0;
    std::string url;
    static void usage(void)
    {
        {
            fprintf(
                stderr,
                "fsbench [option]... URL\n"
                "  -f|--force               不等待服务器响应直接返回\n"

                "  -t|--time <sec>          设置运行fsbench的时间，默认30秒\n "
                "  -p|--proxy <server:port> 使用代理服务器\n"
                "  -c|--clients <n>         运行客户端数量，默认1个"
                "  -1|--http10              使用 HTTP/1.0 协议\n"
                "  -2|--http11              使用 HTTP/1.1 协议\n"
                "  --get                    使用 GET 方法\n"
                "  --head                   使用 HEAD 方法 \n"
                "  --options                使用 OPTIONS 方法\n"
                "  --trace                  使用 TRACE 方法\n"
                "  -?|-h|--help             显示帮助信息\n"
                "  -V|--version             显示程序版本\n");
        }
    }
    const struct option long_options[14] = {
        {"force", no_argument, &force, 'f'},
        {"time", required_argument, NULL, 't'},
        {"proxy", required_argument, NULL, 'p'},//未实现
        {"clients", required_argument, NULL, 'c'},
        {"http10", no_argument, NULL, '1'},
        {"http11", no_argument, NULL, '2'},
        {"get", no_argument, &method, METHOD_GET},
        {"head", no_argument, &method, METHOD_HEAD},
        {"options", no_argument, &method, METHOD_OPTIONS},
        {"trace", no_argument, &method, METHOD_TRACE},
        {"help", no_argument, NULL, '?'},
        {"version", no_argument, NULL, 'V'},
        {NULL, 0, NULL, 0}};
    std::string handle(int argc, char **argv)
    {
        int op;
        int options_index;
        while ((op = getopt_long(argc, argv, "frt:p:c:12?v", long_options,
                                 &options_index)) != -1)
        {
            switch (op)
            {

            case 'f':
                force = 1;
                break;
            case 'r':
                reload = 1;
                break;
            case 't':
                runtime = atoi(optarg);
                break;
            case 'p':
                ProxyServer = optarg;//未实现代理服务器
                break;
            case 'c':
                clients = atoi(optarg);
                break;
            case '1':
                http10 = true, http11 = false;
                break;
            case '2':
                http10 = false, http11 = true;
                break;
            case '?':
                usage();
                break;
            default:
                std::cout << "输入错误!" << std::endl;
                usage();
                break;
            }
        }
        if (optind == argc)
        {
            std::cout << "没有URL!" << std::endl;
            usage();
            return "";
        }

        return argv[optind];
    }

    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;

private:
    Parser()
        : force(0), reload(0), runtime(30), clients(1), http10(true),
          http11(false), ProxyServer(""), isexpired(false) {}
    std::vector<BenchRes> Result;
    std::mutex ThreadLock;
};