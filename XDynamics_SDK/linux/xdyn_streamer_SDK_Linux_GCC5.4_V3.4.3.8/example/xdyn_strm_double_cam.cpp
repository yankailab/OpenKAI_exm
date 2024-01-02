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
#include "depth2PC.h"

using namespace XD;

bool doExit = false;

struct UserHandler
{
    uint32_t flag;
    XDYN_Streamer *stream;
    void *user;
    PCConver pcConver;
    PCPoint_t *pcAttr;
    uint32_t imgSize;
    uint32_t other;

    // save data
    std::ofstream ofs;
    int cnt;
};

static void SaveImageData_depth(UserHandler *hdl, void *data, uint32_t size, int max = 10)
{
    if(hdl->cnt >= max) return;

    hdl->ofs << std::string((char *)data, size);

    hdl->cnt ++;
    if(hdl->cnt == max){
        hdl->ofs.close();
    }
}

void EventCB(void *handle, int event, void *data)
{
    UserHandler *user = (UserHandler *)handle;
    
    if(event == XDYN_CB_EVENT_DEVICE_DISCONNECT){
        printf("device is disconnect, exit!!\n");
        // doExit = true;
    }
}

void StreamCB(void *handle, MemSinkCfg *cfg, XdynFrame_t *data)
{

    UserHandler *user = (UserHandler *)handle;

    // wo can do some user handler
    // warnning : 这个回调函数内部不适合做耗时很大的操作，请注意

    printf("[%d] get stream data: ", user->flag);
    for(int i = 0; i < MEM_AGENT_SINK_MAX; i ++){
        printf("[%d:%d] ", i, cfg->isUsed[i]);
    }
    printf("\n");

    for(int i = 0; i < MEM_AGENT_SINK_MAX; i ++){
        if(cfg->isUsed[i] && data[i].addr){
            if(i == MEM_AGENT_SINK_DEPTH){
                SaveImageData_depth(user, data[i].addr, data[i].size);
                if(data[i].ex){
                    XdynDepthFrameInfo_t *depthInfo = (XdynDepthFrameInfo_t *)data[i].ex;
                    if(!user->pcConver.IsInit()){
                        MemSinkInfo info;
                        user->stream->GetResolution(info);
                        user->pcConver.Init(info, depthInfo->fUnitOfDepth, depthInfo->fLensParas);
                        user->imgSize = info.width * info.height;
                        user->pcAttr  = (PCPoint_t *)calloc(1, info.width * info.height * sizeof(PCPoint_t));
                    }

                    // 将深度转化为点云
                    // user->pcConver.Depth2PC((uint16_t *)data[i].addr, user->pcAttr);

                    // 保存点云
                    // SaveImageData_pointcloud(user->pcAttr, user->imgSize * sizeof(PCPoint_t));
                    printf("[%d] get depth size [%d], fUnitOfDepth = %f, hz[%f, %f]\n", 
                            user->flag, data[i].size, depthInfo->fUnitOfDepth, 
                            depthInfo->fModFreqMHZ[0], depthInfo->fModFreqMHZ[1]);
                }
            }else if(i == MEM_AGENT_SINK_POINT_CLOUD){
            }else if(i == MEM_AGENT_SINK_GRAY){
            }else if(i == MEM_AGENT_SINK_CONFID){
                printf("[%d] get confid size [%d]\n", user->flag, data[i].size);
            }
        }
    }
}

UserHandler *UserHandle_Create(std::string prtIP, uint32_t flag)
{
    int res = XD_SUCCESS;
    MemSinkCfg memCfg;
    XdynCamInfo_t camInfo;
    unsigned int phaseInt[4] = {1000000, 1000000, 0, 0};
    unsigned int specialInt[4] = {1000000, 1000000, 0, 0};

    UserHandler *userHdl = new UserHandler();
    userHdl->flag = flag;
    userHdl->cnt = 0;

    char buf[100];
    sprintf(buf, "depth_%d.dat", userHdl->flag);
    userHdl->ofs.open(std::string(buf), std::ios::binary | std::ios::out);

    XDYN_Streamer *stream = CreateStreamerNet(XDYN_PRODUCT_TYPE_XD_500, EventCB, userHdl, prtIP);
    if(stream == nullptr){
        printf("get streamer failed, return\n");
        delete userHdl;
        return nullptr;
    }

    userHdl->stream = stream;

    res = stream->OpenCamera(XDYN_DEV_TYPE_TOF);
    if(res != XD_SUCCESS){
        printf("open camera failed, exitm [%d]\n", res);
        goto END;
    }

    memCfg.isUsed[MEM_AGENT_SINK_DEPTH] = true;
    // memCfg.isUsed[MEM_AGENT_SINK_GRAY] = true;
    memCfg.isUsed[MEM_AGENT_SINK_CONFID] = true;
    res = stream->ConfigSinkType(XDYN_SINK_TYPE_CB, memCfg, StreamCB, userHdl);
    if(res != XD_SUCCESS){
        printf("config sink type failed, return [%d]\n", res);
        goto END;
    }

    res = stream->ConfigAlgMode(XDYN_ALG_MODE_EMB_ALG_IPC_PASS);
    if(res != XD_SUCCESS){
        printf("config alg failed, return [%d]\n", res);
        goto END;
    }

    stream->SetFps(10);
    stream->SetCamInt(phaseInt, specialInt);
    stream->SetCamBinning(XDYN_BINNING_MODE_2x2); // 使用binning 2x2的方法，分辨率为320 * 240
    res = stream->ConfigCamParams();
    if(res != XD_SUCCESS){
        printf("conifg cam params failed, [%d]\n", res);
        goto END;
    }

    return userHdl;

END:
    stream->StopStreaming();
    stream->CloseCamera();
    DestroyStreamer(stream);
    
    delete stream;
    delete userHdl;
    return nullptr;
}



int UserHandle_Start(UserHandler *hdl)
{
    int res = XD_SUCCESS;
    if(hdl && hdl->stream){
        res = hdl->stream->StartStreaming();
        if(res != XD_SUCCESS){
            printf("strart streaming failed, [%d]\n", res);
            return XD_ERR;
        }
    }else{
        return XD_ERR;
    }
    
    return XD_SUCCESS;
}

void UserHandle_stop(UserHandler *hdl)
{
    if(hdl && hdl->stream){
        hdl->stream->StopStreaming();
    }
}


void UserHandle_close(UserHandler *hdl)
{
    if(hdl && hdl->stream){
        hdl->stream->CloseCamera();
    }
}

void UserHandle_Destory(UserHandler *hdl)
{
    if(hdl){
        if(hdl->stream){
            hdl->stream->StopStreaming();
            hdl->stream->CloseCamera();
            DestroyStreamer(hdl->stream);
        }
        delete hdl;
    }
}


void SignalFunc(int signNo)
{    
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

    UserHandler *usrHdl1 = UserHandle_Create("192.168.100.2", 1);
    UserHandler *usrHdl2 = UserHandle_Create("192.168.100.3", 2);

    UserHandle_Start(usrHdl1);
    UserHandle_Start(usrHdl2);
    
    while (!doExit){
#ifdef _WIN32
        Sleep(1000);
#elif __linux__
        usleep(1000000);
#endif
    }

    UserHandle_Destory(usrHdl1);
    UserHandle_Destory(usrHdl2);

    XdynContextUninit();
    return 0;
}
