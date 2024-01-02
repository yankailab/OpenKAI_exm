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
* FileName:     sialib_TofRGBProc.h
* Abstract:     ToF��RGBD�㷨��ӿ��ļ�
* Description:  ToF��RGBD�㷨�⡣
*               ��sialib_TofRGBProc���ڵĽӿں�����ʹ��ר��ǰ׺��sitrp����
* Version:      V1.0
* Author:       
* CreateDate:   20230316
*******************************************************************************/
#ifndef __SIALIB_TOF_RGBPROC_H__
#define __SIALIB_TOF_RGBPROC_H__
#include "siaGlobalOuter_RGBProc.h"


////////////////////////////////////////////////////////////////////////////////
// Macro definition
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//����TOF�㷨���ӿ�ӿ�������ⲿ���õĺ�
//ע�������ⲿ���õĺ����ӿ�ǰ��Ӧ���ϸúꡣ
//    ���磺TOFARITH_EXPORT void TofArithMain(void);
//#ifdef TOFARITH_EXPORTS
#ifdef  _WIN32
#define TOFARITH_EXPORTS extern "C" __declspec(dllexport)
//#elif defined (_LINUX)||(__linux__)  //����ohos��ȥ����Linux��־
#else
#define TOFARITH_EXPORTS extern "C" //MSSu��20200516��Windows�±���ɹ��������ñ��� 
#endif
//#else  
//#define TOFARITH_EXPORTS __declspec(dllimport)   
//#endif  


////////////////////////////////////////////////////////////////////////////////
//
// Function declaration ���ⲿ�ɵ��� - ���������̽ӿڣ�
//
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
* FunctionName: sitrpInit()
* Abstract:     �㷨��ʼ��
* InputParas:   CALIPARAS_RP* pstCaliPara -- �궨�����ṹ��
*               RP_DYNPARA* pstDynParas -- ��̬���������ýṹ��
*               unsigned char ucEnableLogFile
*                   -- �Ƿ����ɳ�ʼ��log�ļ��Ŀ��ء� FALSE(0)-No, TRUE(1)-Yes.
*               unsigned char ucEnableCostTimeLogFile
*                   -- �Ƿ����ɺ�ʱͳ��log�ļ��Ŀ��ء� FALSE(0)-No, TRUE(1)-Yes.
* OutputParas:  unsigned int* puiSuccFlag -- �㷨��ʼ���ɰܱ�ʶ
* Return:       void* -- ȫ�ֿռ�bufferָ��
*
*                   A. �㷨�ڳ�ʼ��ʱ(��������)���Զ�����̶���ȫ��buffer�ռ䣬
*                      ���ڳ�ʼ������ʱ��������ÿռ��bufferָ�뷵�ظ��ⲿ��
*                      ���ⲿ���б��档
*                      �����ִ��ÿ֡�㷨ʱ������buffer������㷨��
*                   B. ����ʼ��ʧ�ܣ��򷵻ص�bufferָ�뽫����ΪNULL��
*
* Description:  1) ���÷�ʽ:
*                  �����㷨�ⲿ����һ��ȫ�ֵ�bufferָ��:
*                       void* pGlbBuffer = NULL;
*                       pGlbBuffer = sitrpInit(...);
*                  �ڵ��������㷨�ӿ�ʱ������bufferָ�봫���㷨:
*                       sitrpSetDynamicConfig(pGlbBuffer, ...);
*                       sitrpRunRGBProcess(pGlbBuffer, ...);
*                       sitrpRelease(&pGlbBuffer, ...);
*
*               2) ��ucEnableLogFile == TRUE�����Զ����㷨��ʼ���ɰܵ�log��Ϣ
*                  ��ӡ���ļ���sialib_tofRGBproc_init_log.txt����
*
*               3) ��ucEnableCostTimeLogFile == TRUE�����Զ��� �����������̵�
*                  �㷨ִ�к�ʱ��Ϣ��ӡ����Ӧ�ĺ�ʱͳ��log�ļ��С�
*                  ���У�
*                       sitrpInit()
*                       sitrpSetDynamicConfig()
*                       sitrpRunRGBProcess()
*                  ����ĺ�ʱͳ��log�ļ��ֱ�Ϊ
*                       ��sialib_tofRGBproc_init_costtime_log.txt��
*                       ��sialib_tofRGBproc_dynconfig_costtime_log.txt��
*                       ��sialib_tofRGBproc_run_costtime_log.txt��
*
*               4) puiSuccFlag:
*                   Range: 0-��ʼ���ɹ�, >0-��ʼ��ʧ��(��bitλ����ʧ��ԭ��)��
*                   ���ö�����͡�PP_ARTIH_SUCCESS_FLAG���Ķ��塣
*
*                   ע������ʼ��ʧ�ܣ���Ӱ�죨���жϣ������㷨��ִ�С�
*                       ��ˣ�����ݷ�����ʧ��ԭ�򣬽��У������ȣ��޸���
*******************************************************************************/
TOFARITH_EXPORTS void* sitrpInit(unsigned int* puiInitSuccFlag, CALIPARAS_RP* pstCaliPara, 
    RP_DYNPARA* pstDynParas, unsigned char ucEnableLogFile, unsigned char ucEnableCostTimeLogFile);


