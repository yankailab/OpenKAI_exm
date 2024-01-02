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
* Abstract:     ToF的RGBD算法库接口文件
* Description:  ToF的RGBD算法库。
*               “sialib_TofRGBProc”内的接口函数，使用专用前缀“sitrp”。
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
//定义TOF算法链接库接口允许给外部调用的宏
//注：允许外部调用的函数接口前，应加上该宏。
//    例如：TOFARITH_EXPORT void TofArithMain(void);
//#ifdef TOFARITH_EXPORTS
#ifdef  _WIN32
#define TOFARITH_EXPORTS extern "C" __declspec(dllexport)
//#elif defined (_LINUX)||(__linux__)  //兼容ohos，去掉了Linux标志
#else
#define TOFARITH_EXPORTS extern "C" //MSSu，20200516：Windows下编译成功，但调用报错 
#endif
//#else  
//#define TOFARITH_EXPORTS __declspec(dllimport)   
//#endif  


////////////////////////////////////////////////////////////////////////////////
//
// Function declaration （外部可调用 - 主调用流程接口）
//
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
* FunctionName: sitrpInit()
* Abstract:     算法初始化
* InputParas:   CALIPARAS_RP* pstCaliPara -- 标定参数结构体
*               RP_DYNPARA* pstDynParas -- 动态参数的设置结构体
*               unsigned char ucEnableLogFile
*                   -- 是否生成初始化log文件的开关。 FALSE(0)-No, TRUE(1)-Yes.
*               unsigned char ucEnableCostTimeLogFile
*                   -- 是否生成耗时统计log文件的开关。 FALSE(0)-No, TRUE(1)-Yes.
* OutputParas:  unsigned int* puiSuccFlag -- 算法初始化成败标识
* Return:       void* -- 全局空间buffer指针
*
*                   A. 算法在初始化时(本函数内)，自动分配固定的全局buffer空间，
*                      并在初始化结束时，将分配好空间的buffer指针返回给外部，
*                      由外部进行保存。
*                      随后再执行每帧算法时，将该buffer传入给算法。
*                   B. 若初始化失败，则返回的buffer指针将被置为NULL。
*
* Description:  1) 调用方式:
*                  需在算法外部定义一个全局的buffer指针:
*                       void* pGlbBuffer = NULL;
*                       pGlbBuffer = sitrpInit(...);
*                  在调用其它算法接口时，将该buffer指针传入算法:
*                       sitrpSetDynamicConfig(pGlbBuffer, ...);
*                       sitrpRunRGBProcess(pGlbBuffer, ...);
*                       sitrpRelease(&pGlbBuffer, ...);
*
*               2) 若ucEnableLogFile == TRUE，则自动将算法初始化成败等log信息
*                  打印到文件“sialib_tofRGBproc_init_log.txt”。
*
*               3) 若ucEnableCostTimeLogFile == TRUE，则自动将 各主调用流程的
*                  算法执行耗时信息打印到相应的耗时统计log文件中。
*                  其中：
*                       sitrpInit()
*                       sitrpSetDynamicConfig()
*                       sitrpRunRGBProcess()
*                  输出的耗时统计log文件分别为
*                       “sialib_tofRGBproc_init_costtime_log.txt”
*                       “sialib_tofRGBproc_dynconfig_costtime_log.txt”
*                       “sialib_tofRGBproc_run_costtime_log.txt”
*
*               4) puiSuccFlag:
*                   Range: 0-初始化成功, >0-初始化失败(按bit位保存失败原因)。
*                   详见枚举类型“PP_ARTIH_SUCCESS_FLAG”的定义。
*
*                   注：若初始化失败，将影响（或中断）后续算法的执行。
*                       因此，需根据反馈的失败原因，进行（参数等）修复。
*******************************************************************************/
TOFARITH_EXPORTS void* sitrpInit(unsigned int* puiInitSuccFlag, CALIPARAS_RP* pstCaliPara, 
    RP_DYNPARA* pstDynParas, unsigned char ucEnableLogFile, unsigned char ucEnableCostTimeLogFile);


