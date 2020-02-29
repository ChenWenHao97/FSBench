#include<iostream>
#include<string>
using namespace  std;

class ParseHttpResponse{
    public:
        bool HeadEndSymbol;
        int ContentLength;
        bool IsHeadEnd(const string& response)
        {
            // cout<<"!!!headend  in"<<endl;
            string headend = "\r\n\r\n";
            int res = response.find(headend);
            if(res == -1)
                return false;
            // cout<<"!!!headend  out"<<endl;
            return true;
        }
        int GetContentLength(const string& response)
        {
            string content = "Content-Length:";
            int contentbegin = response.find(content);
            if(contentbegin == -1)
                return -1;
            string end ="\r\n";
            int contentend = response.find(end,contentbegin);
            string contentstr = response.substr(contentbegin+16,contentend-contentbegin-16);
            cout<<"contentstr:"<<contentstr<<endl;
            cout <<stoi(contentstr)<<endl;
            return stoi(contentstr);
        }
        ParseHttpResponse():HeadEndSymbol(false),ContentLength(0){}
};