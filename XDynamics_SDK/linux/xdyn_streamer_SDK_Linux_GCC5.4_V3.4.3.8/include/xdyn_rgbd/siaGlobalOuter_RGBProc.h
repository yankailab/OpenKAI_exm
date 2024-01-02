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
* Abstract:     �������㷨�ⲿ���롢�����صĺ궨�塢���ݽṹ�ȡ�
* Description:  1) ���ⲿ���ã�����dllһ�𷢲���
*               2) �����㷨�⡰siaGlobalOuter_RGBProc.h���ڵĺ궨�塢���ݽṹ��
*                  ʹ��ר��ǰ׺��RP_����
* Version:      V0.1
* Author:       
* CreateDate:   20230316
*******************************************************************************/
#ifndef __SIA_GLOBAL_OUTER_RGBPROC_H__
#define __SIA_GLOBAL_OUTER_RGBPROC_H__
#include "siaGlobalOuter_DataTypeDef.h"
//#include "siaGlobal.h"

//RPģ���ڲξ��󳤶�
#define RP_INTRINSIC_MATRIX_LEN        9

//RPģ����ת���󳤶�
#define RP_ROTATION_MATRIX_LEN         9

//RPģ��ƽ�ƾ��󳤶�
#define RP_TRANSLATION_MATRIX_LEN      3

//------------------------------------------------------------------------------
//�����㷨�汾���ַ����鳤��
#define RP_ARITH_VERSION_LEN_MAX    64 //�㷨�汾���ַ�������󳤶�

//------------------------------------------------------------------------------
//�����㷨��ʼ���ɹ�/ʧ�ܱ�ʶ��Ӱ�������㷨��ʼ���ɰܣ�
//1) �޷���32λ��unsigned int������bitλ����ʧ�ܱ�ʶ
enum RP_ARITH_INIT_SUCCESS_FLAG
{
    RP_INIT_SUCCESS = 0x00, //��ʼ���ɹ�
    RP_INIT_FAIL_GLB_BUFFER_SIZE_CALC_ERROR = (0x01 << 0), //��ʼ��ʧ��-buffer�ռ��С�����쳣
    RP_INIT_FAIL_GLB_BUFFER_ALLOCATE = (0x01 << 1), //��ʼ��ʧ��-ȫ��buffer�ռ����ʧ��
    RP_INIT_FAIL_INVALID_FIXPARA_POINTER = (0x01 << 2), //��ʼ��ʧ��-��Ч������̶����ò���ָ�� (NULL == pstFixParas)
    RP_INIT_FAIL_INVALID_PARA_IN_DEPTH_WIDTH = (0x01 << 5), //��ʼ��ʧ��-��Ч���������: usInDepthWidth
    RP_INIT_FAIL_INVALID_PARA_IN_DEPTH_HEIGHT = (0x01 << 6), //��ʼ��ʧ��-��Ч���������: usInDepthHeight
    RP_INIT_FAIL_INVALID_PARA_IN_RGB_WIDTH = (0x01 << 7), //��ʼ��ʧ��-��Ч���������: usInRGBWidth
    RP_INIT_FAIL_INVALID_PARA_IN_RGB_HEIGHT = (0x01 << 8), //��ʼ��ʧ��-��Ч���������: usInRGBHeight
    RP_INIT_FAIL_INVALID_PARA_OUT_IMG_WIDTH = (0x01 << 19), //��ʼ��ʧ��-��Ч���������: usOutImgWidth
    RP_INIT_FAIL_INVALID_PARA_OUT_IMG_HEIGHT = (0x01 << 20), //��ʼ��ʧ��-��Ч���������: usOutImgHeight
    RP_INIT_FAIL_MODULE_INIT_PRECALC = (0x01 << 22), //��ʼ��ʧ��-ģ���㷨��ʼ������ǰ����ʧ��
};