/*******************************************************************************
* FunctionName: sitrpSetDynamicConfig()
* Abstract:     设置动态配置参数
* InputParas:   void* pGlbBuffer -- 全局空间buffer指针
*               PP_DYNPARA* pstDynParas -- 动态配置参数结构体指针
*               unsigned char ucEnableLogFile -- 是否生成动态配置log文件的开关。
*                                                FALSE(0)-No, TRUE(1)-Yes.
* 
* OutputParas:  unsigned int* puiSuccFlag -- 算法成败标识
*               unsigned int* puiAbnormalFlag -- 算法执行异常标识
* Return:       none
* Description:  1) pGlbBuffer:
*                   为sitrpInit()返回的全局buffer指针。
*                   因为：动态配置更新时，会切换各算法模块的开关。
*                   因此，为了避免未开启的算法模块占用内存，各模块的 动态全局空间
*                         将在此函数内单独进行内存管理(分配/释放)。
*
*               2) puiSuccFlag:
*                   Range: 0-执行成功, >0-执行失败(按bit位保存失败原因)。
*                   详见枚举类型“PP_ARTIH_SUCCESS_FLAG”的定义。
*
*               3) puiAbnormalFlag:
*                   Range: 0-执行正常, >0-执行异常(按bit位保存异常原因)。
*                   详见枚举类型“PP_ARITH_ABNORMAL_FLAG”的定义。
*
*               4) 若ucEnableLogFile == TRUE，则自动将动态配置参数设置成败等log
*                  信息打印到文件“sialib_tofRGBproc_dynconfig_log.txt”。
*
*               5) 本函数在算法初始化后、第1帧算法开始执行之前，应调用1次。
*
*               6) 若要在更新动态配置，需在该帧算法开始执行之前，调用本函数进行更新。
********************************************************************************/
TOFARITH_EXPORTS void sitrpSetDynamicConfig(void* pGlbBuffer,
    RP_DYNPARA* pstDynParas, unsigned int* puiSuccFlag,
    unsigned int* puiAbnormalFlag, unsigned char ucEnableLogFile);


/*******************************************************************************
* FunctionName: sitrpRunRGBProcess()
* Abstract:     执行Tof和Rgb的sensor配准算法
* InputParas:   void* pGlbBuffer -- 全局空间buffer指针
*               RP_INPARAS* pstInDatas -- 算法输入数据结构体指针
*               unsigned char ucEnableLogFile -- 是否生成动态配置log文件的开关。
*                                                FALSE(0)-No, TRUE(1)-Yes.
* OutputParas:  RP_OUTPARAS* pstOutDatas -- 算法输出数据结构体指针
*               unsigned int* puiSuccFlag -- 算法成败标识
*               unsigned int* puiAbnormalFlag -- 算法执行异常标识
* Return:       none
* Description:  1) pGlbBuffer:
*                   为sitppInit()返回的全局buffer指针。
*
*               2) ppuiSuccFlag:
*                   Range: 0-执行成功, >0-执行失败(按bit位保存失败原因)。
*                   详见枚举类型“RP_ARTIH_SUCCESS_FLAG”的定义。
*
*               3) puiAbnormalFlag:
*                   Range: 0-执行正常, >0-执行异常(按bit位保存异常原因)。
*                   详见枚举类型“RP_ARITH_ABNORMAL_FLAG”的定义。
*
*               4) 若ucEnableLogFile == TRUE，则自动将动态配置参数设置成败等log
*                  信息打印到文件“sialib_tofRGBproc_run_log.txt”。
*
*               5) 本函数为算法主入口函数，每帧调用一次。
*
*******************************************************************************/
TOFARITH_EXPORTS void sitrpRunRGBProcess(void* pGlbBuffer,
    RP_INPARAS* pstInDatas, RP_OUTPARAS* pstOutDatas, unsigned int* puiSuccFlag,
    unsigned int* puiAbnormalFlag, unsigned char ucEnableLogFile);


/*******************************************************************************
* FunctionName: sitrpRelease()
* Abstract:     释放算法buffer
* InputParas:   void** pGlbBuffer -- 全局空间buffer指针
*               unsigned char ucEnableLogFile -- 是否生成log文件的开关。
*                                                FALSE-否， TRUE-是。
* OutputParas:  void** pGlbBuffer -- 全局空间buffer指针
* Return:       none
* Description:  1) pGlbBuffer:
*                   为sitppInit()返回的全局buffer指针。
*
*               2) 处理完最后一帧后，调用此函数，释放算法使用的全部buffer空间。
*                  注：应在对算法输出结果的最终调用结束后，再释放。
*                      若在释放后，仍引用算法输出结果，将导致程序错误。
*
*               3) 若ucEnableLogFile == TRUE，则自动将算法释放成功的log信息
*                  打印到文件“sialib_tofRGBproc_release_log.txt”。
*******************************************************************************/
TOFARITH_EXPORTS void sitrpRelease(void** pGlbBuffer,
    unsigned char ucEnableLogFile);


