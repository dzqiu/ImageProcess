#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QMessageBox>
//#include <opencv2/highgui.hpp>
//#include <opencv2/core.hpp>
#include "Snake/common.h"
#include "Snake/drlse_edge.h"
#include "Snake/gvfc.h"
#include "helper.h"
#include <fstream>
#include <QFile>
#include <QFileDialog>
#include "detailwindow.h"
using namespace std;
using namespace cv;
#define ConvaIMCPLI 0
#define ConvaCPLI   1
#define EllipseShape 2
namespace Ui {
class MainWindow;
}

struct SETTING_STRUCT{
  float growTh;
  int GVF_ITER;
  int Snake_ITER;
  int Snake_Step;
  bool Close;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_NextFrameButton_clicked();

    void on_DetailButton_clicked();

    void on_pushButton_clicked();

    void on_SelectIMG_clicked();

private:

    void Init();
    void Preprocess();
    void Snake();
    void CountShow();
    void SetArg();
    void SaveContours();
    void LoadNet();
    void PillSelect();

    void ConcaveSegment( Size size,vector<vector<Point>> srcContours,vector<vector<Point>> *dst,int Mode);

    Ui::MainWindow *ui;
    struct SETTING_STRUCT ARG;
    VideoCapture video;
    int FrameCount;
    Mat FrameMat;
    Mat ResultMat;
    //IplImage *ResultIplImage;

    Mat ROIRGB;
    Mat ROIGray;
    Mat ROIThreshold;


    /* distance = |x1-x2| + |y1-y2|
    /* 应用于峰值查找 */
    Mat ROIDistanTF32F_L1;
    Mat ROIDistanTF8U_L1;
    /* the simple euclidean distance */
    /* 应用于区域生长 */
    Mat ROIDistanTF32F_L2;
    Mat ROIDistanTF8U_L2;


    /**********************************/
    float alpha,beta,gamma,kappa;

    /**********************************/


    Mat ROISeed;                        /*GVF 种子轮廓图，即是初始轮廓*/
    Mat ROIContours;                    /*整体轮廓，用于计算GVF*/
    vector<vector<Point>> SeedContours; /*GVF 种子轮廓，用于sanke*/
    vector<vector<Point>> GVFContours;  /*GVF 结果轮廓*/
    vector<vector<Point>> CorrectContours;
    int result[50];
    QImage FrameImage;
    QImage ResultImage;
    vector<MESSAGE_STRUCT> messResult;


};

#endif // MAINWINDOW_H