/*******************************************************************************
* FunctionName: sitrpSetDynamicConfig()
* Abstract:     ���ö�̬���ò���
* InputParas:   void* pGlbBuffer -- ȫ�ֿռ�bufferָ��
*               PP_DYNPARA* pstDynParas -- ��̬���ò����ṹ��ָ��
*               unsigned char ucEnableLogFile -- �Ƿ����ɶ�̬����log�ļ��Ŀ��ء�
*                                                FALSE(0)-No, TRUE(1)-Yes.
* 
* OutputParas:  unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ
*               unsigned int* puiAbnormalFlag -- �㷨ִ���쳣��ʶ
* Return:       none
* Description:  1) pGlbBuffer:
*                   ΪsitrpInit()���ص�ȫ��bufferָ�롣
*                   ��Ϊ����̬���ø���ʱ�����л����㷨ģ��Ŀ��ء�
*                   ��ˣ�Ϊ�˱���δ�������㷨ģ��ռ���ڴ棬��ģ��� ��̬ȫ�ֿռ�
*                         ���ڴ˺����ڵ��������ڴ����(����/�ͷ�)��
*
*               2) puiSuccFlag:
*                   Range: 0-ִ�гɹ�, >0-ִ��ʧ��(��bitλ����ʧ��ԭ��)��
*                   ���ö�����͡�PP_ARTIH_SUCCESS_FLAG���Ķ��塣
*
*               3) puiAbnormalFlag:
*                   Range: 0-ִ������, >0-ִ���쳣(��bitλ�����쳣ԭ��)��
*                   ���ö�����͡�PP_ARITH_ABNORMAL_FLAG���Ķ��塣
*
*               4) ��ucEnableLogFile == TRUE�����Զ�����̬���ò������óɰܵ�log
*                  ��Ϣ��ӡ���ļ���sialib_tofRGBproc_dynconfig_log.txt����
*
*               5) ���������㷨��ʼ���󡢵�1֡�㷨��ʼִ��֮ǰ��Ӧ����1�Ρ�
*
*               6) ��Ҫ�ڸ��¶�̬���ã����ڸ�֡�㷨��ʼִ��֮ǰ�����ñ��������и��¡�
********************************************************************************/
TOFARITH_EXPORTS void sitrpSetDynamicConfig(void* pGlbBuffer,
    RP_DYNPARA* pstDynParas, unsigned int* puiSuccFlag,
    unsigned int* puiAbnormalFlag, unsigned char ucEnableLogFile);