//------------------------------------------------------------------------------
//�����㷨ִ�гɹ�/ʧ�ܱ�ʶ
//1) �޷���32λ��unsigned int������bitλ����ʧ�ܱ�ʶ
enum RP_ARTIH_SUCCESS_FLAG
{
    RP_ARITH_SUCCESS = 0x00,                            //�㷨ִ�гɹ�
    RP_ARITH_FAIL_INVALID_OUTER_BUFFER      = (0x01 << 0), //�㷨ִ��ʧ��-��Ч���ⲿbufferָ��
    RP_ARITH_FAIL_GLB_BUFFER_ALLOCATE       = (0x01 << 1), //��ʼ��ʧ��-ȫ��buffer�ռ����ʧ��
    RP_ARITH_FAIL_OUTER_BUFFER_CHANGED      = (0x01 << 2), //�㷨ִ��ʧ��-�ⲿbuffer��ַ�����仯
    RP_ARITH_FAIL_INVALID_GLB_FIXVAR_BUFFER = (0x01 << 3), //��ʼ��ʧ��-��Ч���ڲ�bufferָ��: GLB_FIXVAR
    RP_ARITH_FAIL_NOT_INITIALIZED           = (0x01 << 4), //�㷨ִ��ʧ��-δ��ʼ��
    RP_ARITH_FAIL_INVALID_INOUTDATAS_POINTER = (0x01 << 5), //�㷨ִ��ʧ��-��Ч���㷨�������ݽṹ��ָ�� (NULL == pstInOutDatas)
    RP_ARITH_FAIL_CALC_POINTCLOUD           = (0x01 << 6), //�㷨ִ��ʧ��-���Ƽ���ʧ��
    RP_ARITH_FAIL_ALLOCATE_DYN_BUFFER       = (0x01 << 7), //�㷨ִ��ʧ��-��̬buffer�ռ����ʧ��
};


//����RP�㷨ִ�гɹ�/ʧ�ܱ�ʶ
//ע���޷���32λ��unsigned int������bitλ�����쳣��ʶ
enum RP_SUCCESS_FLAG
{
    RP_SUCCESS = 0x00,         //�㷨ִ�гɹ�
    RP_FAIL_INVALID_IN_DEPTH_IMG        = (0x01 << 0),  //�㷨ִ��ʧ��-��Ч���������ͼ��ָ��
    RP_FAIL_INVALID_IN_YUV_IMG          = (0x01 << 1),  //�㷨ִ��ʧ��-��Ч������YUVͼ��ָ��
    RP_FAIL_INVALID_OUT_RGB_IMG         = (0x01 << 2),  //�㷨ִ��ʧ��-��Ч���㷨���RGBͼָ��
    RP_FAIL_INVALID_INPARA              = (0x01 << 3),  //�㷨ִ��ʧ��-��Ч���㷨�������ָ�� 
    RP_FAIL_INVALID_OUTPARA             = (0x01 << 4),  //�㷨ִ��ʧ��-��Ч���㷨�������ָ�� 
    RP_FAIL_INVALID_GLB_BUFFER          = (0x01 << 5),  //�㷨ִ��ʧ��-��Ч��ģ��ȫ��buffer
    RP_FAIL_INVALID_GLB_VARS_POINTER    = (0x01 << 6),  //�㷨ִ��ʧ��-��Ч��ȫ�ֱ����ṹ��ָ��
    RP_FAIL_INVALID_IN_RGB_IMG_POINTER  = (0x01 << 7),  //�㷨ִ��ʧ��-��Ч������RGBͼ��ָ��
    RP_FAIL_INVALID_DEPTH_TO_RGB_MAP    = (0x01 << 8),  //�㷨ִ��ʧ��-��Ч����ȵ�RGBӳ��ͼ
    RP_FAIL_INVALID_INSIZE              = (0x01 << 9),  //�㷨ִ��ʧ��-��Ч������ͼ��ߴ�
    RP_FAIL_INVALID_TOF_INTRINSIC_PARAS = (0x01 << 10), //�㷨ִ��ʧ��-��Ч��Tof����ڲξ���
    RP_FAIL_INVALID_CALIPARA_POINTER    = (0x01 << 11), //�㷨ִ��ʧ��-��Ч�ı궨����ָ��
    RP_FAIL_INVALID_INCALIPARA          = (0x01 << 12), //�㷨ִ��ʧ��-��Ч������궨����ָ��
    RP_FAIL_INVALID_CALI_FIRSTBYTEID    = (0x01 << 13), //�㷨ִ��ʧ��-��Ч��REGģ����ʼ�ֽ���
    RP_FAIL_INVALID_CALI_MODID          = (0x01 << 14), //�㷨ִ��ʧ��-��Ч�ı궨�ļ�ģ��ID
    RP_FAIL_INVALID_CALI_VALIDFLAG      = (0x01 << 15), //�㷨ִ��ʧ��-��Ч��REGģ�������Ч��ʶ
    RP_FAIL_DISABLE_REG_FLAG            = (0x01 << 16), //�㷨ִ��ʧ��-��׼����δʹ�ܱ�ʶ
    RP_FAIL_CALIPARA_LOAD_FAIL          = (0x01 << 17), //�㷨ִ��ʧ��-δ�ɹ����ر궨����
    RP_FAIL_INVALID_CALI_CORR_PIXEL_MODE = (0x01 << 18), //�㷨ִ��ʧ��-��Ч��REG�궨��У������ƥ������
    RP_FAIL_INVALID_CORR_MAP_POINTER    = (0x01 << 19), //�㷨ִ��ʧ��-��Ч������ӳ���ָ��

};

