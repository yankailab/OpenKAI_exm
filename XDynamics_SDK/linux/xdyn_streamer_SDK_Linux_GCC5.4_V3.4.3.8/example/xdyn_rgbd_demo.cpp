#include <iostream>
#include <fstream>
#include <sstream>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <unistd.h>
#include <sys/time.h>
#endif
#include "xdyn_streamer.h"
#include "xdyn_define.h"
#include "xdyn_utils.h"

#include "sialib_TofRGBProc.h"
#include "siaGlobalOuter_RGBProc.h"

using namespace XD;

bool doExit = false;
struct UserHandler
{
    XDYN_Streamer *stream;
    void *user;
    uint32_t imgSize;
    uint32_t other;

    // for rgbd cal
    bool isRgbdInit;
    void *rgbdHdl;
    CALIPARAS_RP rgbdRPRes;
    RP_DYNPARA rgbddynParas;
    RP_INPARAS rgbdInDatas;
    RP_OUTPARAS rgbdOutDatas;
};

int UserHdl_Init(UserHandler *usrHdl)
{
    memset(usrHdl, 0, sizeof(UserHandler));
    return 0;
}

int UserHdl_InitRgbdHdl(UserHandler *usrHdl, XdynRegParams_t *regParams, uint16_t tofW, uint16_t tofH, uint16_t rgbW, uint16_t rgbH)
{
    uint32_t puiInitSuccFlag = RP_INIT_SUCCESS;

    if(usrHdl->isRgbdInit && usrHdl->rgbdHdl){
        sitrpRelease(&usrHdl->rgbdHdl, FALSE);
        usrHdl->rgbdHdl = nullptr;
        usrHdl->isRgbdInit = false;
    }

    char cDllVerion[RP_ARITH_VERSION_LEN_MAX] = { 0 }; //算法版本号字符串
    sitrpGetVersion(cDllVerion);

    printf("Get rgbd hdl version : %s\n", cDllVerion);

    sitrpSetTofIntrinsicMat(usrHdl->rgbdRPRes.fTofIntrinsicMatrix, regParams->fTofIntrinsicMatrix, RP_INTRINSIC_MATRIX_LEN, &puiInitSuccFlag, TRUE);
    sitrpSetRgbIntrinsicMat(usrHdl->rgbdRPRes.fRgbIntrinsicMatrix, regParams->fRgbIntrinsicMatrix, RP_INTRINSIC_MATRIX_LEN, &puiInitSuccFlag,TRUE);
    sitrpSetTranslationMat(usrHdl->rgbdRPRes.fTranslationMatrix, regParams->fTranslationMatrix, RP_TRANSLATION_MATRIX_LEN, &puiInitSuccFlag, TRUE);
    sitrpSetRotationMat(usrHdl->rgbdRPRes.fRotationMatrix, regParams->fRotationMatrix, RP_ROTATION_MATRIX_LEN, &puiInitSuccFlag, TRUE);
    sitrpSetRgbPos(&(usrHdl->rgbdRPRes.bIsRgbCameraLeft), regParams->bIsRgbCameraLeft, &puiInitSuccFlag, TRUE);

    usrHdl->rgbddynParas.usInDepthWidth = tofW;
    usrHdl->rgbddynParas.usInDepthHeight = tofH;
    usrHdl->rgbddynParas.usInYuvWidth = rgbW;
    usrHdl->rgbddynParas.usInYuvHeight = rgbH;
    
    usrHdl->rgbddynParas.usOutR2DWidth = tofW;
    usrHdl->rgbddynParas.usOutR2DHeight = tofH;

    usrHdl->rgbddynParas.usOutD2RWidth = tofW;
    usrHdl->rgbddynParas.usOutD2RHeight = tofH;

    usrHdl->rgbddynParas.uiOutRGBDLen = 0;
    usrHdl->rgbddynParas.ucEnableOutR2D = 0;
    usrHdl->rgbddynParas.ucEnableOutD2R = 0;
    usrHdl->rgbddynParas.ucEnableRGBDPCL = 1;
    usrHdl->rgbddynParas.ucThConfidence = 25;

    usrHdl->rgbdHdl = sitrpInit(&puiInitSuccFlag, &usrHdl->rgbdRPRes, &usrHdl->rgbddynParas, FALSE, FALSE);
    if (puiInitSuccFlag > RP_ARITH_SUCCESS){
        printf("\n Algorithm initialize fail!\n");
        printf("--- *puiInitSuccFlag: %d\n", puiInitSuccFlag);
        return -1;
    }

    // init in out buffer
    usrHdl->rgbdInDatas.pThisGlbBuffer = usrHdl->rgbdHdl;
    usrHdl->rgbdInDatas.pucYuvImg = nullptr;
    usrHdl->rgbdInDatas.usYuvWidth = rgbW;
    usrHdl->rgbdInDatas.usYuvHeight = rgbH;
    usrHdl->rgbdInDatas.pusDepth = nullptr;
    usrHdl->rgbdInDatas.usDepthWidth = tofW;
    usrHdl->rgbdInDatas.usDepthHeight = tofH;
    usrHdl->rgbdInDatas.pucConfidence = nullptr;
    usrHdl->rgbdInDatas.usConfWidth = tofW;
    usrHdl->rgbdInDatas.usConfHeight = tofH;

    usrHdl->rgbdOutDatas.ucEnableOutR2D = 0;
    usrHdl->rgbdOutDatas.ucEnableOutD2R = 0;
    usrHdl->rgbdOutDatas.pstrRGBD = (RGBD_POINT_CLOUD *)malloc(tofW * tofH * sizeof(RGBD_POINT_CLOUD));
    usrHdl->rgbdOutDatas.ucEnableRGBDPCL = 1;

    usrHdl->isRgbdInit = true;

    return 0;
}

