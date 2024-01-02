/***************************Copyright@SI****************************************
*
* IMPORTANT:    READ BEFORE DOWNLOADING, COPYING OR USING.
*
*   By downloading, copying or using the software you agree to this license.
*   If you do not agree to this license, do not download, copy or use the software.
*
* License Agreement:
*
*   Copyright (C) 2021-2023, Wuhan X-Dynamics Co., Ltd. All rights reserved.
*
*   User shall not reverse engineer, reverse compile or reverse assemble the
*   Software product.
*   Authorizations and licenses must be obtained from the copyright holder for
*   the sale, reproduction, distribution, including but not limited to software
*   sales, preloading, bundling, etc.
*   All other rights not expressly authorized by this Agreement shall remain
*   with the copyright holder, and user shall obtain the copyright holder's
*   written consent to use such other rights.
*   The copyright holder reserves the right to modify and upgrade the software
*   without users' request.
*   The user must abide by this agreement unless the above activities are
*   expressly permitted by laws and regulations.
*
* ------------------------------------------------------------------------------
*
* FileName:     siaGlobalOuter_RGBProc.h
* Abstract:     定义与算法外部输入、输出相关的宏定义、数据结构等。
* Description:  1) 可外部调用，可随dll一起发布。
*               2) 后处理算法库“siaGlobalOuter_RGBProc.h”内的宏定义、数据结构，
*                  使用专用前缀“RP_”。
* Version:      V0.1
* Author:       
* CreateDate:   20230316
*******************************************************************************/
#ifndef __SIA_GLOBAL_OUTER_RGBPROC_H__
#define __SIA_GLOBAL_OUTER_RGBPROC_H__
#include "siaGlobalOuter_DataTypeDef.h"
//#include "siaGlobal.h"

//RP模块内参矩阵长度
#define RP_INTRINSIC_MATRIX_LEN        9

//RP模块旋转矩阵长度
#define RP_ROTATION_MATRIX_LEN         9

//RP模块平移矩阵长度
#define RP_TRANSLATION_MATRIX_LEN      3

//------------------------------------------------------------------------------
//定义算法版本号字符数组长度
#define RP_ARITH_VERSION_LEN_MAX    64 //算法版本号字符数组最大长度

//------------------------------------------------------------------------------
//定义算法初始化成功/失败标识（影响整个算法初始化成败）
//1) 无符号32位（unsigned int），按bit位保存失败标识
enum RP_ARITH_INIT_SUCCESS_FLAG
{
    RP_INIT_SUCCESS = 0x00, //初始化成功
    RP_INIT_FAIL_GLB_BUFFER_SIZE_CALC_ERROR = (0x01 << 0), //初始化失败-buffer空间大小计算异常
    RP_INIT_FAIL_GLB_BUFFER_ALLOCATE = (0x01 << 1), //初始化失败-全局buffer空间分配失败
    RP_INIT_FAIL_INVALID_FIXPARA_POINTER = (0x01 << 2), //初始化失败-无效的输入固定配置参数指针 (NULL == pstFixParas)
    RP_INIT_FAIL_INVALID_PARA_IN_DEPTH_WIDTH = (0x01 << 5), //初始化失败-无效的输入参数: usInDepthWidth
    RP_INIT_FAIL_INVALID_PARA_IN_DEPTH_HEIGHT = (0x01 << 6), //初始化失败-无效的输入参数: usInDepthHeight
    RP_INIT_FAIL_INVALID_PARA_IN_RGB_WIDTH = (0x01 << 7), //初始化失败-无效的输入参数: usInRGBWidth
    RP_INIT_FAIL_INVALID_PARA_IN_RGB_HEIGHT = (0x01 << 8), //初始化失败-无效的输入参数: usInRGBHeight
    RP_INIT_FAIL_INVALID_PARA_OUT_IMG_WIDTH = (0x01 << 19), //初始化失败-无效的输入参数: usOutImgWidth
    RP_INIT_FAIL_INVALID_PARA_OUT_IMG_HEIGHT = (0x01 << 20), //初始化失败-无效的输入参数: usOutImgHeight
    RP_INIT_FAIL_MODULE_INIT_PRECALC = (0x01 << 22), //初始化失败-模块算法初始化和提前计算失败
};

