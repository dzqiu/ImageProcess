#include "helper.h"


QImage IplImageToQImage(IplImage img)
{
    QImage image;
    cvCvtColor(&img,&img,CV_BGR2RGB);
    uchar *imgData=(uchar *)img.imageData;
    image=QImage(imgData,img.width,img.height,QImage::Format_RGB888);
    return image;
}


QImage MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

Mat getchannel(Mat src,int channel)
{
    Mat result;

    Mat input = src.clone();
    input.convertTo(input,CV_8UC3);
    Mat rgb[3];
    split(input,rgb);
    cvtColor(input, result, CV_BGR2GRAY);

    if(channel>=3) return result;

    return rgb[0];

}
#define GROW_CONNECTED 4
#if GROW_CONNECTED==4
char x_xn[GROW_CONNECTED]={0,1,-1,0};
char y_yn[GROW_CONNECTED]={1,0,0,-1};
#else
char x_xn[GROW_CONNECTED]=(0,1,-1,0,1,1,-1,-1);
char y_yn[GROW_CONNECTED]=(1,0,0,-1,1,-1,1,-1);
#endif
Mat RegionGrow(Mat gray,Mat peeksMat,float threshold)
{
    vector<Point> peeks;
    Mat grow    =   peeksMat.clone();
    int width   =   peeksMat.cols;
    int height  =   peeksMat.rows;
    for(unsigned int y=0;y<peeksMat.rows;y++)
    {
        for(unsigned int x=0;x<peeksMat.cols;x++)
        {
            if(peeksMat.at<uchar>(y,x)) peeks.push_back(Point(x,y));
        }
    }
    for(unsigned int i = 0;i<peeks.size();i++)
    {
        queue<Point> region;
        Point peekPoint = peeks.at(i);
        region.push(peekPoint);
        unsigned char peekValue = gray.at<uchar>(peekPoint.y,peekPoint.x);
        while(region.size()>0)
        {
            Point Loc   =   region.front();
            region.pop();
            for(int neb=0;neb<4;neb++)
            {
                int xn = Loc.x + x_xn[neb];
                int yn = Loc.y + y_yn[neb];
                if((xn>=0 && xn<width) && (yn>=0 && yn<height)&&grow.at<uchar>(yn,xn)==0)
                {
                    if(gray.at<uchar>(yn,xn)>peekValue*threshold)
                    {
                        grow.at<uchar>(yn,xn)=255;
                        region.push(Point(xn,yn));
                    }
                }
            }

        }
    }
    return grow;
}

const string save_dir  =    "/home/dzqiu/Documents/image/video/save/";
void LabShow(char wname[20],Mat src,Mat Label,Size offset)
{

    Mat sImg = src.clone();
    if(sImg.channels()<3)
        cvtColor(sImg,sImg,COLOR_GRAY2RGB);
    int font_face = cv::FONT_HERSHEY_COMPLEX;
    double font_scale = 0.2;
    for(int x=0;x<Label.cols;x++)
    {
        for(int y=0;y<Label.cols;y++)
        {
            Point pos;
            pos.x=x+offset.width;
            pos.y=y+offset.height;
            if(Label.at<uchar>(y,x))
                putText(sImg,"x",pos,font_face,font_scale,Scalar(0,0,255),1,8);
        }
    }
    imshow(wname,sImg);
//    imwrite(save_dir+wname,sImg);
}
void NumContours(IplImage* result,vector<vector<Point>> contours)
{

    for(int i=0;i<contours.size();i++)
    {
        vector<Point> contour = contours.at(i);
        unsigned int x_ave=0,y_ave=0;
        for(int j=0;j<contour.size();j++)
        {
            Point point=contour.at(j);
            x_ave   +=  point.x;
            y_ave   +=  point.y;
        }
        x_ave  /=  contour.size();
        y_ave  /=  contour.size();
        CvFont font;
        cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,1.0,1.0,0,2,8);
        char string[5];
        sprintf(string,"%d",i);
        cvPutText(result,string,cvPoint(x_ave,y_ave),&font,cvScalar(0,0,0,NULL));  //显示文字
    }
}
void findBeginEnd(Size size,vector<Point> contour,Point &Begin,Point &end)
{
    Mat img=Mat::zeros(size,CV_8UC1);
    drawContours(img,contour,-1,(255),1);
    for(int i=0;i<contour.size();i++)
    {

    }

}
void SegmentCorrect(Size size,vector<vector<Point>> srcPoint,vector<vector<Point>> *dst,int Mode)
{
    const int neb_x[8] = {0,0,1,-1,-1,-1,1,1};
    const int neb_y[8] = {1,-1,0,0,-1,1,-1,1};
    Mat src = Mat::zeros(size,CV_8UC1);
    drawContours(src,srcPoint,-1,Scalar(255));
    vector<vector<Point>> hull(srcPoint.size());
    for(int i=0;i<srcPoint.size();i++)
    convexHull(Mat(srcPoint[i]),hull[i],false);

    Mat drawing = Mat::zeros(size,CV_8UC1);
//    for(int i=0;i<srcPoint.size();i++)
    {
    drawContours(drawing,hull,0,Scalar(255),-1, 8,vector<Vec4i>(), 0, Point());
    drawContours(drawing,srcPoint,0,Scalar(0),-1, 8,vector<Vec4i>(), 0, Point());
    }
    medianBlur(drawing, drawing, 3);
//    imshow("drawing",drawing);

    std::vector<Point2f> ConcavePoint;
    std::vector<std::vector<Point> > contour;//用来存储轮廓
    std::vector<Vec4i> hierarchys;
    findContours(drawing, contour, hierarchys,
                 CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));  //寻找轮廓

    //凹区域少于2要退出
    if(contour.size()<2)
    {
        return ;
    }

    //按照轮廓面积大小排序
    std::sort(contour.begin(),contour.end(),[](const std::vector<Point> &s1,
              const std::vector<Point> &s2){
        double a1=contourArea(s1);
        double a2=contourArea(s2);
        return a1>a2;
    });