void UserHdl_RealseRgbdHdl(UserHandler *usrHdl)
{
    if(!usrHdl->isRgbdInit)
        return ;
    
    usrHdl->isRgbdInit = false;
    
    sitrpRelease(&usrHdl->rgbdHdl, FALSE);
    usrHdl->rgbdHdl = nullptr;

    free(usrHdl->rgbdOutDatas.pstrRGBD);
}

void UserHdl_RgbdHdlImg(UserHandler *usrHdl)
{

}

void UserHdl_Release(UserHandler *usrHdl)
{
    UserHdl_RealseRgbdHdl(usrHdl);
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

// static void SavePointCloud_ply(RGBD_POINT_CLOUD *data, uint32_t size)
// {
//     static int num = 0;
//     bool isLimitConfidence = true;
//     float validConfidenceMin = 0.8f;
//     std::ofstream plyFile(std::string("pt_") + std::to_string(num) + std::string(".ply"), std::ios::out);
//     if(!plyFile.is_open()){
//         std::cout << "open failed, return" << std::endl;
//         return ;
//     }

//     num ++;

//     // add ply header info
//     plyFile << "ply\n";
//     plyFile << "format ascii 1.0\n";
//     plyFile << "element vertex " << std::to_string(eleCnt) << "\n";
//     plyFile << "property float x\n";
//     plyFile << "property float y\n";
//     plyFile << "property float z\n";
//     plyFile << "property uchar red\n";
//     plyFile << "property uchar green\n";
//     plyFile << "property uchar blue\n";
//     plyFile << "end_header\n";

//     for(int i = 0 ; i < size; i ++){
//         std::string outStr = "";
//         if(isLimitConfidence && pTmp[3] < validConfidenceMin)
//             continue;

//         outStr += std::to_string(pTmp[0]);
//         outStr += " ";
//         outStr += std::to_string(pTmp[1]);
//         outStr += " ";
//         outStr += std::to_string(pTmp[2]);
//         outStr += " ";
//         outStr += std::to_string(pTmp[3]);
//         outStr += " ";
//         outStr += std::to_string(pTmp[4]);
//         outStr += " ";
//         outStr += std::to_string(pTmp[5]);
//         outStr += "\n";
//         plyFile << outStr;
//     }

//     plyFile.close();
// }

unsigned char RP_SavePLY_File(const char* pucFileName, RGBD_POINT_CLOUD* pstRGBD, int iLen)
{

    FILE* fFileID = fopen(pucFileName, "w");
    //if (fFileID != NULL)
    {
        fprintf(fFileID, "ply\n");
        fprintf(fFileID, "format ascii 1.0\n");
        fprintf(fFileID, "element vertex %d\n", iLen);
        fprintf(fFileID, "property float x\n");
        fprintf(fFileID, "property float y\n");
        fprintf(fFileID, "property float z\n");
        fprintf(fFileID, "property uchar red\n");
        fprintf(fFileID, "property uchar green\n");
        fprintf(fFileID, "property uchar blue\n");
        fprintf(fFileID, "end_header\n");
        for (int i = 0; i < iLen; i++)
        {
            fprintf(fFileID, "%.1f %0.1f %0.1f %d %d %d\n", pstRGBD[i].fX, pstRGBD[i].fY,
                pstRGBD[i].fZ, pstRGBD[i].r, pstRGBD[i].g, pstRGBD[i].b);
        }
        fclose(fFileID);
    }
    return true;
}

static void SaveImageData_rgb(void *data, uint32_t size, int max = 10)
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

static void SaveImageData_conf(void *data, uint32_t size, int max = 10)
{

    static int cnt = 0;
    static std::ofstream ofstrm;

    if(cnt >= max) return;

    if(!ofstrm.is_open()){
        ofstrm.open("conf.dat", std::ios::binary | std::ios::out);
    }

    ofstrm << std::string((char *)data, size);

    cnt ++;
    if(cnt == max){
        ofstrm.close();
    }
}


void EventCB(void *handle, int event, void *data)
{
    UserHandler *user = (UserHandler *)handle;
    
    // if(event == XDYN_CB_EVENT_DEVICE_DISCONNECT){
    //     printf("device is disconnect, exit!!\n");
    //     doExit = true;
    // }
}

void StreamCB(void *handle, MemSinkCfg *cfg, XdynFrame_t *data)
{

    UserHandler *user = (UserHandler *)handle;
    unsigned int puiSuccFlag = 0;
    unsigned int puiAbnormalFlag = 0;
    static int num = 0;

    // wo can do some user handler
    // warnning : 这个回调函数内部不适合做耗时很大的操作，请注意

    printf("get stream data: ");
    for(int i = 0; i < MEM_AGENT_SINK_MAX; i ++){
        printf("[%d:%d] ", i, cfg->isUsed[i]);
    }
    printf("\n");

    if(!user->isRgbdInit) return;

    if(cfg->isUsed[MEM_AGENT_SINK_DEPTH] && 
        cfg->isUsed[MEM_AGENT_SINK_CONFID] && 
        cfg->isUsed[MEM_AGENT_SINK_RGB]){
        
        SaveImageData_rgb(data[MEM_AGENT_SINK_RGB].addr, data[MEM_AGENT_SINK_RGB].size);
        SaveImageData_depth(data[MEM_AGENT_SINK_DEPTH].addr, data[MEM_AGENT_SINK_DEPTH].size);
        SaveImageData_conf(data[MEM_AGENT_SINK_CONFID].addr, data[MEM_AGENT_SINK_CONFID].size);

        user->rgbdInDatas.pusDepth = (unsigned short *)data[MEM_AGENT_SINK_DEPTH].addr;
        user->rgbdInDatas.pucYuvImg = (unsigned char *)data[MEM_AGENT_SINK_RGB].addr;
        user->rgbdInDatas.pucConfidence = (unsigned char *)data[MEM_AGENT_SINK_CONFID].addr;

        sitrpRunRGBProcess(user->rgbdHdl, &user->rgbdInDatas, &user->rgbdOutDatas, &puiSuccFlag, &puiAbnormalFlag, FALSE);
        if(puiSuccFlag == RP_ARITH_SUCCESS){
            printf("get pc num : %d\n", user->rgbdOutDatas.uiOutRGBDLen);

            //save file 
            std::string fileName = std::string("pt_") + std::to_string(num ++) + std::string(".ply");
            RP_SavePLY_File(fileName.c_str(), user->rgbdOutDatas.pstrRGBD, 320 * 240);
        }else{
            printf("alg cal failed, %d\n", puiSuccFlag);
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

    // init user hdl
    UserHdl_Init(&userHdl);

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

    XDYN_Streamer *stream = CreateStreamerNet(XDYN_PRODUCT_TYPE_XD_500, EventCB, &userHdl, "192.168.31.3");
    // XDYN_Streamer *stream = CreateStreamer((XDYN_PRODUCT_TYPE_e)prtList[0].type, prtList[0].inter1, 
                            // EventCB, &userHdl, prtList[0].prtIP);
    if(stream == nullptr){
        printf("get streamer failed, return\n");
        return -1;
    }

    userHdl.stream = stream;

    res = stream->OpenCamera(XDYN_DEV_TYPE_TOF_RGB);
    if(res != XD_SUCCESS){
        printf("open camera failed, exitm [%d]\n", res);
        goto END;
    }

    memCfg.isUsed[MEM_AGENT_SINK_DEPTH] = true;
    memCfg.isUsed[MEM_AGENT_SINK_CONFID] = true;
    memCfg.isUsed[MEM_AGENT_SINK_RGB] = true;
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

    { // 配置RGB
        XdynRes_t rgbRes;
        rgbRes.width = 320;
        rgbRes.height = 240;
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

    // init rgbd interface
    XdynRegParams_t regParams;
    
    stream->GetCaliRegParams(regParams);
    res = UserHdl_InitRgbdHdl(&userHdl, &regParams, 320, 240, 320, 240);
    if(res != XD_SUCCESS){
        printf("config rgbd alg failed, [%d]\n", res);
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
