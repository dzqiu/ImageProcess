# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import cv2 as cv
import numpy as np
import os
import glob
#import tensorflow as tf 
import matplotlib.pyplot as plt  

print 'feature program run...'

DIM            = 426

# img_dir        = '/home/dzqiu/Documents/image/video/samples/mysample'
# threshold_dir  = '/home/dzqiu/Documents/image/video/samples/mysample/threshold/'
# contour_dir    = '/home/dzqiu/Documents/image/video/samples/mysample/contours/'
# incomplete_dir = '/home/dzqiu/Documents/image/video/samples/mysample/contours/incomplete/'
# complete_dir  =  '/home/dzqiu/Documents/image/video/samples/mysample/contours/complete/'

# img_dir        = '/home/dzqiu/Documents/image/video/samples/mysample'
#img_dir        = '/home/dzqiu/Documents/image/video/samples'
train_dir     	   = '/home/dzqiu/qt-workspace/Pill_Final/python/data/'
validation_dir     = '/home/dzqiu/qt-workspace/Pill_Final/python/validation/'

def DataAugmentation(img_dir):
	threshold_dir  = str(img_dir) +str('/threshold/')
	contour_dir    = str(img_dir) +str('/contours/')
	incomplete_dir = str(img_dir) +str('/contours/incomplete/')
	complete_dir   = str(img_dir) +str('/contours/complete/')
	##读取目录下所有jpeg文件
	img_list = glob.glob(os.path.join(img_dir,'*jpeg'))
	img_list.sort()
	##遍历各个文件，处理
	for img_dir_name in img_list:

	    [img_dir,img_name]=os.path.split(img_dir_name)
	    [name,extension]  =os.path.splitext(img_name)
	    img        = cv.imread(img_dir_name)
	    GrayImg    = cv.cvtColor(img,cv.COLOR_BGR2GRAY);
	    GrayImg    = cv.resize(GrayImg,(DIM, DIM))
	    ret,thresh = cv.threshold(GrayImg,0, 255, cv.THRESH_OTSU)
	    (cnts, _)  = cv.findContours(thresh.copy(),cv.RETR_EXTERNAL,cv.CHAIN_APPROX_NONE)

	    #########
	    print 'write threshold image->'+threshold_dir+img_name
	    result     = np.zeros((DIM,DIM,3),dtype=np.uint8)
	    cv.drawContours(result, cnts, 0, (255, 255, 255), -1)
	    cv.imwrite(threshold_dir+img_name,result)

	    print 'write contour image->'+threshold_dir+img_name
	    result     = np.zeros((DIM,DIM,3),dtype=np.uint8)
	    cv.drawContours(result, cnts, 0, (255, 255, 255), 1)
	    cv.imwrite(contour_dir+img_name,result)


	    contour = cnts[0]
	    
	    Len = len(contour)

	    cv.imwrite(incomplete_dir+name+'_'+str(100)+'_'+str(0)+extension,result)
	    cv.imwrite(complete_dir+name+'_'+str(100)+'_'+str(0)+extension,result)

	    for pstage in range(70,100,2):
		Minstep = 12
		if(Minstep<=0):
		    cv.imshow('result',result)
		    cv.imwrite(incomplete_dir+name+'_'+str(pstage)+'_'+str(0)+extension,result)
		    cv.line(result,begin_pt,dp,(255,255,255),1)      
		    cv.imwrite(complete_dir+name+'_'+str(pstage)+'_'+str(0)+extension,result)
		else:
		    for begin in range(0,Len,Minstep):
		        result     = np.zeros((DIM,DIM,3),dtype=np.uint8)
		        res_array  = []
		        begin_w = contour[begin][0][1]
		        begin_h = contour[begin][0][0]
		        begin_pt = (begin_h,begin_w)
		        if begin+round((Len*pstage*1.0)/100)>Len:
		            pt = (contour[begin][0][0],contour[begin][0][1])
		        #    res_array.append([pt[0],pt[1]])
		            for draw in range(begin,Len,1):
		                dp = (contour[draw][0][0],contour[draw][0][1])
		                cv.line(result,pt,dp,(255,255,255),1)
		            #    res_array.append(dp[0],dp[1])
		                pt=dp
		            pt = (contour[0][0][0],contour[0][0][1])
		        #    res_array.append([pt])
		            surplus  = int(round((Len*pstage*1.0)/100 - (Len-begin)))
		            for draw in range(0,surplus,1):
		                dp = (contour[draw][0][0],contour[draw][0][1])
		                cv.line(result,pt,dp,(255,255,255),1)
		            #    res_array.append([dp])
		                pt=dp
		        #    cv.line(result,begin_pt,dp,(255,255,255),1)
		        else:
		            pt = (contour[begin][0][0],contour[begin][0][1])
		            # res_array.append([pt])
		            surplus  = int(begin+round((Len*pstage*1.0)/100))
		            for draw in range(begin,surplus,1):
		                dp = (contour[draw][0][0],contour[draw][0][1])
		                cv.line(result,pt,dp,(255,255,255),1)
		                #  res_array.append([dp])
		                pt=dp
		    
		        cv.imshow('result',result)
		        cv.imwrite(incomplete_dir+name+'_'+str(pstage)+'_'+str(begin)+extension,result)
		        cv.line(result,begin_pt,dp,(255,255,255),1)      
		        cv.imwrite(complete_dir+name+'_'+str(pstage)+'_'+str(begin)+extension,result)

		        
		        
		        print 'write incomplete contour:'+img_name+':'+str(pstage)
		        # test     = np.zeros((DIM,DIM,3),dtype=np.uint8)
		        # print res_array
		        # cv.drawContours(test,res_array,-1,(255,255,255),-1)
		        # cv.imshow('test',test)
		        t=cv.waitKey(5)

	cv.destroyAllWindows() 
if __name__=='main':
	DataAugmentation(train_dir);
	DataAugmentation(validation_dir);
	


 
