#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include <sys/time.h>
#endif
#include "xdyn_streamer.h"
#include "xdyn_define.h"
#include "xdyn_utils.h"

using namespace XD;

void EventCB(void *handle, int event, void *data)
{
    printf("get event id : %d\n", event);
}

XDYN_Streamer *stream = nullptr;
bool doExit = false;

void SignalFunc(int signNo)
{
    // if(stream == nullptr) return;
    
    doExit = true;
}

int main(int argc, char *argv[])
{
    int res = XD_SUCCESS;

    signal(SIGINT, SignalFunc); // ctrl + c
    // signal(SIGTERM, SignalFunc);
    #ifdef _WIN32
    signal(SIGBREAK, SignalFunc);
    #endif
    // signal(SIGABRT, SignalFunc);

    // init context
    XdynContextInit();

    stream = CreateStreamer(XDYN_PRODUCT_TYPE_XD_500, XDYN_SINK_TYPE_NET, EventCB, nullptr, "192.168.100.2");
    if(stream == nullptr){
        printf("get streamer failed, return\n");
        return -1;
    }

    XdynDev_SyncTime(stream);
    
    DestroyStreamer(stream);
    XdynContextUninit();
    return 0;
}
