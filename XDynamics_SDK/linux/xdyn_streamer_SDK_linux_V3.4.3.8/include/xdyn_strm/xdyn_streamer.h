#ifndef XDYN_STREAMER_H
#define XDYN_STREAMER_H

//后面或许还要加入产品自动检测的部分， 或者说叫做热插拔检测

#include <stdint.h>
#include <vector>
#include <string>

#include "xd_type.h"
#include "xdyn_define.h"
#include "xdyn_lib_export.h"
namespace XD {

class XDYN_LIB_API XDYN_Streamer
{
public:
    virtual ~XDYN_Streamer(){}

    virtual void GetVersion(uint16_t type, XdynVersion_t &version) = 0;

    // 相机开关操作
    virtual int OpenCamera(XDYN_DEV_TYPE_e devType) = 0;
    virtual int CloseCamera() = 0;

    // cfg product sink type
    virtual int ConfigSink(XDYN_SINK_TYPE_E sinkType) = 0;

    // cfg SDK sink type
    virtual int ConfigSinkType(uint32_t type, MemSinkCfg cfg) = 0;
    virtual int ConfigSinkType(uint32_t type, MemSinkCfg cfg, XdynStreamCB streamCB, void *stmHandle) = 0;
    // cfg alg mode
    virtual int ConfigAlgMode(uint32_t mode) = 0;

    // 配置TOF相机参数
    virtual void SetWorkMode(uint32_t mode, uint32_t subMode) = 0;
    virtual void SetCamFreq(uint32_t freq1, uint32_t freq2) = 0;
    virtual void SetCamInt(unsigned int *phaseInt, unsigned int *specialInt) = 0;
    virtual void SetFps(unsigned int fps) = 0;
    virtual void SetCamBinning(XDYN_BINNING_MODE_e mode) = 0;
    virtual void SetCamMirror(XDYN_MIRROR_MODE_e mode) = 0;
    virtual void SetPhaseMode(XDYN_PHASE_MODE_e mode) = 0;
    virtual void SetROI(XdynROIDef_t roi) = 0;
    virtual void SetMipiEndPos(uint32_t pos) = 0;
    virtual int ConfigCamParams() = 0;
    virtual XdynCamInfo_t GetCamInfo() = 0;
    virtual void GetCamInfo(XdynCamInfo_t *info) = 0;
    // 获取图像输出配置信息
    virtual void GetResolution(MemSinkInfo &info) = 0;

    // 配置 RGB 参数信息
    virtual void RgbSetRes(XdynRes_t res) = 0;
    virtual int CfgRgbParams() = 0;

    // 配置算法参数
    virtual void Corr_SetAE(uint8_t enable) = 0;
    virtual void Corr_SetPreDist(uint16_t dist) = 0;
    virtual int ConfigCorrParam() = 0;

    virtual void PP_SetDepthDenoise(XDYN_PP_TDENOISE_METHOD tDenoiseMethod, XDYN_PP_DENOISE_LEVEL tDenoiseLevel, XDYN_PP_SDENOISE_METHOD sDenoiseMethod, XDYN_PP_DENOISE_LEVEL sDenoiseLevel) = 0;
    virtual void PP_SetGrayDenoise(XDYN_PP_TDENOISE_METHOD tDenoiseMethod, XDYN_PP_DENOISE_LEVEL tDenoiseLevel, XDYN_PP_SDENOISE_METHOD sDenoiseMethod, XDYN_PP_DENOISE_LEVEL sDenoiseLevel) = 0;
    virtual void PP_SetDeFlyPixel(uint8_t level) = 0;
    virtual int ConfigPPParam() = 0;

    virtual int GetCaliRegParams(XdynRegParams_t &regParams) = 0;
    virtual int GetRgbLensParams(XdynLensParams_t &rgbLensParam) = 0;

    // 开启流
    virtual int StartStreaming() = 0;
    virtual int StopStreaming() = 0;

    // 读写寄存器，以及DLL配置功能
    virtual int SetDLL(int step) = 0;
    virtual int WriteReg(XDYN_DEV_TYPE_e type, XdynReg_t *regLists, int size) = 0;
    virtual int ReadReg(XDYN_DEV_TYPE_e type, XdynReg_t *regLists, int size) = 0;

};
                                
XDYN_LIB_API XDYN_Streamer *CreateStreamer(XDYN_PRODUCT_TYPE_e ptType, XDYN_SINK_TYPE_E ptInter,
                                           XdynEventCB evCB, void *evHandle, std::string ip = "192.168.31.3");

XDYN_LIB_API XDYN_Streamer *CreateStreamerNet(XDYN_PRODUCT_TYPE_e ptType, XdynEventCB evCB, void *evHandle, std::string ip);

XDYN_LIB_API void  DestroyStreamer(XDYN_Streamer *xdynStream);

XDYN_LIB_API void XdynContextInit();

XDYN_LIB_API void XdynContextUninit();

}



#endif // XDYN_STREAMER_H
