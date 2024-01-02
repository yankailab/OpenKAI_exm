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
    XDYN_Streamer *stream;
    void *user;
    PCConver pcConver;
    PCPoint_t *pcAttr;
    uint32_t imgSize;
    uint32_t other;
};

static void SaveImageData_depth(void *data, uint32_t size, int max = 10)
{

    static int cnt = 0;
    static std::ofstream ofstrm;

    if(cnt >= max) return;

    if(!ofstrm.is_open()){
        ofstrm.open("depth.dat", std::ios::binary | std::ios::out);
    }

    ofstrm << std::string((char *)data, size);

    cnt ++;
    if(cnt == max){
        ofstrm.close();
    }
}

static void SaveImageData_gray(void *data, uint32_t size, int max = 10)
{

    static int cnt = 0;
    static std::ofstream ofstrm;

    if(cnt >= max) return;

    if(!ofstrm.is_open()){
        ofstrm.open("gray.dat", std::ios::binary | std::ios::out);
    }

    ofstrm << std::string((char *)data, size);

    cnt ++;
    if(cnt == max){
        ofstrm.close();
    }
}

static void SaveImageData_amp(void *data, uint32_t size, int max = 10)
{

    static int cnt = 0;
    static std::ofstream ofstrm;

    if(cnt >= max) return;

    if(!ofstrm.is_open()){
        ofstrm.open("amp.dat", std::ios::binary | std::ios::out);
    }

    ofstrm << std::string((char *)data, size);

    cnt ++;
    if(cnt == max){
        ofstrm.close();
    }
}

static void SaveImageData_pointcloud(void *data, uint32_t size, int max = 10)
{

    static int cnt = 0;
    static std::ofstream ofstrm;

    if(cnt >= max) return;

    if(!ofstrm.is_open()){
        ofstrm.open("point_cloud.dat", std::ios::binary | std::ios::out);
    }

    ofstrm << std::string((char *)data, size);

    cnt ++;
    if(cnt == max){
        ofstrm.close();
    }
}

static void SavePointCloud_ply(void *data, uint32_t size)
{
    static int num = 0;
    bool isLimitConfidence = true;
    float validConfidenceMin = 0.8f;
    std::ofstream plyFile(std::string("pt_") + std::to_string(num) + std::string(".ply"), std::ios::out);
    if(!plyFile.is_open()){
        std::cout << "open failed, return" << std::endl;
        return ;
    }

    num ++;

    float *pTmp = nullptr;
    int len = size / sizeof(float);
    int eleCnt = 0;

    pTmp = (float *)data;
    for(int i  = 0; i < len; i += 4, pTmp += 4){
        if(isLimitConfidence && pTmp[3] < validConfidenceMin)
            continue;
        eleCnt ++;
    }

    // add ply header info
    plyFile << "ply\n";
    plyFile << "format ascii 1.0\n";
    plyFile << "element vertex " << std::to_string(eleCnt) << "\n";
    plyFile << "property float x\n";
    plyFile << "property float y\n";
    plyFile << "property float z\n";
    plyFile << "end_header\n";

    pTmp = (float *)data;
    for(int i = 0 ; i < len; i += 4, pTmp += 4){
        std::string outStr = "";
        if(isLimitConfidence && pTmp[3] < validConfidenceMin)
            continue;

        outStr += std::to_string(pTmp[0]);
        outStr += " ";
        outStr += std::to_string(pTmp[1]);
        outStr += " ";
        outStr += std::to_string(pTmp[2]);
        outStr += "\n";
        plyFile << outStr;
    }

    plyFile.close();
}

void EventCB(void *handle, int event, void *data)
{
    UserHandler *user = (UserHandler *)handle;
    
    if(event == XDYN_CB_EVENT_DEVICE_DISCONNECT){
        printf("device is disconnect, exit!!\n");
        doExit = true;
    }
}

