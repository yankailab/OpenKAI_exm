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
    // ͨ�ô�������
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
    //1) ���ͼ������ϵΪ�������Ϊ���ĵĿռ�ֱ������ϵ��
    //2) �������ͼ�ĵ�λΪLSB��������������fUnitOfDepth�󣬿ɽ���λת��Ϊmm��
    uint32_t eOutDepthValueType; //Depth value type for output

    //�������ͼ���������ȣ���λmm/LSB��
    //1) �����ͼ����������ΪUINT16ʱ���� (eOImgDataType == OIMG_DTYPE_UINT16)��
    //   ÿ������ֵ����һ����λΪ LSB ���������ֵ��
    //   �������ֵ������������ fUnitOfDepth �󣬿ɽ���λת��Ϊmm��
    //2) �����ͼ����������ΪDEPTH16ʱ���� (eOImgDataType == OIMG_DTYPE_DEPTH16)��
    //   ÿ������ֵ����һ�� DEPTH16 �ṹ����ֵ�����е��������ֵ usDepth
    //   ÿ�� LSB ��Ӧ 1mm������ʱ��fUnitOfDepth = 1mm/LSB��
    float fUnitOfDepth;

    //����Ƶ�� (Unit: MHz)
    //1) �����ͼ����N������Ƶ�����ɣ����¼��Ӧ��Ƶ��ֵ.
    float fModFreqMHZ[4];

    //��ͷ����
    //1) ��;�����ת���ƣ�������ͼ���ⲿ�������ͨ�����ͼ����ʱʹ�ã���
    //2) ����ͷ�궨��������ʧ�ܣ��򷵻صľ�ͷ����Ϊȫ0.
    //3) Order: [0]-fx, [1]-fy, [2]-cx, [3]-cy, [4]-FOVZoom.
    //4) FOVZoom: ��ͷУ�����ӳ�����ϵ����<1-�Ŵ󣨲úڱߣ���1-�����ţ�>1-��С
    float fLensParas[5]; //��ͷ����
})XdynDepthFrameInfo_t;


XD_PACKED(
typedef struct {
    XdynFrameInfo_t frameInfo;
    //���Ŷ�ͼ����
    //1) Used when (eOImgName == OIMG_NAME_CONFIDENCE).
    //2) �����Ŷ�ͼ����������Ϊ (eOImgDataType == OIMG_DTYPE_UINT8) ʱ��
    //   ÿ�����ص����Ŷ�ֵΪ0~255���ֱ��Ӧ���Ŷ�Ϊ0.f(���Ŷȵ�)~1.f(���Ŷȸ�)��
    //   ÿ�����Ŷ�ֵ���� fUnitOfConfidence �󣬿ɻ����� 0.f~1.f ֮��ʱ��
    float fConfidenceInt2Float; //���Ŷȴ������򸡵��͵�ת����λ
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

    // ��Ϣ��ȡ�¼�
    XDYN_CB_EVENT_CAM_INFO = 0x00000001,
    XDYN_CB_EVENT_STM_STATUS = 0x00000002,
    
    // �쳣�����¼�
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
    float fLenParas[9]; //��ͷУ������[fx, fy, cx, cy, k1, k2, p1, p2, k3]
    float fZoomCoef; //��ͷУ����ͼ�������ϵ����<1-�Ŵ󣨲úڱߣ���1-�����ţ�>1-��С
    float fSkew; //����������������б���ɴ������İ�װû������ᴹֱ����
})XdynLensParams_t;

XD_PACKED(
typedef struct XdynRegParams_st
{
    float fTofIntrinsicMatrix[9]; //Tof������ڲξ���
    float fRgbIntrinsicMatrix[9]; //Rgb������ڲξ���
    float fRotationMatrix[9]; //�����������������ת����
    float fTranslationMatrix[3]; //���������������ƽ�ƾ���
    bool bIsRgbCameraLeft; 
})XdynRegParams_t;

}

#endif // XD_TYPE_H
