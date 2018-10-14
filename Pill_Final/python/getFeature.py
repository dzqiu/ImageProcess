#!/usr/bin/python
#coding:utf-8

import cv2
import numpy as np
import math
import glob
import os

DIM = 512
def getFeature(srcImg):
    ret,thresh = cv2.threshold(srcImg,10,255,cv2.THRESH_BINARY)
    (scnts, _) = cv2.findContours(thresh.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    #the minimum bounding rectangle

    sArea    = cv2.contourArea(scnts[0])

    sMinRect = cv2.minAreaRect(scnts[0])
    sMinRectBox = np.int0(cv2.cv.BoxPoints(sMinRect))
    sMinRectWidth,sMinRectHeight = sMinRect[1]
    sMinRectArea = sMinRectWidth*sMinRectHeight
    sLength = cv2.arcLength(scnts[0],closed=False)

    #Feature 1：
    sMinRectWHRatio = sMinRectWidth/sMinRectHeight
    if(sMinRectWHRatio>1): sMinRectWHRatio = 1.0/sMinRectWHRatio
    
    fitEllipse_Img = np.zeros((DIM,DIM,1),dtype=np.uint8)
    FitEllipse = cv2.fitEllipse(scnts[0])
    # print FitEllipse
    cv2.ellipse(fitEllipse_Img,FitEllipse,(255),1)
    ret,thresh = cv2.threshold(fitEllipse_Img,100,255,cv2.THRESH_BINARY)
    (FitElpcnts, _)=cv2.findContours(thresh.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
    FEContour = scnts[0]
    FitEllipseError = 0
    # cv2.imshow('000',fitEllipse_Img)
    # cv2.waitKey()
    for i in range(0,len(FEContour)-1):
        pt = FEContour[i]
        Point = (pt[0][0],pt[0][1])
        FitEllipseError = FitEllipseError + abs(cv2.pointPolygonTest(FitElpcnts[0],Point,True))
    #Feature 2:
    FitEllipseError = FitEllipseError/len(FEContour)
    FitEllipseArea = cv2.contourArea(FitElpcnts[0])
    FitEllipseLength = cv2.arcLength(FitElpcnts[0],True)
    FEMinRect = cv2.minAreaRect(FitElpcnts[0])
    FEMinRectBox = np.int0(cv2.cv.BoxPoints(FEMinRect))
    FEMinRectWidth,FEMinRectHeight = FEMinRect[1]
    FEMinRectArea = FEMinRectWidth*FEMinRectHeight
    FEMinRectLength = (FEMinRectWidth+FEMinRectHeight)*2

    ###
    S_FEMinRectLenRatio = sLength/FEMinRectLength
    S_FELenRatio = sLength/FitEllipseLength
    #feature 3:
    FEMinRectWHRatio = FEMinRectWidth/FEMinRectHeight
    if(FEMinRectWHRatio>1): FEMinRectWHRatio = 1.0/FEMinRectWHRatio
    
    #feature 4:
    FE_FEMinRectAreaRatio = FitEllipseArea/FEMinRectArea
    #feature 5:
    FE_FEMinRectLengthRatio = FitEllipseLength/FEMinRectLength

    (cx,cy),cradius = cv2.minEnclosingCircle(FitElpcnts[0])
    FEMinCircleArea = cradius*cradius*math.pi
    FEMinCircleLength = 2*math.pi*cradius
    #feature 6:
    FE_FEMinCircleAreaRatio = FitEllipseArea/FEMinCircleArea
    #feature 7:
    FE_FEMinCircleLengthRatio = FitEllipseLength/FEMinCircleLength

    #total features 6 
    #x = [FitEllipseError,FEMinRectWHRatio,FE_FEMinRectAreaRatio,FE_FEMinRectLengthRatio,FE_FEMinCircleAreaRatio,FE_FEMinCircleLengthRatio]
    #total features 7 
    #x = [sMinRectWHRatio,FitEllipseError,FEMinRectWHRatio,FE_FEMinRectAreaRatio,FE_FEMinRectLengthRatio,FE_FEMinCircleAreaRatio,FE_FEMinCircleLengthRatio]
    x = [S_FELenRatio,FitEllipseError,FEMinRectWHRatio,FE_FEMinRectAreaRatio,FE_FEMinRectLengthRatio,FE_FEMinCircleAreaRatio,FE_FEMinCircleLengthRatio]
    # x = [sArea/FitEllipseArea,FitEllipseError,FEMinRectWHRatio,FE_FEMinRectAreaRatio,FE_FEMinRectLengthRatio,FE_FEMinCircleAreaRatio,FE_FEMinCircleLengthRatio]
    return x
def getLabel(name):
    y_lables = [0,0,0,0]
    if 'capsule' in name:
        y_lables=[1,0,0,0]      
    if 'circle' in name:
        y_lables=[0,1,0,0] 
    if 'ellipse' in name:
        y_lables=[0,0,1,0]
    if 'triangle' in name:
        y_lables=[0,0,0,1]
    # print y_lables
    return y_lables 

def getAllFeature(IMG_DIR):
    img_list = glob.glob(os.path.join(IMG_DIR,'*.jpeg'))
    img_list.sort()
    x_data=[]
    y_data=[]
    for img_dir_name in img_list:
        [img_dir,img_name]=os.path.split(img_dir_name)
        [name,extension]=os.path.splitext(img_name)
        y_lables = getLabel(name) 
        img = cv2.imread(img_dir_name,cv2.IMREAD_GRAYSCALE)
        x_feature=getFeature(img)
        if(np.size(x_data)==0):
            x_data = x_feature
            y_data = y_lables
        else:
            x_data=np.row_stack((x_data,x_feature))
            y_data=np.row_stack((y_data,y_lables))
    # max_x = np.argmax(x_data,axis=0)
    # for i in range(np.shape(x_data)[0])：
    #     x_data[i] = np.true_divide(x_data，max_x) 
    return (img_list,x_data,y_data)









