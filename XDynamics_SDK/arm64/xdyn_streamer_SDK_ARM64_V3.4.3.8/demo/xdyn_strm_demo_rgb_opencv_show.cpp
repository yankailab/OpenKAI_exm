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

#include "global.h"

using namespace XD;

bool doExit = false;

typedef struct UsrHandle_st{
    XDYN_Streamer *strm;
    XdynRegParams_t regParams;
    RP_RGB_LENS regLens;
    XdynCamInfo_t camInfo;

    COOR_MAP stCoorMap;
    LEN_IMAP* pstInterpMap;
    int* pInterpInversMap;
}UsrHandle_t;


void UsrHdl_initRgb(UsrHandle_t *usrHdl, int width, int height)
{
    int len = width * height;
    usrHdl->pstInterpMap = (LEN_IMAP*)malloc(sizeof(LEN_IMAP) * len);
	usrHdl->pInterpInversMap = (int*)malloc(sizeof(int) * len);

	//进行RGB镜头参数的初始化工作
    usrHdl->stCoorMap.iRoiXMapUnitLen = width;
    usrHdl->stCoorMap.iRoiYMapUnitLen = height;
    usrHdl->stCoorMap.iRoiXMapTimes = 1;
    usrHdl->stCoorMap.iRoiYMapTimes = 1;
    if (usrHdl->pstInterpMap)
    {
        free(usrHdl->pstInterpMap);
        usrHdl->pstInterpMap = (LEN_IMAP*)malloc(sizeof(LEN_IMAP) * len);
    }

    if (usrHdl->pInterpInversMap)
    {
        free(usrHdl->pInterpInversMap);
        usrHdl->pInterpInversMap = (int*)malloc(sizeof(int) * len);
        for (int i = 0; i < len; i++)
        {
           usrHdl->pInterpInversMap[i] = -1;
        }
    }

    RP_CalcOutRgb2CaliCoorMap(&usrHdl->stCoorMap, width, height);
    RP_RgbLEN_CreateUndistortMap(&usrHdl->regLens, width, height, &usrHdl->stCoorMap, usrHdl->pstInterpMap,usrHdl->pInterpInversMap);
}


void UsrHdl_Release(UsrHandle_t *usrHdl)
{
    free(usrHdl->pstInterpMap);
    free(usrHdl->pInterpInversMap);
}

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