//------------------------------------------------------------------------------
//����RP�㷨�쳣��ʶ������¼����Ӱ�������㷨ִ�гɰܣ�
//ע���޷���32λ��unsigned int������bitλ�����쳣��ʶ
enum RP_ARITH_ABNORMAL_FLAG
{
    RP_ARITH_NORMAL = 0x00,        //���������쳣��
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_POINTER       = (0x01 << 0),       //�쳣-��Ч�����붯̬���ò���ָ�� (NULL == pstDynParas)
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_TOF_WIDTH  = (0x01 << 1),       //�쳣-��Ч�����붯̬���ò���-inTOF Width
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_TOF_HEIGHT = (0x01 << 2),       //�쳣-��Ч�����붯̬���ò���-inTOF Height
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_RGB_WIDTH  = (0x01 << 3),       //�쳣-��Ч�����붯̬���ò���-inRGB Width
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_IN_RGB_HEIGHT = (0x01 << 4),       //�쳣-��Ч�����붯̬���ò���-inRGB Height
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_OUT_WIDTH     = (0x01 << 5),       //�쳣-��Ч�����붯̬���ò���-out Width
    RP_ARITH_ABNORMAL_INVALID_DYNPARA_OUT_HEIGHT    = (0x01 << 6),       //�쳣-��Ч�����붯̬���ò���-out Height
    RP_ARITH_ABNORMAL_INVALID_INOUTDATAS_DEPTH      = (0x01 << 7),       //�쳣-��Ч���㷨��������-pstDepth
    RP_ARITH_ABNORMAL_INVALID_INOUTDATAS_RGB        = (0x01 << 8),       //�쳣-��Ч���㷨��������-pstRGB
    RP_ARITH_ABNORMAL_FAIL_MALLOC                   = (0x01 << 9),       //�쳣-��ʱ�ռ䶯̬����ʧ��
};


////////////////////////////////////////////////////////////////////////////////
// Structure definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//����RP�㷨��������ṹ��
typedef struct RPInputParas
{
    //ģ��ȫ��buffer��ز���
    void* pThisGlbBuffer; //�ⲿ����õĵ�ǰģ���ȫ�ֿռ�buffer�׵�ַ

    //����ͼ��
    unsigned char* pucYuvImg;       //�����YUVͼ��ָ��
    unsigned short usYuvWidth;      //����ͼ���ȣ�yuvͼ��
    unsigned short usYuvHeight;     //����ͼ��߶ȣ�yuvͼ��

    unsigned short* pusDepth;       //��������ͼָ��
    unsigned short usDepthWidth;    //����ͼ���ȣ����ͼ��
    unsigned short usDepthHeight;   //����ͼ��߶ȣ����ͼ��

    unsigned char* pucConfidence;   //��������Ŷ�ͼ��
    unsigned short usConfWidth;     //����ͼ���ȣ����Ŷ�ͼ��
    unsigned short usConfHeight;    //����ͼ��߶ȣ����Ŷ�ͼ��

} RP_INPARAS;


