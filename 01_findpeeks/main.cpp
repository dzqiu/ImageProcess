#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <queue>
using namespace std;
using namespace cv;
int x_offset[8] = {1,1,1,0,0,-1,-1,-1};
int y_offset[8] = {1,-1,0,-1,1,-1,1,0};
#define NEGHTBOR 8
/*寻找最大值邻近点*/
uchar regionalmaxima(Mat im,Point p)
{
    uchar maxima = im.at<uchar>(p.y,p.x);
    for(int i=0;i<NEGHTBOR;i++)
    {
        int x = p.x+x_offset[i];
        int y = p.y+y_offset[i];
        if(x>im.cols-1) x = im.cols-1;
        if(x<0)         x = 0;
        if(y>im.rows-1) y = im.rows-1;
        if(y<0)         y = 0;
        if(im.at<uchar>(y,x)>maxima)
            maxima = im.at<uchar>(y,x);
    }
    return maxima;

}

/*返回最大值滤波图像*/
Mat regionalmaximaImg(Mat im)
{
    Mat result = im.clone();
    for(int i=0;i<im.rows;i++)
        for(int j=0;j<im.cols;j++)
            result.at<uchar>(i,j) = regionalmaxima(im,Point(j,i));
    return result;

}
/*邻近点是否存零点*/
bool isexistNBzero(Mat im,Point p)
{
    for(int i=0;i<NEGHTBOR;i++)
    {
        int x = p.x+x_offset[i];
        int y = p.y+y_offset[i];
        if(x>im.cols-1) x = im.cols-1;
        if(x<0)       x = 0;
        if(y>im.rows-1) x = im.rows-1;
        if(y<0)       y = 0;
        if(im.at<uchar>(y,x)==0)
            return true;
    }
    return false;
}
Mat findpeeks(Mat I,Mat J)
{   
    queue<Point> que;
    /*最大值滤波*/
    J = regionalmaximaImg(J);
    /*对满足条件的点入队列，初始化队列*/
    for(int i=0;i<I.rows;i++)
        for(int j=0;j<I.cols;j++)
            if(J.at<uchar>(i,j) && isexistNBzero(J,Point(j,i)))
                que.push(Point(j,i));
    /*循环队列，直至队列为空*/
    while(que.empty())
    {
        Point p = que.front();
        que.pop();

        for(int i=0;i<NEGHTBOR;i++)
        {
            /*get the neighbor point*/
            Point q;
            q.x = p.x+x_offset[i];
            q.y = p.y+y_offset[i];
            if(q.x>I.cols-1) q.x = I.cols-1;
            if(q.x<0)       q.x = 0;
            if(q.y>I.rows-1) q.y = I.rows-1;
            if(q.y<0)       q.y = 0;

            if(J.at<uchar>(p.y,p.x)<J.at<uchar>(q.y,q.x) &&
                    I.at<uchar>(q.y,q.x)!=I.at<uchar>(q.y,q.x))
            {
                    J.at<uchar>(p.y,p.x)=min(J.at<uchar>(p.y,p.x),
                                                I.at<uchar>(q.y,q.x));
                    que.push(p);
            }
        }
    }
    return (I-J)>0;
}
void LabShow(char wname[20],Mat src,Mat Label,Size offset)
{

    Mat sImg = src.clone();
    if(sImg.channels()<3)
        cvtColor(sImg,sImg,COLOR_GRAY2RGB);
    int font_face = cv::FONT_HERSHEY_COMPLEX;
    double font_scale = 1;
    for(int x=0;x<Label.cols;x++)
    {
        for(int y=0;y<Label.cols;y++)
        {
            Point pos;
            pos.x=x+offset.width;
            pos.y=y+offset.height;
            if(Label.at<uchar>(y,x))
                putText(sImg,"x",pos,font_face,font_scale,Scalar(0,0,255),3,8);
        }
    }
    imshow(wname,sImg);
}

int main(int argc, char *argv[])
{
    /*构建图像*/
    Mat src(Size(1024,1024),CV_8UC1);
    src.setTo(0);
    circle(src,Point(150,200),100,Scalar(255),-1);
    circle(src,Point(300,300),100,Scalar(255),-1);
    circle(src,Point(400,600),100,Scalar(255),-1);
    circle(src,Point(550,600),100,Scalar(255),-1);
    namedWindow("input", CV_WINDOW_NORMAL);
    imshow("input",src);

    Mat dis=Mat(src.size(),CV_32FC1);
    distanceTransform(src,dis,CV_DIST_L2,3);
    normalize(dis,dis,1.0,0.0,NORM_MINMAX);
    dis = dis *255;dis.convertTo(dis,CV_8U);
    namedWindow("distance Transfrom", CV_WINDOW_NORMAL);
    imshow("distance Transfrom",dis);

    namedWindow("peeks", CV_WINDOW_NORMAL);
    Mat peeks = findpeeks(dis,dis-1);
    LabShow("peeks",dis,peeks,Size(0,0));

    waitKey(0);
    return 0;
}
