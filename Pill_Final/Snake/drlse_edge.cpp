//
//  drlse_edge.cpp
//  DRLSE_P/Users/diyang/Desktop/Research_src/DRLSE/DRLSE/drlse_edge.cpp
//
//  Created by Di Yang on 1/03/12.
//  Copyright (c) 2012 The Australian National University. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "drlse_edge.h"
#include "common.h"

#define SMALLNUM 1e-10f
#define PI 3.1416f

CV_IMPL void cvDirac(const CvArr* srcarr,
                     CvArr* dstarr,
                     double sigma)
{
    CV_FUNCNAME("cvDirac");
    
    __BEGIN__;
    CvMat sstub, *src;
    CvMat dstub, *dst;
    CvSize size;
    int i, j, iStep_src, iStep_dst;
    float* fPtr_src, *fPtr_dst, flag=0.0f;
    float temp1=0.0f, temp2=0.0f;
    
    CV_CALL( src = cvGetMat(srcarr, &sstub ));
    CV_CALL( dst = cvGetMat(dstarr, &dstub ));
    
    if( CV_MAT_TYPE(src->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( CV_MAT_TYPE(dst->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel output images are supported" );
    if( !CV_ARE_SIZES_EQ( src, dst ))
        CV_ERROR( CV_StsUnmatchedSizes, "The input images must have the same size" );
    size = cvGetMatSize( src );
    
    iStep_src = src->step / sizeof(fPtr_src[0]);
    fPtr_src = src->data.fl;
    iStep_dst = dst->step / sizeof(fPtr_dst[0]);
    fPtr_dst = dst->data.fl;
    
    for (j=0; j<size.height; j++){
        for (i=0; i<size.width; i++){
            temp1 = fPtr_src[i+iStep_src*j];
            temp2 = (1.0f/2.0f/sigma)*(1.0f+cos(PI*temp1/sigma));
            if (int(temp1*10000)<=int(sigma*10000) && int(temp1*10000)>=int(-sigma*10000)) {
                flag = 1.0f;
            } else {
                flag = 0.0f;
            }
            fPtr_dst[i+iStep_dst*j]=temp2*flag;
        }
    }
    
    __END__;
}

CV_IMPL void cvCalS(const CvArr* srcarr,
                    CvArr* dstarr)
{
    CV_FUNCNAME("cvCalS");
    
    __BEGIN__;
    CvMat sstub, *src;
    CvMat dstub, *dst;
    CvMat* src_dx=0, *src_dy=0;
    CvSize size;
    int i, j;
    int iStep;
    float* fPtr;
    
    CV_CALL( src = cvGetMat(srcarr, &sstub ));
    CV_CALL( dst = cvGetMat(dstarr, &dstub ));
    
    if( CV_MAT_TYPE(src->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( CV_MAT_TYPE(dst->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( !CV_ARE_SIZES_EQ( src, dst ))
        CV_ERROR( CV_StsUnmatchedSizes, "The input images must have the same size" );
    
    size = cvGetMatSize( src );
    
    src_dx  = cvCreateMat(size.height, size.width, CV_32FC1 );
    src_dy  = cvCreateMat(size.height, size.width, CV_32FC1 );
    cvSetZero(src_dx);
    cvSetZero(src_dy);
    
    iStep = dst->step / sizeof(fPtr[0]);
    fPtr = dst->data.fl;
    
    cvSobel(src, src_dx, 1, 0, 1);
    cvSobel(src, src_dy, 0, 1, 1);
    cvMul(src_dx, src_dx, src_dx, 0.25f*0.25f); //rescale gradient
    cvMul(src_dy, src_dy, src_dy, 0.25f*0.25f); //rescale gradient
    cvAdd(src_dx, src_dy, dst);
    
    for(j=0; j<size.height; j++){
        for (i=0; i<size.width; i++)
            fPtr[i+iStep*j] = sqrt(fPtr[i+iStep*j])+SMALLNUM;
    }
    cvReleaseMat(&src_dx);
    cvReleaseMat(&src_dy);
    
    __END__;
}

CV_IMPL void cvCurvature(const CvArr* srcarr_x, 
                         const CvArr* srcarr_y,
                         CvArr* dstarr)
{
    CV_FUNCNAME("cvCurvature");
    
    __BEGIN__;
    
    CvMat sstub_x, sstub_y, *src_x, *src_y;
    CvMat dstub, *dst;
    CvSize size;
    CvMat *Nxx=0, *Nyy=0, *ones=0;
    
    CV_CALL( src_x = cvGetMat(srcarr_x, &sstub_x ));
    CV_CALL( src_y = cvGetMat(srcarr_y, &sstub_y ));
    CV_CALL( dst = cvGetMat(dstarr, &dstub ));
    
    if( CV_MAT_TYPE(src_x->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( CV_MAT_TYPE(src_y->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( CV_MAT_TYPE(dst->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( !CV_ARE_SIZES_EQ( src_x, src_y ))
        CV_ERROR( CV_StsUnmatchedSizes, "The input images must have the same size" );
    
    size = cvGetMatSize( src_x );
    Nxx = cvCreateMat(size.height, size.width, CV_32FC1 );
    Nyy = cvCreateMat(size.height, size.width, CV_32FC1 );
    ones= cvCreateMat(size.height, size.width, CV_32FC1 );
    cvSetZero(Nxx);
    cvSetZero(Nyy);
    cvSet(ones, cvScalar(1.0f));
    
    cvSobel(src_x, Nxx, 1, 0, 1);
    cvSobel(src_y, Nyy, 0, 1, 1);
    cvMul(Nxx, ones, Nxx, 0.25f);
    cvMul(Nyy, ones, Nyy, 0.25f);
    cvAdd(Nxx, Nyy, dst);
    cvReleaseMat(&Nxx);
    cvReleaseMat(&Nyy);
    cvReleaseMat(&ones);
    
    __END__;
    
}

CV_IMPL void cvDistReg(const CvArr* srcarr,
                       CvArr* dstarr)
{
    CV_FUNCNAME("cvDistReg");
    
    __BEGIN__;
    CvMat sstub, *src;
    CvMat dstub, *dst;
    CvMat* src_dx=0, *src_dy=0, *s=0, *ps=0;
    CvMat* dps_x=0, *dps_y=0, *del=0, *ones=0;
    CvSize size;
    int i, j, iStep_s, iStep_ps;
    float* fPtr_s, *fPtr_ps;
    float temp_s=0.0f, temp_ps=0.0f;
    float flag_s1=0.0f, flag_s2=0.0f, flag_ps1=0.0f, flag_ps2=0.0f;
    
    CV_CALL( src = cvGetMat(srcarr, &sstub ));
    CV_CALL( dst = cvGetMat(dstarr, &dstub ));
    
    if( CV_MAT_TYPE(src->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( CV_MAT_TYPE(dst->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( !CV_ARE_SIZES_EQ( src, dst ))
        CV_ERROR( CV_StsUnmatchedSizes, "The input images must have the same size" );
    size = cvGetMatSize( src );
    
    src_dx  = cvCreateMat(size.height, size.width, CV_32FC1 );
    src_dy  = cvCreateMat(size.height, size.width, CV_32FC1 );
    s  = cvCreateMat(size.height, size.width, CV_32FC1 );
    ps  = cvCreateMat(size.height, size.width, CV_32FC1 );
    dps_x  = cvCreateMat(size.height, size.width, CV_32FC1 );
    dps_y  = cvCreateMat(size.height, size.width, CV_32FC1 );
    del  = cvCreateMat(size.height, size.width, CV_32FC1 );
    ones = cvCreateMat(size.height, size.width, CV_32FC1 );
    cvSetZero(src_dx);
    cvSetZero(src_dy);
    cvSetZero(s);
    cvSetZero(ps);
    cvSetZero(dps_x);
    cvSetZero(dps_y);
    cvSetZero(del);
    cvSet(ones, cvScalar(1.0f));
    
    iStep_s = s->step / sizeof(fPtr_s[0]);
    fPtr_s  = s->data.fl;
    iStep_ps= ps->step/ sizeof(fPtr_ps[0]);
    fPtr_ps = ps->data.fl;
    
    cvSobel(src, src_dx, 1, 0, 1);
    cvSobel(src, src_dy, 0, 1, 1);
    cvMul(src_dx, ones, src_dx, 0.25f);
    cvMul(src_dy, ones, src_dy, 0.25f);
    cvCalS(src,s);
    
    for (j=0; j<size.height; j++){
        for (i=0; i<size.width; i++){
            temp_s = fPtr_s[i+iStep_s*j];
            if (int(temp_s*10000)>=0 && int(temp_s*10000)<=10000) {
                flag_s1 = 1.0f;
            } else {
                flag_s1 = 0.0f;
            }
            if (int(temp_s*10000) > 10000) {
                flag_s2 = 1.0f;
            } else {
                flag_s2 = 0.0f;
            }
            temp_ps = flag_s1*sin(2*PI*temp_s)/2/PI+flag_s2*(temp_s-1.0f);
            if (int(temp_ps*10000) == 0) {
                flag_ps1 = 0.0f;
            } else {
                flag_ps1 = 1.0f;
            }
            if (int(temp_s*10000) == 0) {
                flag_ps2 = 0.0f;
            } else {
                flag_ps2 = 1.0f;
            }
            fPtr_ps[i+iStep_ps*j] = (flag_ps1*temp_ps+1.0f-flag_ps1)/(flag_ps2*temp_s+1.0f-flag_ps2);
            if ((flag_ps2*temp_s+1.0f-flag_ps2)==0){
                printf("Something wrong in last: temp_s = %f, flag_ps2 = %f\n", temp_s, flag_ps2);
                exit(0);
            }
        }
    }
    cvMul(ps, src_dx, dps_x);
    cvMul(ps, src_dy, dps_y);
    cvSub(dps_x, src_dx, dps_x);
    cvSub(dps_y, src_dy, dps_y);
    cvCurvature(dps_x, dps_y, dst);
    cvLaplace(src,del,1);
    cvMul(del, ones, del, 0.2f);
    cvAdd(dst, del, dst);
    
    cvReleaseMat(&src_dx);
    cvReleaseMat(&src_dy);
    cvReleaseMat(&s);
    cvReleaseMat(&ps);
    cvReleaseMat(&dps_x);
    cvReleaseMat(&dps_y);
    cvReleaseMat(&del);
    cvReleaseMat(&ones);
    
    __END__;
}

CV_IMPL void cvDrlse_edge(CvArr * srcphi, 
                          CvArr * srcgrad,
                          CvArr * dstarr,
                          double lambda, 
                          double mu, 
                          double alfa, 
                          double epsilon, 
                          int timestep,
                          int iter)
{   
    CV_FUNCNAME( "cvDrlse_edge" );
    
    __BEGIN__;
    
    CvMat sstub1, sstub2, *phi, *grad;
    CvMat dstub, *dst;
    CvMat *gradx=0, *grady=0, *phi_0=0, *phix=0, *phiy=0;
    CvMat *s=0, *Nx=0, *Ny=0, *curvature=0, *distRegTerm=0;
    CvMat *diracPhi=0, *areaTerm=0, *edgeTerm=0;
    CvMat *temp1=0, *temp2=0, *temp3=0, *ones=0;
    CvSize size;
    int i;
    
    CV_CALL( phi    = cvGetMat(srcphi, &sstub1 ));
    CV_CALL( grad   = cvGetMat(srcgrad,   &sstub2 ));
    CV_CALL( dst    = cvGetMat(dstarr,  &dstub));
    
    if( CV_MAT_TYPE(phi->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( CV_MAT_TYPE(grad->type) != CV_32FC1)
        CV_ERROR( CV_StsUnsupportedFormat, "Only-32bit, 1-channel input images are supported" );
    
    if( !CV_ARE_SIZES_EQ( phi, grad ))
        CV_ERROR( CV_StsUnmatchedSizes, "The input images must have the same size" );
    size = cvGetMatSize( phi );
    
    //Initialization 
    gradx = cvCreateMat(size.height, size.width, CV_32FC1 );
    grady = cvCreateMat(size.height, size.width, CV_32FC1 );
    phi_0 = cvCreateMat(size.height, size.width, CV_32FC1 );
    phix  = cvCreateMat(size.height, size.width, CV_32FC1 );
    phiy  = cvCreateMat(size.height, size.width, CV_32FC1 );
    Nx    = cvCreateMat(size.height, size.width, CV_32FC1 );
    Ny    = cvCreateMat(size.height, size.width, CV_32FC1 );
    s     = cvCreateMat(size.height, size.width, CV_32FC1 );
    curvature= cvCreateMat(size.height, size.width, CV_32FC1 );
    distRegTerm= cvCreateMat(size.height, size.width, CV_32FC1 );
    diracPhi  = cvCreateMat(size.height, size.width, CV_32FC1 );
    areaTerm  = cvCreateMat(size.height, size.width, CV_32FC1 );
    edgeTerm  = cvCreateMat(size.height, size.width, CV_32FC1 );
    temp1  = cvCreateMat(size.height, size.width, CV_32FC1 );
    temp2  = cvCreateMat(size.height, size.width, CV_32FC1 );
    temp3  = cvCreateMat(size.height, size.width, CV_32FC1 );
    ones  = cvCreateMat(size.height, size.width, CV_32FC1 );
    cvSetZero(gradx);
    cvSetZero(grady);
    cvSetZero(phix);
    cvSetZero(phiy);
    cvSetZero(Nx);
    cvSetZero(Ny);
    cvSetZero(s);
    cvSetZero(curvature);
    cvSetZero(distRegTerm);
    cvSetZero(diracPhi);
    cvSetZero(areaTerm);
    cvSetZero(edgeTerm);
    cvSetZero(temp1);
    cvSetZero(temp2);
    cvSetZero(temp3);
    cvSet(ones, cvScalar(1.0f));
    
    //--------------BEGIN----------------------
    cvSobel(grad, gradx, 1, 0, 1);
    cvSobel(grad, grady, 0, 1, 1);
    cvMul(gradx, ones, gradx, 0.25f);
    cvMul(grady, ones, grady, 0.25f);
    cvCopy(phi, dst);
    
    for(i=0; i<iter; i++){
        cvNeumannBoundCond(dst, dst);
        cvSobel(dst, phix, 1, 0, 1);
        cvSobel(dst, phiy, 0, 1, 1);
        cvCalS(dst,s);
        cvDiv(phix, s, Nx, 0.25f);
        cvDiv(phiy, s, Ny, 0.25f);
        cvCurvature(Nx, Ny, curvature);
        cvDistReg(dst, distRegTerm);
        cvDirac(dst, diracPhi, epsilon);        //Compute driacPhi;
        cvMul(diracPhi, grad, areaTerm);        //Compute areaTerm
        
        cvMul(gradx, Nx, gradx);                //------------------//
        cvMul(grady, Ny, grady);                // Computing        //
        cvAdd(gradx, grady, temp1);             //                  //
        cvMul(diracPhi, temp1, temp2);          // edgeTerm         //
        cvMul(areaTerm, curvature, temp3);      //                  //
        cvAdd(temp2, temp3, edgeTerm);          //------------------//
        
        cvMul(distRegTerm, ones, distRegTerm, mu);              //  distRegTerm = mu     * distRegTerm
        cvMul(edgeTerm,    ones, edgeTerm,    lambda);          //  edgeTerm    = lambda * edgeTerm
        cvMul(areaTerm,    ones, areaTerm,    alfa);            //  areaTerm    = alfa   * areaTerm
        cvAdd(distRegTerm, edgeTerm, temp1);
        cvAdd(temp1, areaTerm, temp2);                          //  (distRegTerm + edgeTerm + areaTerm)
        cvMul(temp2, ones, temp2, double(timestep));            //  timestep * (distRegTerm + edgeTerm + areaTerm)
        cvAdd(dst, temp2, dst);                                 //  phi = phi + timestep * (distRegTerm + edgeTerm + areaTerm)
    }
    //----------------END------------------------
    
    // Clean up
    cvReleaseMat(&ones);
    cvReleaseMat(&phi_0);
    cvReleaseMat(&gradx);
    cvReleaseMat(&grady);
    cvReleaseMat(&phix);
    cvReleaseMat(&phiy);
    cvReleaseMat(&Nx);
    cvReleaseMat(&Ny);
    cvReleaseMat(&s);
    cvReleaseMat(&curvature);
    cvReleaseMat(&distRegTerm);
    cvReleaseMat(&diracPhi);
    cvReleaseMat(&areaTerm);
    cvReleaseMat(&edgeTerm);
    cvReleaseMat(&temp1);
    cvReleaseMat(&temp2);
    cvReleaseMat(&temp3);
    
    __END__;
    
}

CvPoint*
cvDRLSE(const CvArr * image,
        const CvArr * mask,
	int *length,
        double lambda,
        double alfa,
        double epsilon,
        int timestep,
        int ITER_ext,
        int ITER_int,
        int flag)
{
    IplImage sstub1, sstub2, *msk, *img, *marker, *levelset;
    CvMat *ones, *Ix, *Iy, *phi, *f, *g;
    CvSize size;
    int comp_count =0, iStep;
    float* fPtr;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours = 0;
    CvPoint pt= cvPoint(0,0), *point=NULL;
    double mu = 0.2f/double(timestep);
    char c;

    CV_FUNCNAME( "cvDRLSE" );
    
    __BEGIN__;
    
    CV_CALL( msk = cvGetImage(mask,    &sstub1 ));
    CV_CALL( img = cvGetImage(image,   &sstub2 ));    
    cvSmooth(img, img, CV_GAUSSIAN, 5, 5, 1.5f);
    size = cvGetSize(img);
    levelset = cvCreateImage(size, IPL_DEPTH_8U, 1);
    ones = cvCreateMat(size.height, size.width, CV_32FC1);
    Ix = cvCreateMat(size.height, size.width, CV_32FC1);
    Iy = cvCreateMat(size.height, size.width, CV_32FC1);
    phi = cvCreateMat(size.height, size.width, CV_32FC1);
    f = cvCreateMat(size.height, size.width, CV_32FC1);
    g = cvCreateMat(size.height, size.width, CV_32FC1);
    marker = cvCreateImage(size, IPL_DEPTH_32S, 1);
    cvSet(ones, cvScalar(1.0f));
    
    cvSobel(img, Ix, 1, 0, 1);
    cvSobel(img, Iy, 0, 1, 1);
    cvMul(Ix, Ix, Ix, 0.25f*0.25f);
    cvMul(Iy, Iy, Iy, 0.25f*0.25f);
    cvAdd(Ix, Iy, f);
    cvAdd(f, ones, f);
    cvDiv(NULL, f, g, 1.0f);
    
    cvFindContours( msk, storage, &contours, sizeof(CvContour),CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );
    cvZero(marker);
    for( ; contours != 0; contours = contours->h_next, comp_count++ )
    {
        cvDrawContours( marker, contours, cvScalarAll(255),cvScalarAll(255), -1, -1, 8);
    }
   
    iStep = phi->step/sizeof(fPtr[0]);
    fPtr = phi->data.fl;
    
    for (int j=0; j<size.height; j++)
        for (int i=0; i<size.width; i++) {
            int idx = CV_IMAGE_ELEM( marker, int, j, i );
            if (idx >0 )
                if (flag == CV_LSE_SHR)
                    fPtr[i+iStep*j]=-2.0f;
                else
                    fPtr[i+iStep*j]=2.0f;
            else
                if (flag == CV_LSE_SHR)
                    fPtr[i+iStep*j]=2.0f;
                else
                    fPtr[i+iStep*j]=-2.0f;
        }
    
    for (int i=0; i<ITER_ext; i++) {
        cvDrlse_edge(phi, g, phi, lambda, mu, alfa, epsilon, timestep, ITER_int);
	loadBar(i+1, ITER_ext, 50);
    }
    cvDrlse_edge(phi, g, phi, lambda, mu, 0.0f, epsilon, timestep, ITER_int);
    cvZero(msk);
    if (flag == CV_LSE_SHR)
        cvThreshold(phi, msk, 0.0f, 255, CV_THRESH_BINARY_INV);
    else
        cvThreshold(phi, msk, 0.0f, 255, CV_THRESH_BINARY);

    cvFindContours(msk, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    if(!contours) return 0;
    *length = contours->total;
    //if(*length<10) return 0;
    point = new CvPoint[*length];
    CvSeqReader reader;
    
    cvStartReadSeq(contours, &reader);
    for (int i = 0; i < *length; i++){
        CV_READ_SEQ_ELEM(pt, reader);
        point[i]=pt;
    }
  
    //clean up
    cvReleaseMemStorage(&storage);
    cvReleaseImage(&marker);
    cvReleaseMat(&Ix);
    cvReleaseMat(&Iy);
    cvReleaseMat(&f);
    cvReleaseMat(&g);
    cvReleaseMat(&phi);
    cvReleaseMat(&ones);
    
    return point;
    __END__;
}