//------------------------------------------------------------------------------
//定义算法执行成功/失败标识
//1) 无符号32位（unsigned int），按bit位保存失败标识
enum RP_ARTIH_SUCCESS_FLAG
{
    RP_ARITH_SUCCESS = 0x00,                            //算法执行成功
    RP_ARITH_FAIL_INVALID_OUTER_BUFFER      = (0x01 << 0), //算法执行失败-无效的外部buffer指针
    RP_ARITH_FAIL_GLB_BUFFER_ALLOCATE       = (0x01 << 1), //初始化失败-全局buffer空间分配失败
    RP_ARITH_FAIL_OUTER_BUFFER_CHANGED      = (0x01 << 2), //算法执行失败-外部buffer地址发生变化
    RP_ARITH_FAIL_INVALID_GLB_FIXVAR_BUFFER = (0x01 << 3), //初始化失败-无效的内部buffer指针: GLB_FIXVAR
    RP_ARITH_FAIL_NOT_INITIALIZED           = (0x01 << 4), //算法执行失败-未初始化
    RP_ARITH_FAIL_INVALID_INOUTDATAS_POINTER = (0x01 << 5), //算法执行失败-无效的算法输入数据结构体指针 (NULL == pstInOutDatas)
    RP_ARITH_FAIL_CALC_POINTCLOUD           = (0x01 << 6), //算法执行失败-点云计算失败
    RP_ARITH_FAIL_ALLOCATE_DYN_BUFFER       = (0x01 << 7), //算法执行失败-动态buffer空间分配失败
};


//定义RP算法执行成功/失败标识
//注：无符号32位（unsigned int），按bit位保存异常标识
enum RP_SUCCESS_FLAG
{
    RP_SUCCESS = 0x00,         //算法执行成功
    RP_FAIL_INVALID_IN_DEPTH_IMG        = (0x01 << 0),  //算法执行失败-无效的输入深度图像指针
    RP_FAIL_INVALID_IN_YUV_IMG          = (0x01 << 1),  //算法执行失败-无效的输入YUV图像指针
    RP_FAIL_INVALID_OUT_RGB_IMG         = (0x01 << 2),  //算法执行失败-无效的算法输出RGB图指针
    RP_FAIL_INVALID_INPARA              = (0x01 << 3),  //算法执行失败-无效的算法输入参数指针 
    RP_FAIL_INVALID_OUTPARA             = (0x01 << 4),  //算法执行失败-无效的算法输出参数指针 
    RP_FAIL_INVALID_GLB_BUFFER          = (0x01 << 5),  //算法执行失败-无效的模块全局buffer
    RP_FAIL_INVALID_GLB_VARS_POINTER    = (0x01 << 6),  //算法执行失败-无效的全局变量结构体指针
    RP_FAIL_INVALID_IN_RGB_IMG_POINTER  = (0x01 << 7),  //算法执行失败-无效的输入RGB图像指针
    RP_FAIL_INVALID_DEPTH_TO_RGB_MAP    = (0x01 << 8),  //算法执行失败-无效的深度到RGB映射图
    RP_FAIL_INVALID_INSIZE              = (0x01 << 9),  //算法执行失败-无效的输入图像尺寸
    RP_FAIL_INVALID_TOF_INTRINSIC_PARAS = (0x01 << 10), //算法执行失败-无效的Tof相机内参矩阵
    RP_FAIL_INVALID_CALIPARA_POINTER    = (0x01 << 11), //算法执行失败-无效的标定参数指针
    RP_FAIL_INVALID_INCALIPARA          = (0x01 << 12), //算法执行失败-无效的输入标定参数指针
    RP_FAIL_INVALID_CALI_FIRSTBYTEID    = (0x01 << 13), //算法执行失败-无效的REG模块起始字节数
    RP_FAIL_INVALID_CALI_MODID          = (0x01 << 14), //算法执行失败-无效的标定文件模块ID
    RP_FAIL_INVALID_CALI_VALIDFLAG      = (0x01 << 15), //算法执行失败-无效的REG模块参数有效标识
    RP_FAIL_DISABLE_REG_FLAG            = (0x01 << 16), //算法执行失败-配准功能未使能标识
    RP_FAIL_CALIPARA_LOAD_FAIL          = (0x01 << 17), //算法执行失败-未成功加载标定参数
    RP_FAIL_INVALID_CALI_CORR_PIXEL_MODE = (0x01 << 18), //算法执行失败-无效的REG标定和校正像素匹配类型
    RP_FAIL_INVALID_CORR_MAP_POINTER    = (0x01 << 19), //算法执行失败-无效的坐标映射表指针

};

//------------------------------------------------------------------------------
//定义RP算法异常标识（仅记录，不影响整个算法执行成败）
//注：无符号32位（unsigned int），按bit位保存异常标识
enum RP_ARITH_ABNORMAL_FLAG
{
    RP_ARITH_NORMAL = 0x00,        //正常（无异常）
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_POINTER       = (0x01 << 0),       //异常-无效的输入动态配置参数指针 (NULL == pstDynParas)
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_TOF_WIDTH  = (0x01 << 1),       //异常-无效的输入动态配置参数-inTOF Width
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_TOF_HEIGHT = (0x01 << 2),       //异常-无效的输入动态配置参数-inTOF Height
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_RGB_WIDTH  = (0x01 << 3),       //异常-无效的输入动态配置参数-inRGB Width
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_RGB_HEIGHT = (0x01 << 4),       //异常-无效的输入动态配置参数-inRGB Height
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_OUT_WIDTH     = (0x01 << 5),       //异常-无效的输入动态配置参数-out Width
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_OUT_HEIGHT    = (0x01 << 6),       //异常-无效的输入动态配置参数-out Height
    RP_ARITH_ABNORMAL_INVALID_INOUTDATAS_DEPTH      = (0x01 << 7),       //异常-无效的算法输入数据-pstDepth
    RP_ARITH_ABNORMAL_INVALID_INOUTDATAS_RGB        = (0x01 << 8),       //异常-无效的算法输入数据-pstRGB
    RP_ARITH_ABNORMAL_FAIL_MALLOC                   = (0x01 << 9),       //异常-临时空间动态分配失败
};


