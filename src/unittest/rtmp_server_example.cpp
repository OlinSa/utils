#include <iostream>
#include <fstream>
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include "rtmp_server.h"

using namespace std;

class Reader{
public:
    Reader(string filename):_filename(filename) {
        
    }
    ~Reader(){
        if(in) {
            in.close();
        }
    }
    bool Init(){
        in.open(_filename, ios::in | ios::binary);
        if(!in) {
            cout<<"open "<<_filename<<" failed"<<endl;
            return false;
        }
        return true;
    }
    int GetData(char *buf, int size) {
        in.read(buf, size);
        if(!in) {
            return -1;
        }
        return in.gcount();
    }

private:
    ifstream in;
    string _filename;
};

static void Usage(char *appname)
{
    cout<<appname<<" [option]"<<endl;
    cout<<"option"<<endl;
    cout<<"\t"<<"-f | --filename"<<endl;
    cout<<"\t"<<"-u | --url"<<endl;
    cout<<"\t"<<"-c | --cache"<<endl;
}

int main(int argc, char *argv[])
{
    string url = "rtmp://127.0.0.1:1935/live/test264";
    int cacheSize = 10 * 1024;
    string filename;

    int c;
    const char *optstring = "f:u:c:";
    static struct option long_options[] = {
        {"filename", required_argument,0, 'f'},
        {"url", optional_argument,0, 'u'},
        {"cache", optional_argument,0, 'c'}
    };
    int option_index;;
    while(-1 != (c = getopt_long(argc, argv, optstring, long_options, &option_index))) {
        switch (c)
        {
        case 0:
            cout<<"option "<<long_options[option_index].name<<" with arg"<<optarg<<endl;
            break;
        case 'f':
            filename = optarg;
            break;
        case 'u':
            url = optarg;
            break;
        case 'c':
            cacheSize = atoi(optarg);
            break;
        default:
            cout<<"?? get returned character code "<<c<<endl;
            Usage(argv[0]);
            return -1;
        }
    }
    cout<<"filename:"<<filename<<endl;
    cout<<"url:"<<url<<endl;
    cout<<"cacheSize:"<<cacheSize<<endl;

    Reader reader(filename);
    if(!reader.Init()) {
        cout<<"reader init failed"<<endl;
        return -1;
    }

    RtmpServer r;
    if(!r.Init(url, cacheSize)) {
        cout<<"rtmp init failed"<<endl;
        return -1;
    }

    int bufSize = 1024 * 100;
    char *buf = new char[bufSize];
    if(!buf) {
        cout<<"create buf failed "<<bufSize<<" byte"<<endl;
        return -1;
    }
    printf("#0x:%p\n",buf);
    int pos = 0;
    
    int ret;

    ssize_t size;
    while((ret = reader.GetData(buf+pos, bufSize- pos)) > 0){
        pos += ret;
        do {
            
            size =  r.PublishH264(buf, pos, NULL);  
            cout<<"send "<<size<<" byte"<<endl;
            if(size <  0) {
                break;
            }
            break;
            pos -= size;
        }while(pos > 0);
        break;
    }
    printf("##0x:%p\n",buf);
    delete[] buf;
    cout<<"pos:"<<pos<<endl;
    return 0;
}