////////////////////////////////////////////////////////////////////////////////
//
// Function declaration （外部可调用 - 子功能接口）
//
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
* FunctionName: sitrpGetVersion()
* Abstract:     获取算法版本号
* InputParas:   none
* OutputParas:  char* -- 算法版本号字符串指针
* Return:       none
* Description:  1) 调用时需在外部开辟为char* pcDllVersion开辟字符数组。
*                  例：
*                       char cDllVerion[CORR_ARITH_VERSION_LEN_MAX] = {0};
*                  再调用此函数，获取算法版本号：
*                       sitcorrGetVersion(cDllVerion);
*
*               2) 若传入的 pcDllVersion 为NULL，
*                  则自动将版本号信息打印到文件“sialib_tofRGBproc_version.txt”。
*
* Modified:     none
*******************************************************************************/
TOFARITH_EXPORTS void sitrpGetVersion(char* pcDllVersion);


/*******************************************************************************
* FunctionName: sitrpSetTofIntrinsicMat()
* Abstract:     获取ToF标定参数
* InputParas:   float* pfInitri -- Tof的内参指针
*               int iInitriLen -- Tof的内参数组长度
*               unsigned char ucEnableLogFile -- 是否生成log文件的开关。
*                                                FALSE-否， TRUE-是。
* OutputParas:  float* pfOutBuffer -- 输出内参指针               
*               unsigned int* puiSuccFlag -- 算法成败标识

* Return:       unsigned char  函数成功标识
* Description:  none
*******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetTofIntrinsicMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

/*******************************************************************************
* FunctionName: sitrpSetRgbIntrinsicMat()
* Abstract:     获取RGB标定参数
* InputParas:   float* pfInitri -- Rgb的内参指针
*               int iInitriLen -- Rgb的内参数组长度
*               unsigned char ucEnableLogFile -- 是否生成log文件的开关。
*                                                FALSE-否， TRUE-是。
* OutputParas:  float* pfOutBuffer -- 输出内参指针
*               unsigned int* puiSuccFlag -- 算法成败标识
*
* Return:       unsigned char  函数成功标识
* Description:  none
*******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetRgbIntrinsicMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);


/*******************************************************************************
* FunctionName: sitrpSetTranslationMat()
* Abstract:     获取Tof和RG的平移参数
* InputParas :  float* pfInitri -- Rgb的内参指针
*               int iInitriLen -- Rgb的内参数组长度
*               unsigned char ucEnableLogFile -- 是否生成log文件的开关。
*                       FALSE - 否， TRUE - 是。
* OutputParas : float* pfOutBuffer -- 输出内参指针
*               unsigned int* puiSuccFlag -- 算法成败标识
*
* Return:       unsigned char  函数成功标识
* Description:  none
*******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetTranslationMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

/*******************************************************************************
*FunctionName: sitrpSetRotationMat()
* Abstract : 获取Tof和RG的旋转参数
* InputParas :  float* pfInitri -- Rgb的内参指针
*               int iInitriLen -- Rgb的内参数组长度
*               unsigned char ucEnableLogFile -- 是否生成log文件的开关。
*                       FALSE - 否， TRUE - 是。
* OutputParas : float* pfOutBuffer -- 输出内参指针
*               unsigned int* puiSuccFlag -- 算法成败标识
*
* Return : unsigned char  函数成功标识
* Description : none
* ******************************************************************************/
TOFARITH_EXPORTS unsigned char sitrpSetRotationMat(float* pfOutBuffer, float* pfInitri, int iInitriLen, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

/*******************************************************************************
*FunctionName: sitrpSetRgbPos()
* Abstract : 获取Tof和RG的相对位置参数
* InputParas :  float* pfInitri -- Rgb的内参指针
*               int iInitriLen -- Rgb的内参数组长度
*               unsigned char ucEnableLogFile -- 是否生成log文件的开关。
*                       FALSE - 否， TRUE - 是。
* OutputParas : float* pfOutBuffer -- 输出内参指针
*               unsigned int* puiSuccFlag -- 算法成败标识
*
* Return : unsigned char  函数成功标识
* Description : none
* ******************************************************************************/

TOFARITH_EXPORTS unsigned char sitrpSetRgbPos(unsigned char* pBOutBuffer, unsigned char bIsLeft, unsigned int* puiSuccFlag, unsigned char ucEnableLogFile);

#endif


