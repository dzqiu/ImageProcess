#include "mainwindow.h"
#include "ui_mainwindow.h"

#define VIDEO_DIR               "/home/dzqiu/Documents/image/video/no_overlay.MOV"
#define SAVE_DIR                "/home/dzqiu/qt-workspace/Pill_Final/IMG/"
#define SAVE_SEGMENTATION       "/home/dzqiu/qt-workspace/Pill_Final/python/segmentation/"
#define PYTHON_LOAD             "/home/dzqiu/qt-workspace/Pill_Final/python/loadNet.py"
#define SAVE_CLASSIFY           "/home/dzqiu/qt-workspace/Pill_Final/python/segmentation/result.txt"
#define OPENDIR_DEFALUT         "/home/dzqiu/qt-workspace/Pill_Final/IMG"
#define FRAME_WIDTH     1024
#define FRAME_HEIGHT    512
#define RIO_SIZE        cv::Size(512,512)
#define RIO_POINT       cv::Point(325,0)
#define RIO             cv::Rect(RIO_POINT,RIO_SIZE)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    SetArg();
//    Init();
//    ui->SrcLabel->setPixmap(QPixmap::fromImage(FrameImage.scaled(ui->SrcLabel->width(),ui->SrcLabel->height())));
//    ui->ResLabel->setPixmap(QPixmap::fromImage(ResultImage.scaled(ui->SrcLabel->width(),ui->SrcLabel->height())));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    video = VideoCapture(VIDEO_DIR);
    if(!video.isOpened())
    {
        QMessageBox::warning(this,"Error","Video read error.");
        return ;
    }
    FrameCount=0;

//    video >> FrameMat;
     imwrite(string(SAVE_DIR)+"FrameMat.bmp",FrameMat);
    cv::resize(FrameMat,FrameMat,Size(FRAME_WIDTH,FRAME_HEIGHT));