/*******************************************************************************
* FunctionName: sitrpRunRGBProcess()
* Abstract:     ִ��Tof��Rgb��sensor��׼�㷨
* InputParas:   void* pGlbBuffer -- ȫ�ֿռ�bufferָ��
*               RP_INPARAS* pstInDatas -- �㷨�������ݽṹ��ָ��
*               unsigned char ucEnableLogFile -- �Ƿ����ɶ�̬����log�ļ��Ŀ��ء�
*                                                FALSE(0)-No, TRUE(1)-Yes.
* OutputParas:  RP_OUTPARAS* pstOutDatas -- �㷨������ݽṹ��ָ��
*               unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ
*               unsigned int* puiAbnormalFlag -- �㷨ִ���쳣��ʶ
* Return:       none
* Description:  1) pGlbBuffer:
*                   ΪsitppInit()���ص�ȫ��bufferָ�롣
*
*               2) ppuiSuccFlag:
*                   Range: 0-ִ�гɹ�, >0-ִ��ʧ��(��bitλ����ʧ��ԭ��)��
*                   ���ö�����͡�RP_ARTIH_SUCCESS_FLAG���Ķ��塣
*
*               3) puiAbnormalFlag:
*                   Range: 0-ִ������, >0-ִ���쳣(��bitλ�����쳣ԭ��)��
*                   ���ö�����͡�RP_ARITH_ABNORMAL_FLAG���Ķ��塣
*
*               4) ��ucEnableLogFile == TRUE�����Զ�����̬���ò������óɰܵ�log
*                  ��Ϣ��ӡ���ļ���sialib_tofRGBproc_run_log.txt����
*
*               5) ������Ϊ�㷨����ں�����ÿ֡����һ�Ρ�
*
*******************************************************************************/
TOFARITH_EXPORTS void sitrpRunRGBProcess(void* pGlbBuffer,
    RP_INPARAS* pstInDatas, RP_OUTPARAS* pstOutDatas, unsigned int* puiSuccFlag,
    unsigned int* puiAbnormalFlag, unsigned char ucEnableLogFile);


/*******************************************************************************
* FunctionName: sitrpRelease()
* Abstract:     �ͷ��㷨buffer
* InputParas:   void** pGlbBuffer -- ȫ�ֿռ�bufferָ��
*               unsigned char ucEnableLogFile -- �Ƿ�����log�ļ��Ŀ��ء�
*                                                FALSE-�� TRUE-�ǡ�
* OutputParas:  void** pGlbBuffer -- ȫ�ֿռ�bufferָ��
* Return:       none
* Description:  1) pGlbBuffer:
*                   ΪsitppInit()���ص�ȫ��bufferָ�롣
*
*               2) ���������һ֡�󣬵��ô˺������ͷ��㷨ʹ�õ�ȫ��buffer�ռ䡣
*                  ע��Ӧ�ڶ��㷨�����������յ��ý��������ͷš�
*                      �����ͷź��������㷨�������������³������
*
*               3) ��ucEnableLogFile == TRUE�����Զ����㷨�ͷųɹ���log��Ϣ
*                  ��ӡ���ļ���sialib_tofRGBproc_release_log.txt����
*******************************************************************************/
TOFARITH_EXPORTS void sitrpRelease(void** pGlbBuffer,
    unsigned char ucEnableLogFile);


////////////////////////////////////////////////////////////////////////////////
//
// Function declaration ���ⲿ�ɵ��� - �ӹ��ܽӿڣ�
//
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
* FunctionName: sitrpGetVersion()
* Abstract:     ��ȡ�㷨�汾��
* InputParas:   none
* OutputParas:  char* -- �㷨�汾���ַ���ָ��
* Return:       none
* Description:  1) ����ʱ�����ⲿ����Ϊchar* pcDllVersion�����ַ����顣
*                  ����
*                       char cDllVerion[CORR_ARITH_VERSION_LEN_MAX] = {0};
*                  �ٵ��ô˺�������ȡ�㷨�汾�ţ�
*                       sitcorrGetVersion(cDllVerion);
*
*               2) ������� pcDllVersion ΪNULL��
*                  ���Զ����汾����Ϣ��ӡ���ļ���sialib_tofRGBproc_version.txt����
*
* Modified:     none
*******************************************************************************/
TOFARITH_EXPORTS void sitrpGetVersion(char* pcDllVersion);