//    for(int i=0;i<contour.size();i++)
//    {
//        cout<<"Area="<<contourArea(contour.at(i))<<endl;
//    }
    if(contourArea(contour.at(1))<80) return;
    int minDistance=100000000;//最短距离
    for(int i=0;i<contour[0].size();++i)
        for(int j=0;j<contour[1].size();++j)
        {
            //欧氏距离
            int d= std::sqrt(std::pow((contour[0][i].x-contour[1][j].x),2)+
                    std::pow((contour[0][i].y-contour[1][j].y),2));
            if(minDistance>d)
            {
                minDistance=d;
                ConcavePoint.push_back(contour[0][i]);
                ConcavePoint.push_back(contour[1][j]);
            }
        }
    Point begin =ConcavePoint[ConcavePoint.size() - 2];
    Point end   =ConcavePoint[ConcavePoint.size() - 1];
    vector<Point> srccontour = srcPoint.at(0);

    for(int i=0;i<8;i++)
    {
        int x_nb =begin.x + neb_x[i];
        int y_nb =begin.y + neb_y[i];
        if(x_nb>=0 && x_nb<src.cols && y_nb>=0 &&y_nb<src.rows)
        {
            src.at<uchar>(y_nb,x_nb)=0;
        }

        x_nb =end.x + neb_x[i];
        y_nb =end.y + neb_y[i];
        if(x_nb>=0 && x_nb<src.cols && y_nb>=0 &&y_nb<src.rows)
        {
            src.at<uchar>(y_nb,x_nb)=0;
        }

    }

    dst->clear();
    vector<vector<Point>> dst_result;
    findContours(src,dst_result,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    std::sort(dst_result.begin(),dst_result.end(),[](const std::vector<Point> &s1,
              const std::vector<Point> &s2){
        double a1=s1.size();
        double a2=s2.size();
        return a1>a2;
    });
    if(Mode)
        for(int k=0;k<dst_result.size();k++)
            SortPoint(size,dst_result.at(k));
    dst->push_back(dst_result.at(0));
    dst->push_back(dst_result.at(1));
//    for(int i=0;i<dst_result.size();i++)
//    {
//        if((dst_result.at(i).size())>10) dst->push_back(dst_result.at(i));
//    }

    return ;
}

