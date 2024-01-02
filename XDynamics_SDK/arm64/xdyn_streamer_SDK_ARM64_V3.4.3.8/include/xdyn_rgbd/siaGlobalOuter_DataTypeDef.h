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
* Abstract:     ����Tof�㷨�ⲿͨ�õ��������͡��ṹ��ȡ�
* Description:  1) ���ⲿ���ã�����dllһ�𷢲���
*               2) ��TofУ���㷨�⡢�����㷨��ȹ��õ��������Ͷ��嵥����ȡ������
*                  Ŀ�ģ����ⲻͬ�㷨��֮����������ض��塣
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
//���������ֵ��
#define TRUE                        1
#define FALSE                       0


//------------------------------------------------------------------------------
//����ͼ������
//���ֵ����
enum DEPTH_VALUE_TYPE
{
    DEPTH_VALUE_XYR = 0,            //���ֵ���� - б��
    DEPTH_VALUE_XYZ,                //���ֵ���� - �ռ�����(X,Y,Z)�е�Zֵ
};


//�����������
enum DEPTH_DATA_TYPE
{
    DEPTH_TYPE_UINT16 = 0,          //����������� - unsigned short��16bit���ֵ��
    DEPTH_TYPE_DEPTH16,             //����������� - DEPTH16�����DEPTH16�ṹ�嶨�壩
};


//������������
enum POINTCLOUD_DATA_TYPE
{
    PC_TYPE_PC32F = 0,              //������������ - PC32F�����PC32F�ṹ�嶨�壩
    PC_TYPE_DEPTH_POINT_CLOUD,      //������������ - DEPTH_POINT_CLOUD�����DEPTH_POINT_CLOUD�ṹ�嶨�壩
    PC_TYPE_RGBD_POINT_CLOUD,       //������������ - RGBD_POINT_CLOUD�����RGBD_POINT_CLOUD�ṹ�嶨�壩
};


//���ͼ�������
enum OUT_IMAGE_NAME
{
    OIMG_NAME_POINTCLOUD = 0,       //���ͼ������ - ����ͼ
    OIMG_NAME_DEPTH,                //���ͼ������ - ���ͼ
    OIMG_NAME_AMP,                  //���ͼ������ - ����ͼ
    OIMG_NAME_GRAY,                 //���ͼ������ - �Ҷ�ͼ
    OIMG_NAME_BG,                   //���ͼ������ - ����ͼ
    OIMG_NAME_FLAG,                 //���ͼ������ - ���ͼ
    OIMG_NAME_CONFIDENCE,           //���ͼ������ - ���Ŷ�ͼ
    OIMG_NAME_REG_RGB,              //���ͼ������ - ��׼��RGBͼ
    OIMG_NAME_RGB,                  //���ͼ������ - ԭʼ��RGBͼ
    OIMG_NAME_RP_RGB2DEPTH,         //���ͼ������ - �ɽ���ͼ��ӳ�䵽���ͼ��
    OIMG_NAME_RP_DEPTH2RGB,         //���ͼ������ - ���ͼ��ӳ�䵽�ɽ���ͼ��
    OIMG_NAME_RP_RGBD_POINT_CLOUD,  //���ͼ������ - RGB��׼��ĵ���ͼ
};


//���ͼ�����������
enum OUT_IMAGE_DATA_TYPE
{
    OIMG_DTYPE_UINT8 = 0,           //���ͼ����������� - unsigned char
    OIMG_DTYPE_UINT16,              //���ͼ����������� - unsigned short
    OIMG_DTYPE_FLOAT32,             //���ͼ����������� - float
    OIMG_DTYPE_DEPTH16,             //���ͼ����������� - DEPTH16
    OIMG_DTYPE_PC32F,               //���ͼ����������� - PC32F
    OIMG_DTYPE_DEPTH_POINT_CLOUD,   //���ͼ����������� - DEPTH_POINT_CLOUD
    OIMG_DTYPE_FLAG,                //���ͼ����������� - CORR_FLAG_PIXEL
    OIMG_DTYPE_RGB,                 //���ͼ����������� - RGB
};


//�������ͼ��buffer���䷽ʽ
enum OUT_IMAGE_BUFFER_TYPE
{
    OIMG_BUFFER_NULL = 0,           //���ͼ��buffer���䷽ʽ - �ޣ����㷨�������ͼ��
    OIMG_BUFFER_USER,               //���ͼ��buffer���䷽ʽ - ���û�����
    OIMG_BUFFER_INNER,              //���ͼ��buffer���䷽ʽ - ���㷨�ڲ�����
};


////////////////////////////////////////////////////////////////////////////////
// Structure definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//��������������ͽṹ�� - DEPTH16
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
//2) ��������ǰ׺Ϊ��d16��������DEPTH16 d16Depth;
//
typedef struct DepthDataType_Android
{
    unsigned short usDepth      : 13; //Unit: mm. Scale: 0~8191mm
    unsigned short usConfidence : 3;
} DEPTH16;


//�������ͼ����������
union UNION_DEPTH_DATA_TYPE
{
    unsigned short usDepth;
    DEPTH16 stDepth;
};


//------------------------------------------------------------------------------
//��������������ͽṹ�� - PC32F
//1) ��������ǰ׺Ϊ��pc��������PC32F pcPnt;
typedef struct PointCloud_Float32
{
    float fX; //x coordinate
    float fY; //y coordinate
    float fZ; //z coordinate
} PC32F;


//��������������ͽṹ�� - DEPTH_POINT_CLOUD
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
//2) ��������ǰ׺Ϊ��dpc��������DEPTH_POINT_CLOUD dpcPnt;
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