/*******************************************************************************
* FunctionName: sitrpSetTofIntrinsicMat()
* Abstract:     ��ȡToF�궨����
* InputParas:   float* pfInitri -- Tof���ڲ�ָ��
*               int iInitriLen -- Tof���ڲ����鳤��
*               unsigned char ucEnableLogFile -- �Ƿ�����log�ļ��Ŀ��ء�
*                                                FALSE-�� TRUE-�ǡ�
* OutputParas:  float* pfOutBuffer -- ����ڲ�ָ��               
*               unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ

* Return:       unsigned char  �����ɹ���ʶ
* Description:  none
*******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetTofIntrinsicMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

/*******************************************************************************
* FunctionName: sitrpSetRgbIntrinsicMat()
* Abstract:     ��ȡRGB�궨����
* InputParas:   float* pfInitri -- Rgb���ڲ�ָ��
*               int iInitriLen -- Rgb���ڲ����鳤��
*               unsigned char ucEnableLogFile -- �Ƿ�����log�ļ��Ŀ��ء�
*                                                FALSE-�� TRUE-�ǡ�
* OutputParas:  float* pfOutBuffer -- ����ڲ�ָ��
*               unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ
*
* Return:       unsigned char  �����ɹ���ʶ
* Description:  none
*******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetRgbIntrinsicMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);


/*******************************************************************************
* FunctionName: sitrpSetTranslationMat()
* Abstract:     ��ȡTof��RG��ƽ�Ʋ���
* InputParas :  float* pfInitri -- Rgb���ڲ�ָ��
*               int iInitriLen -- Rgb���ڲ����鳤��
*               unsigned char ucEnableLogFile -- �Ƿ�����log�ļ��Ŀ��ء�
*                       FALSE - �� TRUE - �ǡ�
* OutputParas : float* pfOutBuffer -- ����ڲ�ָ��
*               unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ
*
* Return:       unsigned char  �����ɹ���ʶ
* Description:  none
*******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetTranslationMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

/*******************************************************************************
*FunctionName: sitrpSetRotationMat()
* Abstract : ��ȡTof��RG����ת����
* InputParas :  float* pfInitri -- Rgb���ڲ�ָ��
*               int iInitriLen -- Rgb���ڲ����鳤��
*               unsigned char ucEnableLogFile -- �Ƿ�����log�ļ��Ŀ��ء�
*                       FALSE - �� TRUE - �ǡ�
* OutputParas : float* pfOutBuffer -- ����ڲ�ָ��
*               unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ
*
* Return : unsigned char  �����ɹ���ʶ
* Description : none
* ******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetRotationMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

/*******************************************************************************
*FunctionName: sitrpSetRgbPos()
* Abstract : ��ȡTof��RG�����λ�ò���
* InputParas :  float* pfInitri -- Rgb���ڲ�ָ��
*               int iInitriLen -- Rgb���ڲ����鳤��
*               unsigned char ucEnableLogFile -- �Ƿ�����log�ļ��Ŀ��ء�
*                       FALSE - �� TRUE - �ǡ�
* OutputParas : float* pfOutBuffer -- ����ڲ�ָ��
*               unsigned int* puiSuccFlag -- �㷨�ɰܱ�ʶ
*
* Return : unsigned char  �����ɹ���ʶ
* Description : none
* ******************************************************************************/

TOFARITH_EXPORTS unsigned char sitrpSetRgbPos(unsigned char* pBOutBuffer, unsigned char bIsLeft, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

#endif