//------------------------------------------------------------------------------
//���嶯̬���ò����ṹ��
typedef struct RGBProc_DynamicControlParameters
{
    unsigned short usInDepthWidth;          //�����ToF��
    unsigned short usInDepthHeight;         //�����ToF��
    unsigned short usInYuvWidth;            //�����RGB��
    unsigned short usInYuvHeight;           //�����RGB��

    unsigned short usOutR2DWidth;           //�����ͼ���
    unsigned short usOutR2DHeight;          //�����ͼ���

    unsigned short usOutD2RWidth;           //�����ͼ���
    unsigned short usOutD2RHeight;          //�����ͼ���

    unsigned int uiOutRGBDLen;

    //�Ƿ����RGB2Depthͼ��
    unsigned char ucEnableOutR2D;           //Ĭ�ϣ�0

    //�Ƿ����Depth2RGBͼ��
    unsigned char ucEnableOutD2R;           //Ĭ�ϣ�0

    //�Ƿ����RGBD����ͼ
    unsigned char ucEnableRGBDPCL;          //Ĭ�ϣ�0

    unsigned char ucThConfidence;           //���Ŷ��˲��Ĳ�����ֵ

} RP_DYNPARA;

//------------------------------------------------------------------------------
//����RPģ��궨�����ṹ��
typedef struct CaliParas_RP
{
    //�궨����
    float fTofIntrinsicMatrix[RP_INTRINSIC_MATRIX_LEN];     //Tof������ڲξ���
    float fRgbIntrinsicMatrix[RP_INTRINSIC_MATRIX_LEN];     //Rgb������ڲξ���
    float fRotationMatrix[RP_ROTATION_MATRIX_LEN];          //�����������������ת����
    float fTranslationMatrix[RP_TRANSLATION_MATRIX_LEN];    //���������������ƽ�ƾ���
    unsigned char bIsRgbCameraLeft;                         //Rgb���Tof�Ƿ�����ߣ��������Դ�������������������
} CALIPARAS_RP;


//����RP�㷨���ͼ��ṹ��
typedef struct RPOutputParas
{
    //����ɽ���ͼ��ӳ�䵽���ͼ��
    unsigned char* pucRgb2DepthImg; //���RGBͼ��ӳ�䵽���ͼ��ָ��
    unsigned short usOutR2DWidth;   //���R2D��ͼ���
    unsigned short usOutR2DHeight;  //���R2D��ͼ���
    unsigned char  ucEnableOutR2D;  //�Ƿ����R2D��ͼ��


    //������ͼ��ӳ�䵽�ɼ���ͼ��
    unsigned short* pusDepth2RgbImg;//���������ͼ��ӳ�䵽�ɼ���ͼ��ָ��   
    unsigned short usOutD2RWidth;   //���D2R��ͼ���    
    unsigned short usOutD2RHeight;  //���D2R��ͼ���
    unsigned char  ucEnableOutD2R;  //�Ƿ����D2R��ͼ��

    //���RGBD�ĵ���ͼ��
    RGBD_POINT_CLOUD* pstrRGBD;     //���RGBD�ĵ�����ͼ��
    unsigned int    uiOutRGBDLen;   //���RGBD���Ƶ�����
    unsigned char  ucEnableRGBDPCL; //�Ƿ����RGBD����

} RP_OUTPARAS;

#endif // !__SIA_GLOBAL_OUTER_RGBPROC_H__

