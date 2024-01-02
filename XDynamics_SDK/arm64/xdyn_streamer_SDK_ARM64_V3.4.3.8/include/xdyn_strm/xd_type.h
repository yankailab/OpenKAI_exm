#ifndef XD_TYPE_H
#define XD_TYPE_H

#include <stdint.h>
#include "mem_sink_cfg.h"

#ifdef __GNUC__
  #define XD_PACKED( __Declaration__ ) __Declaration__ __attribute__((packed))
#elif defined(_WIN32)
  #define XD_PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#endif
namespace XD {

enum XdRes {
    // 通用错误反馈码
    XD_SUCCESS = 0,
    XD_ERR = -1,
    XD_INVALRES = -2,
    XD_TIMEOUT = -3,
    XD_UNINIT = -4,
};

#define XD_CHECK_RES(res) if(res != XD_SUCCESS) return res;

typedef struct{
    uint16_t type;
    uint16_t major;
    uint16_t minor;
    uint16_t revision;
    uint16_t temp;
}XdynVersion_t;

struct FrameResolution{
    uint16_t width;
    uint16_t height;
    uint16_t stride;
    uint32_t fmt;
};

XD_PACKED(
typedef struct{
    uint16_t width;
    uint16_t height;
    uint16_t format;
    uint16_t index;
    uint32_t rx_ts;
    uint32_t tx_us;
    uint16_t temp0;
    uint16_t temp1;
    uint64_t timestamp;
    uint32_t reserved[18];
})XdynFrameInfo_t;

XD_PACKED(
typedef struct {
    XdynFrameInfo_t frameInfo;
    //1) 深度图的坐标系为：以相机为中心的空间直角坐标系。
    //2) 整型深度图的单位为LSB，乘以量化精度fUnitOfDepth后，可将单位转化为mm。
    uint32_t eOutDepthValueType; //Depth value type for output

    //整型深度图的量化精度，单位mm/LSB。
    //1) 当深度图的数据类型为UINT16时，即 (eOImgDataType == OIMG_DTYPE_UINT16)，
    //   每个像素值代表一个单位为 LSB 的整型深度值，
    //   整型深度值乘以量化精度 fUnitOfDepth 后，可将单位转化为mm。
    //2) 当深度图的数据类型为DEPTH16时，即 (eOImgDataType == OIMG_DTYPE_DEPTH16)，
    //   每个像素值代表一个 DEPTH16 结构体数值，其中的整型深度值 usDepth
    //   每个 LSB 对应 1mm，即此时，fUnitOfDepth = 1mm/LSB。
    float fUnitOfDepth;

    //调制频率 (Unit: MHz)
    //1) 若深度图是由N个调制频率生成，则记录相应的频率值.
    float fModFreqMHZ[4];

    //镜头参数
    //1) 用途：深度转点云（当点云图由外部软件自行通过深度图计算时使用）。
    //2) 若镜头标定参数加载失败，则返回的镜头参数为全0.
    //3) Order: [0]-fx, [1]-fy, [2]-cx, [3]-cy, [4]-FOVZoom.
    //4) FOVZoom: 镜头校正的视场缩放系数：<1-放大（裁黑边），1-不缩放，>1-缩小
    float fLensParas[5]; //镜头参数
})XdynDepthFrameInfo_t;


XD_PACKED(
typedef struct {
    XdynFrameInfo_t frameInfo;
    //置信度图参数
    //1) Used when (eOImgName == OIMG_NAME_CONFIDENCE).
    //2) 当置信度图的数据类型为 (eOImgDataType == OIMG_DTYPE_UINT8) 时，
    //   每个像素的置信度值为0~255，分别对应置信度为0.f(置信度低)~1.f(置信度高)。
    //   每个置信度值乘以 fUnitOfConfidence 后，可换算至 0.f~1.f 之间时。
    float fConfidenceInt2Float; //置信度从整型向浮点型的转换单位
})XdynConfidenceInfo_t;


typedef struct XdynFrame_st{
    void *addr;
    uint32_t size;
    void *ex;

    XdynFrame_st() : addr(nullptr), size(0), ex(nullptr){}
    XdynFrame_st& operator=(XdynFrame_st &frame)
    {
        this->addr = frame.addr;
        this->size = frame.size;
        this->ex = frame.ex;
        return *this;
    }
}XdynFrame_t;

typedef void (*XdynEventCB)(void *handle, int event, void *data);
typedef void (*XdynStreamCB)(void *handle, MemSinkCfg *cfg, XdynFrame_t *data);


typedef enum{
    XDYN_CB_EVENT_INVALIB = 0x00000000,

    // 信息获取事件
    XDYN_CB_EVENT_CAM_INFO = 0x00000001,
    XDYN_CB_EVENT_STM_STATUS = 0x00000002,
    
    // 异常反馈事件
    XDYN_CB_EVENT_ERROR = 0x00010001,
    XDYN_CB_EVENT_DEVICE_DISCONNECT = 0x00010002,
}XDYN_CB_EVENT_E;

typedef struct
{
    /// PHASE 1-4 integration time in nano-second
    uint32_t PHASE1_4TInt;
    /// PHASE 5-8 integration time in nano-second
    uint32_t PHASE5_8TInt;
    /// special phase integration time in nano-second
    uint32_t specialPhaseTInt;
}XdynCamTInt_t;

typedef struct XdynCamInfo_st{
    MemSinkInfo info;
    
    uint32_t tofChipID;
    uint32_t tofUID[2];
    int32_t tSensor;
    int32_t tDriver;

    XdynCamTInt_t tInt[4];

}XdynCamInfo_t;

typedef struct XdynStmStatus_st{
    uint32_t state;
    uint32_t tSensor;
    uint32_t tDriver;
    uint32_t reserve[10];
}XdynStmStatus_t;

typedef struct
{
    uint32_t addr;
    uint32_t val;
}XdynReg_t;

typedef struct
{
    /// x start position (value starts from 0)
    uint16_t xStart;
    /** num samples per row before Resolution Reduction (must be multiple of 4!).

    NOTE: this is not the original register field of xSize.
    */
    uint16_t xSizeOrg;
    /// x step size (1-32)
    uint16_t xStep;
    /// y start position (value starts from 0), when analog binning is used, yStart must be EVEN number
    uint16_t yStart;
    /** num samples per column before Resolution Reduction.

    NOTE: this is not the original register field of ySize.
    */
    uint16_t ySizeOrg;
    /// y step size (1-32), when analog binning is used, yStep must be EVEN number!
    uint16_t yStep;
}XdynROIDef_t;

typedef struct XdynRes_st{
    uint16_t width;
    uint16_t height;
    uint16_t stride;
    uint32_t fmt;
    
    uint16_t fps;
}XdynRes_t;

XD_PACKED(
typedef struct XdynLensParams_st
{
    float fLenParas[9]; //镜头校正参数[fx, fy, cx, cy, k1, k2, p1, p2, k3]
    float fZoomCoef; //镜头校正后图像的缩放系数：<1-放大（裁黑边），1-不缩放，>1-缩小
    float fSkew; //描述传感器轴间的倾斜，由传感器的安装没有与光轴垂直引起
})XdynLensParams_t;

XD_PACKED(
typedef struct XdynRegParams_st
{
    float fTofIntrinsicMatrix[9]; //Tof相机的内参矩阵
    float fRgbIntrinsicMatrix[9]; //Rgb相机的内参矩阵
    float fRotationMatrix[9]; //右相机相对左相机的旋转矩阵
    float fTranslationMatrix[3]; //右相机相对左相机的平移矩阵
    bool bIsRgbCameraLeft; 
})XdynRegParams_t;

}

#endif // XD_TYPE_H
