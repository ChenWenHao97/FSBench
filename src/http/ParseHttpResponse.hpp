#pragma once
#include <iostream>
#include <string>
using namespace std;

class ParseHttpResponse
{
public:
    bool HeadEndSymbol;
    int ContentLength;
    bool IsHeadEnd(const string &response)
    {
        string headend = "\r\n\r\n";
        int res = response.find(headend);
        if (res == -1)
            return false;
        return true;
    }
    int GetContentLength(const string &response)
    {
        string content = "Content-Length:";
        int contentbegin = response.find(content);
        if (contentbegin == -1)
            return -1;
        string end = "\r\n";
        int contentend = response.find(end, contentbegin);
        string contentstr = response.substr(contentbegin + 16, contentend - contentbegin - 16);

        return stoi(contentstr);
    }
    ParseHttpResponse() : HeadEndSymbol(false), ContentLength(0) {}
};