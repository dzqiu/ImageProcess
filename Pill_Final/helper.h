#ifndef HELPER_H
#define HELPER_H
#include <QImage>
#include <QDebug>
#include "Snake/common.h"
#include "Snake/drlse_edge.h"
#include "Snake/gvfc.h"
#include "cuda_runtime.h"
#include "cublas_v2.h"
#include <iostream>
#include <vector>
#include <queue>
#include <string>

struct MESSAGE_STRUCT
{
  int num;
  QString color;
  QString shape;
};
const char LabelString[4][20]={"胶囊","圆形药丸","椭圆形药丸","三角形药丸"};
using namespace std;
using namespace cv;
typedef  unsigned char eleType;
QImage MatToQImage(const cv::Mat& mat);
QImage IplImageToQImage(IplImage img);
void SegmentCorrect(Size size, vector<vector<Point>> srcPoint, vector<vector<Point>> *dst, int Mode);
void LabShow(char wname[20],Mat src,Mat Label,Size offset);
void NumContours(IplImage* result,vector<vector<Point>> contours);
Mat getchannel(Mat src, int channel);
Mat RegionGrow(Mat gray,Mat peeksMat,float threshold);
Mat imregionmax(const Mat *src,eleType h);
void SortPoint(Size size,vector<Point> &contour);
void cvMatMulGPU(CvMat *a,CvMat *b,CvMat *d);

#endif // HELPER_H
