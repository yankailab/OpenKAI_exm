#include <opencv/opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

//RGB�ľ�ͷ����
#define RGB_LEN_PARAS_NUM_MAX          9

#define SUB_FRAME_WIDTH_MAX            640 //��֡ͼ���ȵ����ֵ
#define SUB_FRAME_HEIGHT_MAX           480 //��֡ͼ��߶ȵ����ֵ

#define BBOOL bool

#define TRUE true

#define FALSE false

#define EPSILON_FLOAT               0.000001f //�������ݾ���

#define ABS(a)                      ((a) < 0 ? -(a) : (a)) //ȡ����ֵ

typedef struct RP_Rgb_Lens
{
	float fRgbLenParas[RGB_LEN_PARAS_NUM_MAX]; //�ɼ��⾵ͷУ������[fx, fy, cx, cy, k1, k2, p1, p2, k3]
	float fRgbZoomCoef; //�ɼ��⾵ͷУ����ͼ�������ϵ����<1-�Ŵ󣨲úڱߣ���1-�����ţ�>1-��С
	float fRgbSkew; //�ɼ�������������������б���ɴ������İ�װû������ᴹֱ����

}RP_RGB_LENS;

//���嵱ǰͼ�񵽱궨ͼ�������ӳ��ṹ��
typedef struct CoordinateMap_Out2Cali
{
	int iRoiXMapUnitLen; //ROI����X�������ص�����ӳ���λ����
	int iRoiYMapUnitLen; //ROI����Y�������ص�����ӳ���λ����
	int iRoiXMapTimes; //ROI����X�������ص�����ӳ�����
	int iRoiYMapTimes; //ROI����Y�������ص�����ӳ�����
	int iRoiXMapLUT[SUB_FRAME_WIDTH_MAX]; //ROI����X�������ص�����ӳ���
	int iRoiYMapLUT[SUB_FRAME_HEIGHT_MAX]; //ROI����Y�������ص�����ӳ���
} COOR_MAP;

typedef struct Rect_Short16
{
	short sXL; //���Ͻ�x���꣨left��
	short sYT; //���Ͻ�y���꣨top��
	short sXR; //���½�x���꣨right��
	short sYB; //���½�y���꣨bottom��
} RECT16S;

//����2D����ṹ��-������32λ
//ע����������ǰ׺Ϊ��coor2f��������COOR2D32F coor2fPnt
typedef struct Coordinate2D_Float32
{
	float fX; //x����
	float fY; //y����
} COOR2D32F;


//------------------------------------------------------------------------------
//���徵ͷУ��˫���Բ�ֵ����ӳ���ṹ��
typedef struct LenInterpolationMap
{

	int iMapNearestId; //��У��ͼ��ԭʼͼ��ӳ�����꣬����ڲ�ֵ-ӳ����ͼ������ ��ֵΪ-1ʱ����Ǹ�����ڵ���Ч��
	int iMapNearId11; //˫���Բ�ֵ-ӳ�����������Ͻ�������ͼ������
	int iMapNearId12; //˫���Բ�ֵ-ӳ�����������Ͻ�������ͼ������
	int iMapNearId21; //˫���Բ�ֵ-ӳ�����������½�������ͼ������
	int iMapNearId22; //˫���Բ�ֵ-ӳ�����������½�������ͼ������
	unsigned char ucCoef11; //˫���Բ�ֵ-���Ͻ�������ֵϵ��
	unsigned char ucCoef12; //˫���Բ�ֵ-���Ͻ�������ֵϵ��
	unsigned char ucCoef21; //˫���Բ�ֵ-���½�������ֵϵ��
	unsigned char ucCoef22; //˫���Բ�ֵ-���½�������ֵϵ��
} LEN_IMAP;




// typedef struct XdynRegParams_st
// {
// 	float fTofIntrinsicMatrix[9]; //Tof������ڲξ���

// 	float fRgbIntrinsicMatrix[9]; //Rgb������ڲξ���

// 	float fRotationMatrix[9]; //�����������������ת����

// 	float fTranslationMatrix[3]; //���������������ƽ�ƾ���

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

void Gray16To8(cv::Mat& src, cv::Mat& dst);		//�ú������Ż�

int GetIdent(uint16_t* Depth, int height, int width, vector<Point>& TrayHole, float* RGBMatrixT,
	float* TraMatrix, float* AleMatrixT, float* TofMatrix);

int ReadDepth(const char* fileName, uint16_t** Depth, int len, int FrameNum);

int SaveImg(const char* fileName, uint8_t* pucImg, int len);