static void SaveImageData_rgb(void *data, uint32_t size, int max = 10)
{

    static int cnt = 0;
    static std::ofstream ofstrm;

    if(cnt >= max) return;

    if(!ofstrm.is_open()){
        ofstrm.open("rgb.yuv", std::ios::binary | std::ios::out);
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
    
    if(event == XDYN_CB_EVENT_DEVICE_DISCONNECT){
        printf("device is disconnect, exit!!\n");
        doExit = true;
    }
}

void StreamCB(void *handle, MemSinkCfg *cfg, XdynFrame_t *data)
{
    UsrHandle_t *usr = (UsrHandle_t *)handle;
    static bool isWriteFlag = true;

    printf("get stream data: ");
    for(int i = 0; i < MEM_AGENT_SINK_MAX; i ++){
        printf("[%d:%d] ", i, cfg->isUsed[i]);
    }
    printf("\n");

    Rect rgbRect;
    rgbRect.x = 204;
    rgbRect.y = 75;
    rgbRect.width = 257;
    rgbRect.height = 267;

    vector<Point> origPoint;
    origPoint.push_back(Point(rgbRect.x, rgbRect.y));
	origPoint.push_back(Point(rgbRect.x + rgbRect.width, rgbRect.y + rgbRect.height));

    vector<Point> point = origPoint;
    RP_GetRGBMappingPt(origPoint, point, usr->pInterpInversMap, 640, 480);

    float RGBMatrixT[9] = { 0 };
    float AleMatrixT[9] = { 0 };

	RP_Calc3x3TransposeMatrix(usr->regParams.fRotationMatrix, AleMatrixT);

	RP_Calc3x3InverseMatrix(usr->regParams.fRgbIntrinsicMatrix, RGBMatrixT);
    
    // GetIdent(nullptr, usr->camInfo.info.rgbH, usr->camInfo.info.rgbW, point, 
    //         RGBMatrixT, usr->regParams.fTranslationMatrix, 
    //         AleMatrixT, usr->regParams.fTofIntrinsicMatrix);

    // Rect tofRect;
	// tofRect.x = point[0].x;
	// tofRect.y = point[0].y;
	// tofRect.width = point[1].x - point[0].x;
	// tofRect.height = point[1].y - point[0].y;

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
                    printf("get depth size [%d], fUnitOfDepth = %f, hz[%f, %f] timestamp %llu\n", 
                            data[i].size, depthInfo->fUnitOfDepth, depthInfo->fModFreqMHZ[0], depthInfo->fModFreqMHZ[1], 
                            depthInfo->frameInfo.timestamp);

                    GetIdent((uint16_t *)data[i].addr, usr->camInfo.info.rgbH, usr->camInfo.info.rgbW, point, 
                            RGBMatrixT, usr->regParams.fTranslationMatrix, 
                            AleMatrixT, usr->regParams.fTofIntrinsicMatrix);

                    Rect tofRect;
                    tofRect.x = point[0].x;
                    tofRect.y = point[0].y;
                    tofRect.width = point[1].x - point[0].x;
                    tofRect.height = point[1].y - point[0].y;

                    Mat depthMat(depthInfo->frameInfo.height, depthInfo->frameInfo.width, CV_16U, Scalar(0));
                    ushort* D16 = &depthMat.ptr<ushort>(0)[0];
                    uint16_t *depethLsb = (uint16_t *)data[i].addr;
                    for (int j = 0; j < data[i].size / 2; j ++) {
                        D16[j] = depethLsb[j];
                    }

                    cv::rectangle(depthMat, tofRect, Scalar(65535), 1);
                    cv::rectangle(depthMat, rgbRect, Scalar(0), 1);
                    imshow("depth", depthMat);
                    cv::waitKey(100);

                    // imwrite(std::string("depth.jpg"), depthMat);
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
            }else if(i == MEM_AGENT_SINK_RGB){
                SaveImageData_rgb(data[i].addr, data[i].size, 10);
                cv::Mat yuvImg;
                cv::Mat rgbImg;
                yuvImg.create(usr->camInfo.info.rgbH * 3 / 2, usr->camInfo.info.rgbW, CV_8UC1);
                memcpy(yuvImg.data, data[i].addr, data[i].size);

                cvtColor(yuvImg, rgbImg, COLOR_YUV2BGR_NV12);

                cv::rectangle(rgbImg, rgbRect, Scalar(0), 1);
                
                cv::imshow("yuv", rgbImg);
                cv::waitKey(100);

                if(isWriteFlag){
                    cv::imwrite("rgb.jpg", rgbImg);
                    isWriteFlag = false;
                }

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

    UsrHandle_t usrHdl;
    MemSinkCfg memCfg;
    unsigned int phaseInt[4] = {1000000, 1000000, 0, 0};
    unsigned int specialInt[4] = {1000000, 1000000, 0, 0};
    
    memset(&usrHdl, 0, sizeof(usrHdl));

    // init context
    XdynContextInit();

    XDYN_Streamer *stream = CreateStreamerNet(XDYN_PRODUCT_TYPE_XD_1000, EventCB, &usrHdl, "192.168.31.3");
    if(stream == nullptr){
        printf("get streamer failed, return\n");
        return -1;
    }

    usrHdl.strm = stream;

    res = stream->OpenCamera(XDYN_DEV_TYPE_TOF_RGB);
    if(res != XD_SUCCESS){
        printf("open camera failed, exitm [%d]\n", res);
        goto END;
    }

    memCfg.isUsed[MEM_AGENT_SINK_DEPTH] = true;
    memCfg.isUsed[MEM_AGENT_SINK_CONFID] = true;
    memCfg.isUsed[MEM_AGENT_SINK_RGB] = true;
    res = stream->ConfigSinkType(XDYN_SINK_TYPE_CB, memCfg, StreamCB, &usrHdl);
    if(res != XD_SUCCESS){
        printf("config sink type failed, return [%d]\n", res);
        goto END;
    }

    res = stream->ConfigAlgMode(XDYN_ALG_MODE_EMB_ALG_IPC_PASS);
    if(res != XD_SUCCESS){
        printf("config alg failed, return [%d]\n", res);
        goto END;
    }

    stream->SetCamFreq(62, 25);
    stream->SetFps(10);
    stream->SetCamInt(phaseInt, specialInt);
    // stream->SetCamBinning(XDYN_BINNING_MODE_2x2); // 使用binning 2x2的方法，分辨率为320 * 240
    stream->SetPhaseMode(XDYN_PHASE_MODE_1);
    res = stream->ConfigCamParams();
    if(res != XD_SUCCESS){
        printf("conifg cam params failed, [%d]\n", res);
        goto END;
    }
    
    { // 配置RGB
        XdynRes_t rgbRes;
        rgbRes.width = 640;
        rgbRes.height = 480;
        rgbRes.stride = 0;
        rgbRes.fmt = 4;
        rgbRes.fps = 10;
        stream->RgbSetRes(rgbRes);
        res = stream->CfgRgbParams();
        if(res != XD_SUCCESS){
            printf("config rgb failed, [%d]\n", res);
            goto END;
        }
    }
                
    Sleep(1000);

    res = stream->GetCaliRegParams(usrHdl.regParams);
    printf("reg params res %d\n", res);

    XdynLensParams_t rgbLensParam;
    res = stream->GetRgbLensParams(rgbLensParam);
    memcpy(&usrHdl.regLens, &rgbLensParam, sizeof(XdynLensParams_t));

    stream->GetCamInfo(&usrHdl.camInfo);

    UsrHdl_initRgb(&usrHdl, 640, 480);

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