////////////////////////////////////////////////////////////////////////////////
// Structure definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//定义RP算法输入参数结构体
typedef struct RPInputParas
{
    //模块全局buffer相关参数
    void* pThisGlbBuffer; //外部分配好的当前模块的全局空间buffer首地址

    //输入图像
    unsigned char* pucYuvImg;       //输入的YUV图像指针
    unsigned short usYuvWidth;      //输入图像宽度（yuv图）
    unsigned short usYuvHeight;     //输入图像高度（yuv图）

    unsigned short* pusDepth;       //输入的深度图指针
    unsigned short usDepthWidth;    //输入图像宽度（深度图）
    unsigned short usDepthHeight;   //输入图像高度（深度图）

    unsigned char* pucConfidence;   //输入的置信度图像
    unsigned short usConfWidth;     //输入图像宽度（置信度图）
    unsigned short usConfHeight;    //输入图像高度（置信度图）

} RP_INPARAS;


//------------------------------------------------------------------------------
//定义动态配置参数结构体
typedef struct RGBProc_DynamicControlParameters
{
    unsigned short usInDepthWidth;          //输入的ToF宽
    unsigned short usInDepthHeight;         //输入的ToF高
    unsigned short usInYuvWidth;            //输入的RGB宽
    unsigned short usInYuvHeight;           //输入的RGB高

    unsigned short usOutR2DWidth;           //输出的图像宽
    unsigned short usOutR2DHeight;          //输出的图像高

    unsigned short usOutD2RWidth;           //输出的图像宽
    unsigned short usOutD2RHeight;          //输出的图像高

    unsigned int uiOutRGBDLen;

    //是否输出RGB2Depth图像
    unsigned char ucEnableOutR2D;           //默认：0

    //是否输出Depth2RGB图像
    unsigned char ucEnableOutD2R;           //默认：0

    //是否输出RGBD点云图
    unsigned char ucEnableRGBDPCL;          //默认：0

    unsigned char ucThConfidence;           //置信度滤波的参数阈值

} RP_DYNPARA;

//------------------------------------------------------------------------------
//定义RP模块标定参数结构体
typedef struct CaliParas_RP
{
    //标定参数
    float fTofIntrinsicMatrix[RP_INTRINSIC_MATRIX_LEN];     //Tof相机的内参矩阵
    float fRgbIntrinsicMatrix[RP_INTRINSIC_MATRIX_LEN];     //Rgb相机的内参矩阵
    float fRotationMatrix[RP_ROTATION_MATRIX_LEN];          //右相机相对左相机的旋转矩阵
    float fTranslationMatrix[RP_TRANSLATION_MATRIX_LEN];    //右相机相对左相机的平移矩阵
    unsigned char bIsRgbCameraLeft;                         //Rgb相对Tof是否在左边，以人正对传感器表面来定义左右
} CALIPARAS_RP;


//定义RP算法输出图像结构体
typedef struct RPOutputParas
{
    //输出可将光图像映射到深度图像
    unsigned char* pucRgb2DepthImg; //输出RGB图像映射到深度图像指针
    unsigned short usOutR2DWidth;   //输出R2D的图像宽
    unsigned short usOutR2DHeight;  //输出R2D的图像高
    unsigned char  ucEnableOutR2D;  //是否输出R2D的图像


    //输出深度图像映射到可见光图像
    unsigned short* pusDepth2RgbImg;//输出输出深度图像映射到可见光图像指针   
    unsigned short usOutD2RWidth;   //输出D2R的图像宽    
    unsigned short usOutD2RHeight;  //输出D2R的图像高
    unsigned char  ucEnableOutD2R;  //是否输出D2R的图像

    //输出RGBD的点云图像
    RGBD_POINT_CLOUD* pstrRGBD;     //输出RGBD的点云数图像
    unsigned int    uiOutRGBDLen;   //输出RGBD点云的数量
    unsigned char  ucEnableRGBDPCL; //是否输出RGBD点云

} RP_OUTPARAS;

#endif // !__SIA_GLOBAL_OUTER_RGBPROC_H__

