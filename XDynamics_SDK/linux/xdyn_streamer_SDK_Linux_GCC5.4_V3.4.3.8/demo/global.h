#include <opencv/opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

//RGB的镜头参数
#define RGB_LEN_PARAS_NUM_MAX          9

#define SUB_FRAME_WIDTH_MAX            640 //子帧图像宽度的最大值
#define SUB_FRAME_HEIGHT_MAX           480 //子帧图像高度的最大值

#define BBOOL bool

#define TRUE true

#define FALSE false

#define EPSILON_FLOAT               0.000001f //浮点数据精度

#define ABS(a)                      ((a) < 0 ? -(a) : (a)) //取绝对值

typedef struct RP_Rgb_Lens
{
	float fRgbLenParas[RGB_LEN_PARAS_NUM_MAX]; //可见光镜头校正参数[fx, fy, cx, cy, k1, k2, p1, p2, k3]
	float fRgbZoomCoef; //可见光镜头校正后图像的缩放系数：<1-放大（裁黑边），1-不缩放，>1-缩小
	float fRgbSkew; //可见光描述传感器轴间的倾斜，由传感器的安装没有与光轴垂直引起

}RP_RGB_LENS;

//定义当前图像到标定图像的坐标映射结构体
typedef struct CoordinateMap_Out2Cali
{
	int iRoiXMapUnitLen; //ROI区域X方向像素的坐标映射表单位长度
	int iRoiYMapUnitLen; //ROI区域Y方向像素的坐标映射表单位长度
	int iRoiXMapTimes; //ROI区域X方向像素的坐标映射次数
	int iRoiYMapTimes; //ROI区域Y方向像素的坐标映射次数
	int iRoiXMapLUT[SUB_FRAME_WIDTH_MAX]; //ROI区域X方向像素的坐标映射表
	int iRoiYMapLUT[SUB_FRAME_HEIGHT_MAX]; //ROI区域Y方向像素的坐标映射表
} COOR_MAP;

typedef struct Rect_Short16
{
	short sXL; //左上角x坐标（left）
	short sYT; //左上角y坐标（top）
	short sXR; //右下角x坐标（right）
	short sYB; //右下角y坐标（bottom）
} RECT16S;

//定义2D坐标结构体-浮点型32位
//注：变量类型前缀为“coor2f”，例：COOR2D32F coor2fPnt
typedef struct Coordinate2D_Float32
{
	float fX; //x坐标
	float fY; //y坐标
} COOR2D32F;


//------------------------------------------------------------------------------
//定义镜头校正双线性插值坐标映射表结构体
typedef struct LenInterpolationMap
{

	int iMapNearestId; //从校正图到原始图的映射坐标，最近邻插值-映射点的图像坐标 ：值为-1时，标记该最近邻点无效。
	int iMapNearId11; //双线性插值-映射后坐标的左上角邻域点的图像坐标
	int iMapNearId12; //双线性插值-映射后坐标的右上角邻域点的图像坐标
	int iMapNearId21; //双线性插值-映射后坐标的左下角邻域点的图像坐标
	int iMapNearId22; //双线性插值-映射后坐标的右下角邻域点的图像坐标
	unsigned char ucCoef11; //双线性插值-左上角邻域点插值系数
	unsigned char ucCoef12; //双线性插值-右上角邻域点插值系数
	unsigned char ucCoef21; //双线性插值-左下角邻域点插值系数
	unsigned char ucCoef22; //双线性插值-右下角邻域点插值系数
} LEN_IMAP;




// typedef struct XdynRegParams_st
// {
// 	float fTofIntrinsicMatrix[9]; //Tof相机的内参矩阵

// 	float fRgbIntrinsicMatrix[9]; //Rgb相机的内参矩阵

// 	float fRotationMatrix[9]; //右相机相对左相机的旋转矩阵

// 	float fTranslationMatrix[3]; //右相机相对左相机的平移矩阵

// }XdynRegParams_t;


void RP_GetRGBMappingPt(vector<Point>& TrayHole, vector<Point>& TrayHoleMap, int* pInterpInversMap, int width, int height);

bool RP_CalcOutRgb2CaliCoorMap(COOR_MAP* pstCoorMap, int iWidth, int iHeight);

void GLB_GetCoordinateMapInROI(int iXMapUnitLen, int iYMapUnitLen,
	int iXMapTimes, int iYMapTimes, int* piXMapLUT, int* piYMapLUT,
	RECT16S rsThisROIRect, int* piRoiXMapLUT, int* piRoiYMapLUT,
	int* piRoiXMapUnitLen, int* piRoiYMapUnitLen,
	int* piRoiXMapTimes, int* piRoiYMapTimes);

bool RP_RgbLEN_CreateUndistortMap(RP_RGB_LENS* pstRgbLens, int iWidth, int iHeight, COOR_MAP* pstCoorMap, LEN_IMAP* pstInterpMap, int* pInterpInversMap);

void RP_RgbLEN_Undistort(unsigned char* pucImage, LEN_IMAP* pstInterpMap, int width, int height);

void RP_Calc3x3Mutiply3x1Matrix(float* pSrc1, float* pSrc2, float* pDst);

void RP_Calc3x3TransposeMatrix(float* pSrc, float* pDst);

int RP_Calc3x3InverseMatrix(float* pSrc, float* pDst);

void Gray16To8(cv::Mat& src, cv::Mat& dst);		//该函数待优化

int GetIdent(uint16_t* Depth, int height, int width, vector<Point>& TrayHole, float* RGBMatrixT,
	float* TraMatrix, float* AleMatrixT, float* TofMatrix);

int ReadDepth(const char* fileName, uint16_t** Depth, int len, int FrameNum);

int SaveImg(const char* fileName, uint8_t* pucImg, int len);
