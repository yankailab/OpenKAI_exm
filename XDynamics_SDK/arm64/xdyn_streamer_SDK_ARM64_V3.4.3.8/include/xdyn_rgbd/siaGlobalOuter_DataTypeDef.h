/***************************Copyright@SI****************************************
*
* IMPORTANT:    READ BEFORE DOWNLOADING, COPYING OR USING.
*
*   By downloading, copying or using the software you agree to this license.
*   If you do not agree to this license, do not download, copy or use the software.
*
* License Agreement:
*
*   Copyright (C) 2021, Wuhan Silicon Integrated Co., Ltd. All rights reserved.
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
* FileName:     siaGlobalOuter_DataTypeDef.h
* Abstract:     定义Tof算法外部通用的数据类型、结构体等。
* Description:  1) 可外部调用，可随dll一起发布。
*               2) 将Tof校正算法库、后处理算法库等共用的数据类型定义单独提取出来。
*                  目的：避免不同算法库之间出现类型重定义。
* Version:      V1.0
* Author:       MSSu
* CreateDate:   20210416
*
* Modified:     Wuhao, 20210927
*  Version:     V1.1
*  Content:     1) enum OUT_IMAGE_NAME: add OIMG_NAME_REG_RGB.
*               2) enum OUT_IMAGE_DATA_TYPE: add OIMG_DTYPE_RGB.
*******************************************************************************/
#ifndef __SIA_GLOBAL_OUTER_DATA_TYPE_DEF_H__
#define __SIA_GLOBAL_OUTER_DATA_TYPE_DEF_H__


////////////////////////////////////////////////////////////////////////////////
// Macro definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//定义基本数值宏
#define TRUE                        1
#define FALSE                       0


//------------------------------------------------------------------------------
//定义图像类型
//深度值类型
enum DEPTH_VALUE_TYPE
{
    DEPTH_VALUE_XYR = 0,            //深度值类型 - 斜距
    DEPTH_VALUE_XYZ,                //深度值类型 - 空间坐标(X,Y,Z)中的Z值
};


//深度数据类型
enum DEPTH_DATA_TYPE
{
    DEPTH_TYPE_UINT16 = 0,          //深度数据类型 - unsigned short（16bit深度值）
    DEPTH_TYPE_DEPTH16,             //深度数据类型 - DEPTH16（详见DEPTH16结构体定义）
};


//点云数据类型
enum POINTCLOUD_DATA_TYPE
{
    PC_TYPE_PC32F = 0,              //点云数据类型 - PC32F（详见PC32F结构体定义）
    PC_TYPE_DEPTH_POINT_CLOUD,      //点云数据类型 - DEPTH_POINT_CLOUD（详见DEPTH_POINT_CLOUD结构体定义）
    PC_TYPE_RGBD_POINT_CLOUD,       //点云数据类型 - RGBD_POINT_CLOUD（详见RGBD_POINT_CLOUD结构体定义）
};


//输出图像的名称
enum OUT_IMAGE_NAME
{
    OIMG_NAME_POINTCLOUD = 0,       //输出图像名称 - 点云图
    OIMG_NAME_DEPTH,                //输出图像名称 - 深度图
    OIMG_NAME_AMP,                  //输出图像名称 - 幅度图
    OIMG_NAME_GRAY,                 //输出图像名称 - 灰度图
    OIMG_NAME_BG,                   //输出图像名称 - 背景图
    OIMG_NAME_FLAG,                 //输出图像名称 - 标记图
    OIMG_NAME_CONFIDENCE,           //输出图像名称 - 置信度图
    OIMG_NAME_REG_RGB,              //输出图像名称 - 配准后RGB图
    OIMG_NAME_RGB,                  //输出图像名称 - 原始的RGB图
    OIMG_NAME_RP_RGB2DEPTH,         //输出图像名称 - 可将光图像映射到深度图像
    OIMG_NAME_RP_DEPTH2RGB,         //输出图像名称 - 深度图像映射到可将光图像
    OIMG_NAME_RP_RGBD_POINT_CLOUD,  //输出图像名称 - RGB配准后的点云图
};