void StreamCB(void *handle, MemSinkCfg *cfg, XdynFrame_t *data)
{

    UserHandler *user = (UserHandler *)handle;

    // wo can do some user handler
    // warnning : 这个回调函数内部不适合做耗时很大的操作，请注意

    printf("get stream data: ");
    for(int i = 0; i < MEM_AGENT_SINK_MAX; i ++){
        printf("[%d:%d] ", i, cfg->isUsed[i]);
    }
    printf("\n");

    for(int i = 0; i < MEM_AGENT_SINK_MAX; i ++){
        if(cfg->isUsed[i] && data[i].addr){
            if(i == MEM_AGENT_SINK_DEPTH){
                SaveImageData_depth(data[i].addr, data[i].size, 10);
                if(data[i].ex){
                    XdynDepthFrameInfo_t *depthInfo = (XdynDepthFrameInfo_t *)data[i].ex;
                    // if(!user->pcConver.IsInit()){
                    //     MemSinkInfo info;
                    //     user->stream->GetResolution(info);
                    //     user->pcConver.Init(info, depthInfo->fUnitOfDepth, depthInfo->fLensParas);
                    //     user->imgSize = info.width * info.height;
                    //     user->pcAttr  = (PCPoint_t *)calloc(1, info.width * info.height * sizeof(PCPoint_t));
                    // }

                    // 将深度转化为点云
                    // user->pcConver.Depth2PC((uint16_t *)data[i].addr, user->pcAttr);
                    
                    // 保存点云
                    // SaveImageData_pointcloud(user->pcAttr, user->imgSize * sizeof(PCPoint_t));
                    printf("get depth size [%d], fUnitOfDepth = %f, hz[%f, %f] timestamp %lu\n", 
                            data[i].size, depthInfo->fUnitOfDepth, depthInfo->fModFreqMHZ[0], depthInfo->fModFreqMHZ[1], 
                            depthInfo->frameInfo.timestamp);
                }
            }else if(i == MEM_AGENT_SINK_POINT_CLOUD){
                SaveImageData_pointcloud(data[i].addr, data[i].size);
                //SavePointCloud_ply(memData[i].addr, memData[i].size);
                // printf("get point cloud size [%d]\n", data[i].size);
            }else if(i == MEM_AGENT_SINK_GRAY){
                // printf("get gray size [%d]\n", data[i].size);
                SaveImageData_gray(data[i].addr, data[i].size);
            }else if(i == MEM_AGENT_SINK_CONFID){
                // printf("get confid size [%d]\n", data[i].size);
                SaveImageData_amp(data[i].addr, data[i].size);
            }
        }
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

    UserHandler userHdl;

    MemSinkCfg memCfg;
    XdynCamInfo_t camInfo;
    unsigned int phaseInt[4] = {1000000, 1000000, 0, 0};
    unsigned int specialInt[4] = {1000000, 1000000, 0, 0};

    // init context
    XdynContextInit();

    // std::vector<XdynProductDesc_t> prtList;
    // ScanProductFromNet(prtList, "192.168.100.2");
    // if(prtList.empty()){
    //     printf("not found product connect, return\n");
    //     return -1;
    // }

    // printf("scan product list :\n");
    // for(auto i = 0; i < prtList.size(); i ++){
    //     printf("    type = %d, werlfID = 0x%08x, inter1 = %d, inter2 = %d\n",
    //                prtList[i].type, prtList[i].warelfID, prtList[i].inter1, prtList[i].inter2);
    // }

    XDYN_Streamer *stream = CreateStreamerNet(XDYN_PRODUCT_TYPE_XD_400, EventCB, &userHdl, "192.168.31.3");
    // XDYN_Streamer *stream = CreateStreamer((XDYN_PRODUCT_TYPE_e)prtList[0].type, prtList[0].inter1, 
                            // EventCB, &userHdl, prtList[0].prtIP);
    if(stream == nullptr){
        printf("get streamer failed, return\n");
        return -1;
    }

    userHdl.stream = stream;

    res = stream->OpenCamera(XDYN_DEV_TYPE_TOF);
    if(res != XD_SUCCESS){
        printf("open camera failed, exitm [%d]\n", res);
        goto END;
    }

    stream->GetCamInfo(&camInfo);

    // memCfg.isUsed[MEM_AGENT_SINK_RAW] = true;
    memCfg.isUsed[MEM_AGENT_SINK_DEPTH] = true;
    memCfg.isUsed[MEM_AGENT_SINK_CONFID] = true;
    res = stream->ConfigSinkType(XDYN_SINK_TYPE_CB, memCfg, StreamCB, &userHdl);
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

    res = stream->StartStreaming();
    if(res != XD_SUCCESS){
        printf("strart streaming failed, [%d]\n", res);
        goto END;
    }
    
    while (!doExit){
        #ifdef _WIN32
        Sleep(1000);
        #elif __linux__
        usleep(1000000);
        #endif
        // stream->GetCamInfo(&camInfo);
        // printf("chipID = %08x, UID[0] = %08x, UID[1] = %08x, tSensor = %d\n", 
        //         camInfo.tofChipID, camInfo.tofUID[0], camInfo.tofUID[1], camInfo.tSensor);
    }

END:
    stream->StopStreaming();
    stream->CloseCamera();
    DestroyStreamer(stream);

    XdynContextUninit();
    return 0;
}