Point ConnectPoint(Point head,Mat contourImg,Mat &result,bool oneWay=false)
{
    #define Point_CONNECTED 8
    #if Point_CONNECTED==4
    char x_xn[Point_CONNECTED]={0,1,-1,0};
    char y_yn[Point_CONNECTED]={1,0,0,-1};
    #else
    char x_xn[Point_CONNECTED]={0,1,-1,0,1,1,-1,-1};
    char y_yn[Point_CONNECTED]={1,0,0,-1,1,-1,1,-1};
    #endif
    result.setTo(0);
    result.at<float>(head.y,head.x)=1;
    queue<Point> que;
    que.push(head);
    int maxLen=0;
    Point TureHeader;
    if(oneWay)
    {
        for(int i=0;i<Point_CONNECTED;i++)
        {
            int x=head.x+x_xn[i];
            int y=head.y+y_yn[i];
            if(x>=0 && x<result.cols && y>=0 && y<result.rows)
                if(contourImg.at<uchar>(y,x)!=0 && result.at<float>(y,x)==0)
                {
                    que.push(Point(x,y));
                    result.at<float>(y,x)=result.at<float>(head.y,head.x)+1;
                    if(result.at<float>(y,x)>maxLen)
                        TureHeader=Point(x,y);
                    break;
                }
        }
    }


    while(que.size()>0)
    {
        Point header = que.front();
        que.pop();
        for (int i=0;i<Point_CONNECTED;i++)
        {
            int x=header.x+x_xn[i];
            int y=header.y+y_yn[i];
            if(x>=0 && x<result.cols && y>=0 && y<result.rows)
                if(contourImg.at<uchar>(y,x)!=0 && result.at<float>(y,x)==0)
                {
                    que.push(Point(x,y));
                    result.at<float>(y,x)=result.at<float>(header.y,header.x)+1;
                    if(result.at<float>(y,x)>maxLen)
                        TureHeader=Point(x,y);
                }

        }

    }
//    Mat show=result.clone();
//    Mat saveImg=Mat::zeros(Size(256,256),CV_8UC1);

//    normalize(result,show,1.0,0.0,NORM_MINMAX);
//    show=1-show;
//    saveImg = show*255;
//    imwrite(string(SAVE_DIR)+"distance.jpeg",saveImg);
//    imshow("distance Img",show);
//    waitKey();
    return TureHeader;
}


void SortPoint(Size size,vector<Point> &contour)
{
    struct PD_struct
    {
        Point p;
        uint16_t distance;
    };
//    if(contourArea(contour)<=contour.size()) return;
    Mat tmp=Mat::zeros(size,CV_32FC1);
    Mat contourImg = Mat::zeros(size,CV_8UC1);
    vector<vector<Point>> Pvector;
    Pvector.push_back(contour);
    drawContours(contourImg,Pvector,-1,Scalar(255),1);
    Point Header1= ConnectPoint(contour.at(0),contourImg,tmp);
    Point Header2= ConnectPoint(Header1,contourImg,tmp,true);

    vector<struct PD_struct> PD;
    for(int i=0;i<contour.size();i++)
    {
        struct PD_struct  t;
        t.p=contour.at(i);
        t.distance=tmp.at<float>(t.p.y,t.p.x);
        PD.push_back(t);
    }
    std::sort(PD.begin(),PD.end(),[](const struct PD_struct &s1,
              const struct PD_struct &s2){
        double a1=s1.distance;
        double a2=s2.distance;
        return a1>a2;
    });
    int flag=0;
    for(int i=0;i<PD.size()-1;i++)
    {
        Point p1=PD.at(i).p;
        Point p2=PD.at(i+1).p;
        if (abs(p1.x-p2.x)+abs(p1.y-p2.y)>20)
            flag++;
        if(flag>2) return;
    }
   for(int i=0;i<contour.size();i++)
    {

        contour.at(i)=PD.at(i).p;
    }
}

void cvMatMulGPU(CvMat *a,CvMat *b,CvMat *d)
{
    int m=a->rows;
    int k=a->cols;
    int n=b->cols;

    float *dev_A,*dev_B,*dev_H;
    cudaMalloc((void **)&dev_A,m*k*sizeof(float));
    cudaMalloc((void **)&dev_B,k*n*sizeof(float));
    cudaMalloc((void **)&dev_H,m*n*sizeof(float));
    cublasSetVector(m*k,sizeof(float),a->data.ptr,1,dev_A,1);
    cublasSetVector(k*n,sizeof(float),b->data.ptr,1,dev_B,1);
    cudaThreadSynchronize();
    cublasStatus_t status;
    cublasHandle_t handle;
    status=cublasCreate(&handle);
    float alpha=1;float beta=0;
    cublasSgemm(handle,
                CUBLAS_OP_N,
                CUBLAS_OP_N,
                m,n,k,
                &alpha,
                dev_A,m,
                dev_B,k,
                &beta,
                dev_H,m);
    cudaThreadSynchronize();
    cublasGetVector(m*n,sizeof(float),(void*)dev_H,1,(void*)d->data.ptr,1);
    cudaFree(dev_A);
    cudaFree(dev_B);
    cudaFree(dev_H);
    cublasDestroy(handle);
}