//    imshow("FrameMat",FrameMat);

    if(FrameMat.empty())
    {
        return;
    }
    FrameImage  = MatToQImage(FrameMat);
    ResultMat   = Mat(FrameMat.size(),CV_8U);
    ResultMat.setTo(0);
    ResultImage = MatToQImage(ResultMat);
    float t = (float)cvGetTickCount();
    Preprocess();
    cout<<" preprocess time = "<<((float)cvGetTickCount() - t)/(cvGetTickFrequency()*1e6)<<endl;
    t = (float)cvGetTickCount();
    Snake();
    SaveContours();
    cout<<" GVF time = "<<((float)cvGetTickCount() - t)/(cvGetTickFrequency()*1e6)<<endl;
    t = (float)cvGetTickCount();
    LoadNet();
    cout<<" LoadTime time = "<<((float)cvGetTickCount() - t)/(cvGetTickFrequency()*1e6)<<endl;
}
void MainWindow::Preprocess()
{
    FrameMat(RIO).copyTo(ROIRGB);

    ROIGray         =   getchannel(ROIRGB,0);

    GaussianBlur(ROIGray,ROIGray,Size(3,3),0,0);

    threshold(ROIGray,ROIThreshold,0,255,CV_THRESH_OTSU);
    ROIThreshold    =   ~ROIThreshold;

    /*distance transform to look for centre of the pill*/
    distanceTransform(ROIThreshold,ROIDistanTF32F_L1,CV_DIST_L1,3);
    normalize(ROIDistanTF32F_L1,ROIDistanTF32F_L1,1.0,0.0,NORM_MINMAX);
    Mat(ROIDistanTF32F_L1*255).convertTo(ROIDistanTF8U_L1,CV_8U);
    Mat peeks = imregionmax(&ROIDistanTF8U_L1,1);


    distanceTransform(ROIThreshold,ROIDistanTF32F_L2,CV_DIST_L2,3);
    normalize(ROIDistanTF32F_L2,ROIDistanTF32F_L2,1.0,0.0,NORM_MINMAX);
    Mat(ROIDistanTF32F_L2*255).convertTo(ROIDistanTF8U_L2,CV_8U);
    ROISeed     = RegionGrow(ROIDistanTF8U_L2,peeks,ARG.growTh);
    ROISeed.copyTo(ResultMat(RIO));
    ResultImage = MatToQImage(ResultMat);



    /*comput the contour of the source image,using to compute GVF*/
    vector<vector<cv::Point>> srcContourPoints ;
    findContours(ROIThreshold,srcContourPoints,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
    ROIContours = Mat(ROIThreshold.size(),CV_8UC1,cv::Scalar(0));
    drawContours(ROIContours,srcContourPoints,-1,Scalar(255),1);


    vector<vector<cv::Point>> tmpContours;

    findContours(ROISeed,tmpContours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    int InitLen=tmpContours.size();
    ConcaveSegment(RIO_SIZE,tmpContours,&SeedContours,ConvaIMCPLI);

    while(InitLen!=SeedContours.size())
    {
        InitLen=tmpContours.size();
        tmpContours.clear();
        tmpContours.insert(tmpContours.end(),SeedContours.begin(),SeedContours.end());
        ConcaveSegment(RIO_SIZE,tmpContours,&SeedContours,ConvaIMCPLI);
    }
#if 0
    Mat tmp0=ROIDistanTF8U_L2.clone();
    cvtColor(tmp0,tmp0,COLOR_GRAY2RGB);
    drawContours(tmp0,SeedContours,-1,Scalar(0,0,0),1);
    imshow("ROIRGB",tmp0);
//    imwrite(string(SAVE_DIR)+"GrowWith32.bmp",tmp0);
#endif

    cout<<"Seed n:"<<SeedContours.size()<<endl;
#if 0
    Mat tmp1=ROIDistanTF8U_L2.clone();
    cvtColor(tmp1,tmp1,COLOR_GRAY2RGB);
    drawContours(tmp1,SeedContours,-1,Scalar(0,0,0),2);
    imshow("ROIRGB",tmp1);
    imwrite(string(SAVE_DIR)+"SeedContoursEllip.bmp",tmp1);
#endif
}
void MainWindow::ConcaveSegment( Size size,vector<vector<Point>> srcContours,vector<vector<Point>> *dst,int Mode=ConvaCPLI)
{
   dst->clear();
   for(int i=0;i<srcContours.size();i++)
   {
       vector<vector<Point>> cSample;
       cSample.clear();
       cSample.push_back(srcContours.at(i));
       vector<vector<Point>> SegContour;
       SegmentCorrect(RIO_SIZE,cSample,&SegContour,Mode);
       vector<vector<Point>> result;
       result.clear();
       if(SegContour.size()>0) result = SegContour;
       else result.push_back(srcContours.at(i));
       if(Mode !=EllipseShape)
       {
        dst->insert(dst->end(),result.begin(),result.end());
        continue;
       }


       for(int j=0;j<result.size();j++)
       {
           Mat tmp=Mat::zeros(size,CV_8UC1);
           vector<Point> contour = result.at(j);
           Point pt=contour.at(contour.size()-1);
           RotatedRect ellipsemege =fitEllipse(contour);
           ellipse(tmp,ellipsemege,(255));
           Mat th=Mat::zeros(size,CV_8UC1);
           threshold(tmp,th,0,255,CV_THRESH_OTSU);
           vector<vector<Point>> ellipPoint;
           ellipPoint.clear();
           findContours(th,ellipPoint,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
           dst->push_back(ellipPoint.at(0));
       }

   }
}
void MainWindow::Snake()
{

    IplImage Img(ROIContours);
    CvMat *src,*data,*u,*v,sstub;
    src = cvGetMat(&Img,&sstub);
    CvSize size = cvGetSize(src);
    data = cvCreateMat(size.height, size.width, CV_32FC1);
    cvConvertScale(src, data, 1, 0 );
    u = cvCreateMat( size.height, size.width, CV_32FC1);
    v = cvCreateMat( size.height, size.width, CV_32FC1);
    float mu = 0.2f;int alg = CV_GVF;
    // Compute Gradient Vector Flow
    cvGVF(data, u, v, mu, 300, alg);

    GVFContours.clear();
    for(int n=0;n<SeedContours.size();n++)
//    int n=2;
    {
        vector<Point> seek  = SeedContours.at(n);
        int           length= seek.size();
        CvPoint *point = new CvPoint[length];
        for(int k=0;k<length;k++)
        {
            CvPoint pt(seek.at(k));
            point[k]=pt;
        }
        alpha=0.06f,beta=0.0f,gamma=1.0f,kappa=2.0f;
        point = cvSnakeImageGVFloop(point,&length,alpha, beta, gamma, kappa, 10, 20, CV_REINITIAL,u,v);
        vector<Point> GVFResult;
        GVFResult.clear();
        for(int k=0;k<length;k++)
        {
            GVFResult.push_back(point[k]);
        }
        GVFContours.push_back(GVFResult);
    }

    CorrectContours=GVFContours;


}

void MainWindow::SaveContours()
{
    for (int i=0;i<CorrectContours.size();i++)
    {
        Mat contourImg=ROIRGB.clone();
        contourImg.setTo(0);
        drawContours(contourImg,CorrectContours,i,Scalar(255,255,255),1);
        char name[100];
        sprintf(name,SAVE_SEGMENTATION"%d.jpeg",i);
        imwrite(name,contourImg);

    }
}

enum colorType{Red = 0, White, Blue, ColorButt};

const Scalar hsvRedLo( 120,  40,  40);
const Scalar hsvRedHi(130, 255, 255);

const Scalar hsvWhiteLo(50,  0,  0);
const Scalar hsvWhiteHi(120, 255, 255);

const Scalar hsvBlueLo(0,  40,  40);
const Scalar hsvBlueHi(50, 255, 255);

vector<Scalar> hsvLo{hsvRedLo, hsvWhiteLo, hsvBlueLo};
vector<Scalar> hsvHi{hsvRedHi, hsvWhiteHi, hsvBlueHi};
vector<String> textColor{"红", "白", "蓝"};

void MainWindow::PillSelect()
{
    for(int i=0;i<CorrectContours.size();i++)
        SortPoint(RIO_SIZE,CorrectContours.at(i));
    Mat RIOHSV;
    cvtColor(ROIRGB,RIOHSV,CV_BGR2HSV);
    messResult.clear();
    for(int i=0;i<CorrectContours.size();i++)
    {
        Mat pill(RIO_SIZE,CV_8UC1);
        pill.setTo(0);
        drawContours(pill,CorrectContours,i,255,-1);
        Mat RIOPill=ROIRGB.clone();
        for(int c=0;c<RIOPill.cols;c++)
            for(int r=0;r<RIOPill.cols;r++)
                if(pill.at<uchar>(r,c)==0)
                {
                    RIOPill.at<cv::Vec3b>(r,c)[0]=0;
                    RIOPill.at<cv::Vec3b>(r,c)[1]=0;
                    RIOPill.at<cv::Vec3b>(r,c)[2]=0;
                }
        cvtColor(RIOPill,RIOPill,CV_RGB2HSV);
        String colorString;
        for(int k=Red;k<3;k++)
        {
            Mat RangeImg;
            inRange(RIOPill,hsvLo[k],hsvHi[k],RangeImg);
            vector<vector<Point>> rangContours;
            findContours(RangeImg,rangContours,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
            std::sort(rangContours.begin(),rangContours.end(),[](const std::vector<Point> &s1,
                      const std::vector<Point> &s2){
                double a1=contourArea(s1);
                double a2=contourArea(s2);
                return a1>a2;
            });
            if(rangContours.size()==0) continue;
            if(contourArea(rangContours.at(0))>contourArea(CorrectContours.at(i))/4)
                colorString.append(textColor.at(k));
        }
        MESSAGE_STRUCT re;
        re.num=i;
        re.color=QString::fromStdString(colorString);
        re.shape=QString(LabelString[result[i]]);
        messResult.push_back(re);
    }


}
void MainWindow::LoadNet()
{
    system("~/anaconda2/bin/python "PYTHON_LOAD);
    QFile fh(SAVE_CLASSIFY);
    if(!fh.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    while(!fh.atEnd())
    {
        QByteArray instring = fh.readLine();
        int num=QString(instring).toInt();
        instring = fh.readLine();
        int r=QString(instring).toInt();
        result[num]=r;
    }
    PillSelect();
    CountShow();

}

void MainWindow::CountShow()
{
    drawContours(ROIRGB,CorrectContours,-1,Scalar(0,0,255),3);
    IplImage *ResultIplImage =new IplImage(ROIRGB);
    NumContours(ResultIplImage,CorrectContours);
    Mat tmp(ResultIplImage);
    imwrite(string(SAVE_DIR)+"CorrectResult.bmp",tmp);
    cout<<"--------result-------------"<<endl;
    cout<<"Frame="<<FrameCount<<" cout="<<CorrectContours.size()<<endl;
    ResultMat = FrameMat;
    tmp.copyTo(ResultMat(RIO));
    ResultImage = MatToQImage(ResultMat);
    char couter[4]={0};
    for (int i=0;i<CorrectContours.size();i++)
    {
        couter[result[i]]++;
    }
    for (int i=0;i<4;i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(LabelString[i]));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(couter[i])));
    }
}
void MainWindow::SetArg()
{
    ARG.growTh=ui->GrowEdit->text().toFloat();
    ARG.Snake_ITER=ui->SnakeIterEdit->text().toInt();
    ARG.Snake_Step=ui->SnakeStepEdit->text().toInt();
    ARG.Close = ui->CLoseCheckBox->isChecked();
}

void MainWindow::on_NextFrameButton_clicked()
{
     FrameCount++;
//     video >> FrameMat;
     cv::resize(FrameMat,FrameMat,Size(FRAME_WIDTH,FRAME_HEIGHT));
     FrameImage  = MatToQImage(FrameMat);
     ResultMat   = Mat(FrameMat.size(),CV_8U);
     ResultMat.setTo(0);
     ResultImage = MatToQImage(ResultMat);

     float t = (float)cvGetTickCount();
     Preprocess();
     float processTime=((float)cvGetTickCount() - t)/(cvGetTickFrequency()*1e6);
     t = (float)cvGetTickCount();
     Snake();
     SaveContours();
     float GVFTime=((float)cvGetTickCount() - t)/(cvGetTickFrequency()*1e6);
     t = (float)cvGetTickCount();
     LoadNet();
     cout<<"Preprocess time = "<<processTime<<endl;
     cout<<"GVF time = "<<GVFTime<<endl;
     cout<<"LoadTime time = "<<((float)cvGetTickCount() - t)/(cvGetTickFrequency()*1e6)<<endl;
     cout<<"--------------------"<<endl;
     ui->SrcLabel->setPixmap(QPixmap::fromImage(FrameImage.scaled(ui->SrcLabel->width(),ui->SrcLabel->height())));
     ui->ResLabel->setPixmap(QPixmap::fromImage(ResultImage.scaled(ui->SrcLabel->width(),ui->SrcLabel->height())));
}

void MainWindow::on_DetailButton_clicked()
{
    detailWindow *t = new detailWindow(messResult);
    t->exec();
}

void MainWindow::on_pushButton_clicked()
{
    SetArg();
}

void MainWindow::on_SelectIMG_clicked()
{
     QFileDialog *fileDialog = new QFileDialog(this);
     fileDialog->setWindowTitle(tr("Save As"));//设置文件保存对话框的标题
     fileDialog->setAcceptMode(QFileDialog::AcceptOpen);//设置文件对话框为保存模式
     fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
     fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信
     fileDialog->setGeometry(10,30,300,200);//设置文件对话框的显示位置
     fileDialog->setDirectory(OPENDIR_DEFALUT);//设置文件对话框打开时初始打开的位置
//     fileDialog->setFilter(("Image Files(*.jpg *.png)"));//设置文件类型过滤器
     if(fileDialog->exec() == QDialog::Accepted) {//注意使用的是QFileDialog::Accepted或者QDialog::Accepted,不是QFileDialog::Accept
     QString path = fileDialog->selectedFiles()[0];//得到用户选择的文件名
     ui->IMGLable->setText(QFileInfo(path).fileName());
     FrameMat=imread(path.toStdString());
     FrameImage  = MatToQImage(FrameMat);
     ui->SrcLabel->setPixmap(QPixmap::fromImage(FrameImage.scaled(ui->SrcLabel->width(),ui->SrcLabel->height())));
     }

}
