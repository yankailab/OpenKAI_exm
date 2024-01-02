#ifndef XDYN_STREAM_DEFINE_H
#define XDYN_STREAM_DEFINE_H

#include <stdint.h>

namespace XD {

typedef enum
{
    XDYN_PRODUCT_TYPE_XD_500 = 0u,
    XDYN_PRODUCT_TYPE_XD_1000 = 1u,

    XDYN_PRODUCT_TYPE_XD_300 = 0x100u,
    XDYN_PRODUCT_TYPE_XD_500_LH = 0x201u,
    XDYN_PRODUCT_TYPE_XD_500_XB = 0x202u,
    XDYN_PRODUCT_TYPE_XD_500_SJ = 0x203u,
    XDYN_PRODUCT_TYPE_XD_500_STD = 0x204u,

    XDYN_PRODUCT_TYPE_XD_1001 = 0x401u,
    XDYN_PRODUCT_TYPE_XD_1002 = 0x402u,
    XDYN_PRODUCT_TYPE_XD_1003 = 0x403u,

    XDYN_PRODUCT_TYPE_XD_400 = 0x800u,

    XDYN_PRODUCT_TYPE_XD_1100 = 0x464u,
    XDYN_PRODUCT_TYPE_XD_1101 = 0x465u,
    XDYN_PRODUCT_TYPE_XD_1102 = 0x466u,

    XDYN_PRODUCT_TYPE_XD_300_MASK = 0x100u,
    XDYN_PRODUCT_TYPE_XD_500_MASK = 0x200u,
    XDYN_PRODUCT_TYPE_XD_1000_MASK = 0x400u,
    XDYN_PRODUCT_TYPE_XD_400_MASK = 0x800u,

    XDYN_PRODUCT_TYPE_INVALIB = 0xFFFFFFFFu,
} XDYN_PRODUCT_TYPE_e;

typedef enum
{
    XDYN_DEV_TYPE_TOF = 0u,
    XDYN_DEV_TYPE_RGB,
    XDYN_DEV_TYPE_TOF_RGB,
    XDYN_DEV_TYPE_VDRIVER,
    XDYN_DEV_TYPE_ISP1,
    XDYN_DEV_TYPE_ISP2,
    XDYN_DEV_TYPE_TOTAL_NUM
} XDYN_DEV_TYPE_e;


typedef enum{
    XDYN_SINK_TYPE_NONE = 0x00000000,
    XDYN_SINK_TYPE_UVC = 0x00000001,
    XDYN_SINK_TYPE_SHM = 0x00000002,
    XDYN_SINK_TYPE_NET = 0x00000004,
    XDYN_SINK_TYPE_CB = 0x00000008,
    XDYN_SINK_TYPE_CYPRESS = 0x00000010,
}XDYN_SINK_TYPE_E;

typedef enum{
    XDYN_ALG_MODE_EMB_ALG_IPC_PASS = 0,
    XDYN_ALG_MODE_EMB_PASS_IPC_ALG = 1,
    XDYN_ALG_MODE_EMB_CORR_IPC_POST = 2,
    XDYN_ALG_MODE_EMB_DEPTH_IPC_ALG = 3,
    XDYN_ALG_MODE_EMB_DEBUG_IPC_ALG = 4,
    XDYN_ALG_MODE_EMB_PASS_IPC_PASS = 5,
    XDYN_ALG_MODE_INVALID = 0xFFFFFFFF
}XDYN_ALG_MODE_E;

typedef enum
{
    XDYN_BINNING_MODE_NONE = 0u,
    /// average two adjacent pixels in one column
    XDYN_BINNING_MODE_ANALOG,
    /// average two adjacent pixels in one row
    XDYN_BINNING_MODE_DIGITAL,
    //sensor config only analog binning(1col*2row), server finish digital binning
    XDYN_BINNING_MODE_2x2,
    //sensor config only analog binning(1col*2row), server finish the remain analog&digital binning,
    XDYN_BINNING_MODE_4x4,

    XDYN_BINNING_MODE_TOTAL_NUM
} XDYN_BINNING_MODE_e;

typedef enum
{
    XDYN_MIRROR_MODE_NONE = 0u,
    XDYN_MIRROR_MODE_VERTICAL,
    XDYN_MIRROR_MODE_HORIZONTAL,
    XDYN_MIRROR_MODE_BOTH,
    XDYN_MIRROR_MODE_TOTAL_NUM
} XDYN_MIRROR_MODE_e;

typedef enum
{
    XDYN_TEMP_SENSOR_MODE_NONE = 0u,
    XDYN_TEMP_SENSOR_MODE_EVERYPHASE,
    XDYN_TEMP_SENSOR_MODE_EVERYSUBFRAME,
    XDYN_TEMP_SENSOR_MODE_TOTAL_NUM
} XDYN_T_SENSOR_MODE_e;

typedef enum
{
    XDYN_PHASE_MODE_1 = 0,
    XDYN_PHASE_MODE_2,
    XDYN_PHASE_MODE_4,
    XDYN_PHASE_MODE_8,
    XDYN_PHASE_MODE_16,
    XDYN_PHASE_MODE_TOTAL_NUM
} XDYN_PHASE_MODE_e;


typedef enum
{
    XDYN_S_IDLE = 0,
    XDYN_S_CONNECTED,
    XDYN_S_STREAMING,
    XDYN_S_DISCONNECTED,
    XDYN_S_TOTAL_NUM
} XDYN_FSM_STATE_e;

typedef enum 
{
    XDYN_PP_DENOISE_LEVEL_0 = 0,         //去噪等级 - 0级/不去噪
    XDYN_PP_DENOISE_LEVEL_1,             //去噪等级 - 1级
    XDYN_PP_DENOISE_LEVEL_2,             //去噪等级 - 2级
    XDYN_PP_DENOISE_LEVEL_3,             //去噪等级 - 3级
    XDYN_PP_DENOISE_LEVEL_4,             //去噪等级 - 4级
    XDYN_PP_DENOISE_LEVEL_5,             //去噪等级 - 5级
}XDYN_PP_DENOISE_LEVEL;


//------------------------------------------------------------------------------
//定义时域去噪方法
typedef enum 
{
    XDYN_PP_TDENOISE_METHOD_MF = 0,      //时域去噪方法 - MF
    XDYN_PP_TDENOISE_METHOD_FIR,         //时域去噪方法 - FIR
}XDYN_PP_TDENOISE_METHOD;


//------------------------------------------------------------------------------
//定义空域去噪方法
typedef enum 
{
    XDYN_PP_SDENOISE_METHOD_MF = 0,      //空域去噪方法 - MF
    XDYN_PP_SDENOISE_METHOD_BF,          //空域去噪方法 - BF
    XDYN_PP_SDENOISE_METHOD_NLM,         //空域去噪方法 - NLM
}XDYN_PP_SDENOISE_METHOD;


}

#endif // XDYN_STREAM_DEFINE_H