//输出图像的数据类型
enum OUT_IMAGE_DATA_TYPE
{
    OIMG_DTYPE_UINT8 = 0,           //输出图像的数据类型 - unsigned char
    OIMG_DTYPE_UINT16,              //输出图像的数据类型 - unsigned short
    OIMG_DTYPE_FLOAT32,             //输出图像的数据类型 - float
    OIMG_DTYPE_DEPTH16,             //输出图像的数据类型 - DEPTH16
    OIMG_DTYPE_PC32F,               //输出图像的数据类型 - PC32F
    OIMG_DTYPE_DEPTH_POINT_CLOUD,   //输出图像的数据类型 - DEPTH_POINT_CLOUD
    OIMG_DTYPE_FLAG,                //输出图像的数据类型 - CORR_FLAG_PIXEL
    OIMG_DTYPE_RGB,                 //输出图像的数据类型 - RGB
};


//定义输出图像buffer分配方式
enum OUT_IMAGE_BUFFER_TYPE
{
    OIMG_BUFFER_NULL = 0,           //输出图像buffer分配方式 - 无，即算法不输出该图像
    OIMG_BUFFER_USER,               //输出图像buffer分配方式 - 由用户分配
    OIMG_BUFFER_INNER,              //输出图像buffer分配方式 - 由算法内部分配
};


////////////////////////////////////////////////////////////////////////////////
// Structure definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//定义深度数据类型结构体 - DEPTH16
//
//1) Notice:
//  DEPTH16: Android dense depth image format.
//
//  Each pixel is 16 bits, representing a depth ranging measurement from a depth
//  camera or similar sensor. 
//  The 16-bit sample consists of a confidence value and the actual ranging
//  measurement.
//  The confidence value is encoded in the high 3-bits (bit15~bit13), with a
//  value of 0 representing 100% confidence, a value of 1 representing 0%
//  confidence, a value of 2 representing 1/7, a value of 3 representing 2/7,
//  and so on.
//
//  Example for extracts the range and confidence from a pixel of a DEPTH16
//  format image:
//  Method 1:
//      unsigned short usThisDepth = pusDepth[i];
//      unsigned short usDepth = usThisDepth & 0x1FFF;
//      unsigned short usConfidence = (usThisDepth >> 13) & 0x7;
//      float fConfidence = (0 == usConfidence) ? (1) : ((usConfidence - 1) / 7.0f);
//  Method 2:
//      DEPTH16* pstDepth = (DEPTH16*)pusDepth;
//      DEPTH16 stThisDepth = pstDepth[i];
//      unsigned short usDepth = stThisDepth.usDepth;
//      unsigned short usConfidence = stThisDepth.usConfidence;
//      float fConfidence = (0 == usConfidence) ? (1) : ((usConfidence - 1) / 7.0f);
//
//2) 变量类型前缀为“d16”，例：DEPTH16 d16Depth;
//
typedef struct DepthDataType_Android
{
    unsigned short usDepth      : 13; //Unit: mm. Scale: 0~8191mm
    unsigned short usConfidence : 3;
} DEPTH16;


//定义深度图的像素类型
union UNION_DEPTH_DATA_TYPE
{
    unsigned short usDepth;
    DEPTH16 stDepth;
};


//------------------------------------------------------------------------------
//定义点云数据类型结构体 - PC32F
//1) 变量类型前缀为“pc”，例：PC32F pcPnt;
typedef struct PointCloud_Float32
{
    float fX; //x coordinate
    float fY; //y coordinate
    float fZ; //z coordinate
} PC32F;


//定义点云数据类型结构体 - DEPTH_POINT_CLOUD
//
//1) Notice:
//  DEPTH_POINT_CLOUD: Android sparse depth point cloud format.
//
//  A variable-length list of 3D points plus a confidence value, with each point
//  represented by four floats; first the X, Y, Z position coordinates, and then
//  the confidence value.
//  The number of points is(size of the buffer in bytes) / 16.
//  The coordinate system and units of the position values depend on the source
//  of the point cloud data.
//  The confidence value is between 0.0f and 1.0f, inclusive, with 0
//  representing 0% confidence and 1.f representing 100% confidence in the
//  measured position values.
//
//2) 变量类型前缀为“dpc”，例：DEPTH_POINT_CLOUD dpcPnt;
//
typedef struct PointCloudCoordinate_Android
{
    float fX; //x coordinate
    float fY; //y coordinate
    float fZ; //z coordinate
    float fConfidence; //confidence. Range: 0~1. (0-confidence 0%, 1-confidence 100%)
} DEPTH_POINT_CLOUD;


typedef struct PointCloudCoordinate_Rgb
{
    float fX;        //x coordinate
    float fY;        //y coordinate
    float fZ;        //z coordinate
    unsigned char r; // red color
    unsigned char g; // green color
    unsigned char b; // blue color
} RGBD_POINT_CLOUD;


#endif // !__SIA_GLOBAL_OUTER_DATA_TYPE_DEF_H__

